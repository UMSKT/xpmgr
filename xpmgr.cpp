// ReSharper disable CppEnforceOverridingFunctionStyle
typedef struct IUnknown IUnknown;

#include <windows.h>
#include "xpmgr.h"
#include "xpmgr_util.h"

static int option_exists(int argc, char** argv, const char* option) {
    for (int index = 1; index < argc; index++) {
        if (xpmgr_streq(argv[index], option)) {
            return 1;
        }
    }
    return 0;
}

static const char* option_value(int argc, char** argv, const char* option) {
    for (int index = 1; index < argc - 1; index++) {
        if (xpmgr_streq(argv[index], option)) {
            return argv[index + 1];
        }
    }
    return NULL;
}

static char* read_stdin_line(void) {
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    char* buffer;
    int count = 0;
    const int cap = 4096;
    if (input == NULL || input == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    buffer = (char*)HeapAlloc(GetProcessHeap(), 0, cap);
    if (!buffer) {
        return NULL;
    }
    while (count < cap - 1) {
        char ch;
        DWORD got = 0;
        if (!ReadFile(input, &ch, 1, &got, NULL) || got == 0) {
            break;
        }
        if (ch == '\n') {
            break;
        }
        buffer[count++] = ch;
    }
    while (count > 0) {
        char ch = buffer[count - 1];
        if (ch != ' ' && ch != '\t' && ch != '\r') {
            break;
        }
        count--;
    }
    buffer[count] = 0;
    return buffer;
}

static int finish(int code) {
    XPMGR_Cleanup();
    return code;
}

static int windows_command_selected(int argc, char** argv) {
    return option_exists(argc, argv, "/dti")
        || option_exists(argc, argv, "/atp")
        || option_exists(argc, argv, "/xpr")
        || option_exists(argc, argv, "/xpr-eval")
        || option_exists(argc, argv, "/ipk")
        || option_exists(argc, argv, "/dli");
}

/* Windows product activation is XP/2003 only. Office hosts must still reach command dispatch. */
static int windows_host_error(const OSVERSIONINFOA* info) {
    if (info->dwPlatformId == VER_PLATFORM_WIN32_NT
        && info->dwMajorVersion == 5
        && (info->dwMinorVersion == 1 || info->dwMinorVersion == 2)) {
        return 0;
    }
    xpmgr_write("An error occurred: Windows management only works on Windows NT 5.1 and 5.2.");
    if (info->dwPlatformId == VER_PLATFORM_WIN32_NT && info->dwMajorVersion > 5) {
        xpmgr_write(" Use slmgr instead: https://learn.microsoft.com/en-us/windows-server/get-started/activation-slmgr-vbs-options");
        return 3;
    }
    if (info->dwPlatformId == VER_PLATFORM_WIN32_NT && info->dwMajorVersion == 5 && info->dwMinorVersion == 0) {
        xpmgr_write(" You should be fine anyways, since Windows 2000 doesn't use Product Activation.");
        return 4;
    }
    if (info->dwPlatformId == VER_PLATFORM_WIN32_NT && info->dwMajorVersion == 5) {
        return 5;
    }
    return 2;
}

int main(int argc, char* argv[]) {
    const char* USAGE_TEXT =
        "xpmgr - Windows XP License Manager (compiled on " __DATE__ " " __TIME__ ")\n"
        "\n"
        "Usage: \n"
        "/dti: Gets the Installation ID\n"
        "/atp [cid]: Sets Confirmation ID (If successful, activates Windows/Office) (can also read from stdin)\n"
        "/xpr: Gets the days before activation is required (Grace period)\n"
        "/xpr-eval: Gets the days before the evaluation period expires (Eval. copies only)\n"
        "/ipk [pkey]: Sets/changes product key (can also read from stdin)\n"
        "/dli: Gets the product ID of Windows\n"
        "/?: Displays this message";
    const wchar_t* result = NULL;
    wchar_t* wide = NULL;

    if (option_exists(argc, argv, "/?")) {
        xpmgr_write(USAGE_TEXT);
        return 0;
    }

    OSVERSIONINFOA info = {};
    int hostError;
    info.dwOSVersionInfoSize = sizeof(info);
    if (!GetVersionExA(&info)) {
        xpmgr_write("An error occurred: Could not read the operating system version.\n");
        return 2;
    }

    if (info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && info.dwMinorVersion == 0) {
        xpmgr_write("An error occurred: This program requires Windows 98 or later.\n");
        return 2;
    }

    if (!windows_command_selected(argc, argv)) {
        xpmgr_write("An error occurred: No arguments listed\n\n");
        xpmgr_write(USAGE_TEXT);
        return 7;
    }

    hostError = windows_host_error(&info);
    if (hostError) {
        return hostError;
    }

#ifdef ENVIRONMENT32
    /* XP x64 WPA lives in the 64-bit process. Office is 32-bit, so this stays on the Windows path. */
    SYSTEM_INFO systemInfo;
    typedef void (WINAPI *GetNativeSystemInfoFn)(LPSYSTEM_INFO);
    HMODULE kernel = GetModuleHandleA("kernel32.dll");
    GetNativeSystemInfoFn nativeInfo = NULL;
    if (kernel) {
        nativeInfo = (GetNativeSystemInfoFn)GetProcAddress(kernel, "GetNativeSystemInfo");
    }
    if (nativeInfo) {
        nativeInfo(&systemInfo);
    } else {
        GetSystemInfo(&systemInfo);
    }
    if (systemInfo.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_INTEL) {
        if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
            xpmgr_write("An error occurred: Incorrect version of xpmgr. You need to download the x64 version.");
            return 1;
        }
        if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
            xpmgr_write("An error occurred: Windows Product Activation does not exist on this platform.");
            return 1;
        }
        xpmgr_write("An error occurred: Incorrect version of xpmgr. Send an issue at https://github.com/UMSKT/xpmgr/issues if you want to help us make a version for your platform!");
        return 1;
    }
