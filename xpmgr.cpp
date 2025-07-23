// ReSharper disable CppEnforceOverridingFunctionStyle
typedef struct IUnknown IUnknown;

#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "xpmgr.h"

// Command line handling
struct CommandLineArgs {
    static const char* getCmdOption(char** begin, char** end, const std::string& option) {
        const char* opt = option.c_str();
        char** itr = std::find_if(begin, end, [opt](const char* arg) { return strcmp(arg, opt) == 0; });
        if (itr != end && ++itr != end) {
            return *itr;
        }
        return nullptr;
    }

    static bool cmdOptionExists(char** begin, char** end, const std::string& option) {
        const char* opt = option.c_str();
        return std::find_if(begin, end, [opt](const char* arg) { return strcmp(arg, opt) == 0; }) != end;
    }

    static std::string readFromStdin() {
        std::string input;
        if (!std::cin.eof() && std::getline(std::cin, input)) {
            // Remove any trailing whitespace, newlines or carriage returns
            input.erase(std::find_if(input.rbegin(), input.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), input.end());
            return input;
        }
        return "";
    }
};

// Helper function to convert char* to wchar_t*
wchar_t* convertCharArrayToLPCWSTR(const char* charArray) {
    auto* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}

// Helper function to convert wchar_t* to char*
char* convertWCharToChar(const wchar_t* wcharArray) {
    const int size = WideCharToMultiByte(CP_UTF8, 0, wcharArray, -1, nullptr, 0, nullptr, nullptr);
    auto* charStr = new char[size];
    WideCharToMultiByte(CP_UTF8, 0, wcharArray, -1, charStr, size, nullptr, nullptr);
    return charStr;
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

    if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "--GetUsage")) {
        std::cout << USAGE_TEXT;
        return 0;
    }

    // Check Windows version
    OSVERSIONINFOEX info = {};
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx(reinterpret_cast<LPOSVERSIONINFO>(&info));

    if (info.dwMajorVersion != 5) {
        std::cout << "An error occurred: Windows management only works on Windows NT 5.1 and 5.2.";
        if (info.dwMajorVersion > 5) {
            std::cout << " Use slmgr instead: https://learn.microsoft.com/en-us/windows-server/get-started/activation-slmgr-vbs-options";
            return 3;
        }
        return 2;
    }

    if (info.dwMinorVersion != 1 && info.dwMinorVersion != 2) {
        std::cout << "An error occurred: Windows management only works on Windows NT 5.1 and 5.2.";
        if (info.dwMinorVersion == 0) {
            std::cout << " You should be fine anyways, since Windows 2000 doesn't use Product Activation.";
            return 4;
        }
        return 5;
    }

    // Check architecture
#ifdef ENVIRONMENT32
    SYSTEM_INFO systemInfo;
    GetNativeSystemInfo(&systemInfo);
    if (systemInfo.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_INTEL) {
        if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
            std::cout << "An error occurred: Incorrect version of xpmgr. You need to download the x64 version.";
            return 1;
        }
        if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
            std::cout << "An error occurred: Windows Product Activation does not exist on this platform.";
            return 1;
        }
        std::cout << "An error occurred: Incorrect version of xpmgr. Send an issue at https://github.com/UMSKT/xpmgr/issues if you want to help us make a version for your platform!";
        return 1;
    }
#endif

    // Initialize the library
    XPMGR_Initialize();

    // Handle commands
    const wchar_t* result = nullptr;
    if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/dti")) {
        result = XPMGR_GetInstallationID();
    }
    else if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/atp")) {
        const char* confirmationId = CommandLineArgs::getCmdOption(argv, argv + argc, "/atp");
        if (!confirmationId) {
            std::string pipedInput = CommandLineArgs::readFromStdin();
            if (!pipedInput.empty()) {
                result = XPMGR_SetConfirmationID(convertCharArrayToLPCWSTR(pipedInput.c_str()));
            } else {
                std::cout << "An error occurred: No confirmation ID provided via argument or pipe\n\n" << USAGE_TEXT;
                XPMGR_Cleanup();
                return 7;
            }
        } else {
            result = XPMGR_SetConfirmationID(convertCharArrayToLPCWSTR(confirmationId));
        }
    }
    else if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/xpr")) {
        result = XPMGR_GetWPALeft();
    }
    else if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/xpr-eval")) {
        result = XPMGR_GetEvalLeft();
    }
    else if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/ipk")) {
        const char* productKey = CommandLineArgs::getCmdOption(argv, argv + argc, "/ipk");
        if (!productKey) {
            std::string pipedInput = CommandLineArgs::readFromStdin();
            if (!pipedInput.empty()) {
                result = XPMGR_SetProductKey(convertCharArrayToLPCWSTR(pipedInput.c_str()));
            } else {
                std::cout << "An error occurred: No product key provided via argument or pipe\n\n" << USAGE_TEXT;
                XPMGR_Cleanup();
                return 7;
            }
        } else {
            result = XPMGR_SetProductKey(convertCharArrayToLPCWSTR(productKey));
        }
    }
    else if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/dli")) {
        result = XPMGR_GetProductID();
    }
    else {
        std::cout << "An error occurred: No arguments listed\n\n" << USAGE_TEXT;
        XPMGR_Cleanup();
        return 7;
    }

    if (result) {
        char* output = convertWCharToChar(result);
        std::cout << output;
        delete[] output;
    }

    XPMGR_Cleanup();
    return 0;
}