#endif

    XPMGR_Initialize();

    if (option_exists(argc, argv, "/dti")) {
        result = XPMGR_GetInstallationID();
    }
    else if (option_exists(argc, argv, "/atp")) {
        const char* confirmationId = option_value(argc, argv, "/atp");
        char* piped = NULL;
        if (!confirmationId) {
            piped = read_stdin_line();
            if (!piped || !piped[0]) {
                xpmgr_write("An error occurred: No confirmation ID provided via argument or pipe\n\n");
                xpmgr_write(USAGE_TEXT);
                xpmgr_free(piped);
                return finish(7);
            }
            wide = xpmgr_widen(piped);
            xpmgr_free(piped);
        } else {
            wide = xpmgr_widen(confirmationId);
        }
        if (!wide) {
            xpmgr_write("An error occurred: Out of memory\n");
            return finish(7);
        }
        result = XPMGR_SetConfirmationID(wide);
    }
    else if (option_exists(argc, argv, "/xpr")) {
        result = XPMGR_GetWPALeft();
    }
    else if (option_exists(argc, argv, "/xpr-eval")) {
        result = XPMGR_GetEvalLeft();
    }
    else if (option_exists(argc, argv, "/ipk")) {
        const char* productKey = option_value(argc, argv, "/ipk");
        char* piped = NULL;
        if (!productKey) {
            piped = read_stdin_line();
            if (!piped || !piped[0]) {
                xpmgr_write("An error occurred: No product key provided via argument or pipe\n\n");
                xpmgr_write(USAGE_TEXT);
                xpmgr_free(piped);
                return finish(7);
            }
            wide = xpmgr_widen(piped);
            xpmgr_free(piped);
        } else {
            wide = xpmgr_widen(productKey);
        }
        if (!wide) {
            xpmgr_write("An error occurred: Out of memory\n");
            return finish(7);
        }
        result = XPMGR_SetProductKey(wide);
    }
    else if (option_exists(argc, argv, "/dli")) {
        result = XPMGR_GetProductID();
    }
    else {
        xpmgr_write("An error occurred: No arguments listed\n\n");
        xpmgr_write(USAGE_TEXT);
        return finish(7);
    }

    if (result) {
        char* output = xpmgr_narrow(result);
        xpmgr_write(output);
        xpmgr_free(output);
    }
    xpmgr_free(wide);

    XPMGR_Cleanup();
    return 0;
}
