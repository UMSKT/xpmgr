#include "xpmgr.h"
#include <windows.h>
#include <objbase.h>
#include <oleauto.h>
#include <TlHelp32.h>
#include "xpmgr_util.h"

// CLSID and IID definitions
static CLSID XP_CLSID = { 0xACADF079, 0xCBCD, 0x4032, {0x83, 0xF2, 0xFA, 0x47, 0xC4, 0xDB, 0x09, 0x6F} };
static IID XP_IID = { 0xB8CBAD79, 0x3F1F, 0x481A, { 0xBB, 0x0C, 0xE7, 0xBB, 0xD7, 0x7B, 0xDD, 0xD1 } };

// Interface definition
DECLARE_INTERFACE_(ICOMLicenseAgent, IDispatch)
{
protected:
    ~ICOMLicenseAgent() = default;

public:
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID* ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)() PURE;
    STDMETHOD_(ULONG, Release)() PURE;

    /*** IDispatch methods ***/
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT FAR* pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT itinfo, LCID lcid, ITypeInfo FAR* FAR* pptinfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, OLECHAR FAR* FAR* rgszNames, UINT cNames, LCID lcid, DISPID FAR* rgdispid) PURE;
    STDMETHOD(Invoke)(THIS_ DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR* pdispparams, VARIANT FAR* pvarResult, EXCEPINFO FAR* pexcepinfo, UINT FAR* puArgErr) PURE;

    /*** ICOMLicenseAgent methods ***/
    STDMETHOD(Initialize)(THIS_ ULONG dwBPC, ULONG dwMode, BSTR bstrLicSource, ULONG* pdwRetCode) PURE;
    STDMETHOD(GetFirstName)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetFirstName)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetLastName)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetLastName)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetOrgName)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetOrgName)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetEmail)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetEmail)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetPhone)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetPhone)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetAddress1)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetAddress1)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetCity)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetCity)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetState)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetState)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetCountryCode)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetCountryCode)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetCountryDesc)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetCountryDesc)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetZip)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetZip)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetIsoLanguage)(THIS_ ULONG* pdwVal) PURE;
    STDMETHOD(SetIsoLanguage)(THIS_ ULONG dwNewVal) PURE;
    STDMETHOD(GetMSUpdate)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetMSUpdate)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetMSOffer)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetMSOffer)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetOtherOffer)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetOtherOffer)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(GetAddress2)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(SetAddress2)(THIS_ BSTR bstrNewVal) PURE;
    STDMETHOD(AsyncProcessHandshakeRequest)(THIS_ LONG bReviseCustInfo) PURE;
    STDMETHOD(AsyncProcessNewLicenseRequest)() PURE;
    STDMETHOD(AsyncProcessReissueLicenseRequest)() PURE;
    STDMETHOD(AsyncProcessReviseCustInfoRequest)() PURE;
    STDMETHOD(GetAsyncProcessReturnCode)(THIS_ ULONG* pdwRetCode) PURE;
    STDMETHOD(AsyncProcessDroppedLicenseRequest)() PURE;
    STDMETHOD(GenerateInstallationId)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(DepositConfirmationId)(THIS_ BSTR bstrVal, ULONG* pdwRetCode) PURE;
    STDMETHOD(GetExpirationInfo)(THIS_ ULONG* pdwWPALeft, ULONG* pdwEvalLeft) PURE;
    STDMETHOD(AsyncProcessRegistrationRequest)() PURE;
    STDMETHOD(ProcessHandshakeRequest)(THIS_ LONG bReviseCustInfo) PURE;
    STDMETHOD(ProcessNewLicenseRequest)() PURE;
    STDMETHOD(ProcessDroppedLicenseRequest)() PURE;
    STDMETHOD(ProcessReissueLicenseRequest)() PURE;
    STDMETHOD(ProcessReviseCustInfoRequest)() PURE;
    STDMETHOD(EnsureInternetConnection)() PURE;
    STDMETHOD(SetProductKey)(THIS_ LPWSTR pszNewProductKey) PURE;
    STDMETHOD(GetProductID)(THIS_ BSTR* pbstrVal) PURE;
    STDMETHOD(VerifyCheckDigits)(THIS_ BSTR bstrCIDIID, LONG* pbValue) PURE;
};

// Static variables
static BOOL XP_ComInitialized = FALSE;
static ICOMLicenseAgent* XP_LicenseAgent = nullptr;
static wchar_t LastErrorMessage[1024] = L"";

// Helper functions
static void SetLastErrorMessage(const wchar_t* message) {
    lstrcpynW(LastErrorMessage, message, 1024);
}

static unsigned wide_length(const wchar_t* text) {
    unsigned length = 0;
    if (!text) {
        return 0;
    }
    while (text[length]) {
        length++;
    }
    return length;
}

static void write_ulong_wide(unsigned long value, wchar_t* out) {
    wchar_t digits[16];
    int count = 0;
    int index = 0;
    if (value == 0) {
        out[0] = L'0';
        out[1] = 0;
        return;
    }
    while (value && count < 16) {
        digits[count++] = (wchar_t)(L'0' + (value % 10));
        value /= 10;
    }
    while (count > 0) {
        out[index++] = digits[--count];
    }
    out[index] = 0;
}

static int product_key_char(wchar_t ch) {
    const wchar_t* alphabet = L"2346789BCDFGHJKMPQRTVWXY";
    for (int index = 0; alphabet[index]; index++) {
        if (alphabet[index] == ch) {
            return 1;
        }
    }
    return 0;
}

static int product_key_ok(const wchar_t* key) {
    if (!key) {
        return 0;
    }
    for (int group = 0; group < 5; group++) {
        if (group) {
            if (*key != L'-') {
                return 0;
            }
            key++;
        }
        for (int index = 0; index < 5; index++) {
            if (!product_key_char(*key)) {
                return 0;
            }
            key++;
        }
    }
    return *key == 0;
}

static BOOL XP_LoadLicenseManager() {
    if (!XP_ComInitialized) {
        const HRESULT status = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (FAILED(status)) {
            SetLastErrorMessage(L"Failed to initialize COM");
            return FALSE;
        }
        XP_ComInitialized = TRUE;
    }

    if (!XP_LicenseAgent) {
        HRESULT status = CoCreateInstance(XP_CLSID, nullptr, CLSCTX_INPROC_SERVER, XP_IID, reinterpret_cast<void**>(&XP_LicenseAgent));
        if (SUCCEEDED(status)) {
            ULONG dwRetCode;
            status = XP_LicenseAgent->Initialize(0xC475, 3, nullptr, &dwRetCode);
            if (SUCCEEDED(status) && dwRetCode == 0) {
                return TRUE;
            }
            XP_LicenseAgent->Release();
            XP_LicenseAgent = nullptr;
        }
        SetLastErrorMessage(L"Failed to create license manager instance");
        return FALSE;
    }
    return TRUE;
}

// Helper function to verify a confirmation ID chunk
static bool VerifyConfirmationIDChunk(ICOMLicenseAgent* agent, const wchar_t* chunk) {
    LONG pbValue = 0;
    BSTR bstrChunk = SysAllocString(chunk);
    HRESULT status = agent->VerifyCheckDigits(bstrChunk, &pbValue);
    SysFreeString(bstrChunk);
    return SUCCEEDED(status) && pbValue != 0;
}

struct ProcessHandle {
    HANDLE handle;
    explicit ProcessHandle(HANDLE h) : handle(h) {}
    ~ProcessHandle() {
        if (handle && handle != INVALID_HANDLE_VALUE) {
            CloseHandle(handle);
        }
    }
    operator HANDLE() const { return handle; }
};

typedef HANDLE (WINAPI *CreateToolhelp32SnapshotFn)(DWORD flags, DWORD processId);
typedef BOOL (WINAPI *Process32Fn)(HANDLE snapshot, LPPROCESSENTRY32 entry);

struct Toolhelp {
    HANDLE snapshot;
    Process32Fn first;
    Process32Fn next;
};

/* NT 4 has no ToolHelp in kernel32. Windows 98 has the ANSI exports.
   Resolve them here so neither host gets a static import. Missing exports skip cleanup. */
static int open_process_snapshot(Toolhelp* toolhelp) {
    HMODULE kernel = GetModuleHandleA("kernel32.dll");
    CreateToolhelp32SnapshotFn createSnapshot;
    if (!kernel) {
        return 0;
    }
    createSnapshot = (CreateToolhelp32SnapshotFn)GetProcAddress(kernel, "CreateToolhelp32Snapshot");
    toolhelp->first = (Process32Fn)GetProcAddress(kernel, "Process32First");
    toolhelp->next = (Process32Fn)GetProcAddress(kernel, "Process32Next");
    if (!createSnapshot || !toolhelp->first || !toolhelp->next) {
        return 0;
    }
    toolhelp->snapshot = createSnapshot(TH32CS_SNAPPROCESS, 0);
    if (toolhelp->snapshot == INVALID_HANDLE_VALUE) {
        return -1;
    }
    return 1;
}

static bool IsProcessRunning(const char* processName) {
    Toolhelp toolhelp;
    int opened = open_process_snapshot(&toolhelp);
    if (opened == 0) {
        return false;
    }
    if (opened < 0) {
        xpmgr_write("Failed to create process snapshot: ");
        xpmgr_write_uint(GetLastError());
        xpmgr_write("\n");
        return false;
    }

    ProcessHandle snapshot(toolhelp.snapshot);
    PROCESSENTRY32 processEntry{};
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (!toolhelp.first(snapshot, &processEntry)) {
        xpmgr_write("Failed to retrieve process information: ");
        xpmgr_write_uint(GetLastError());
        xpmgr_write("\n");
        return false;
    }

    do {
        if (lstrcmpiA(processEntry.szExeFile, processName) == 0) {
            return true;
        }
    } while (toolhelp.next(snapshot, &processEntry));

    return false;
}

static bool TerminateProcessByName(const char* processName) {
    Toolhelp toolhelp;
    int opened = open_process_snapshot(&toolhelp);
    if (opened == 0) {
        return false;
    }
    if (opened < 0) {
        xpmgr_write("Failed to create process snapshot: ");
        xpmgr_write_uint(GetLastError());
        xpmgr_write("\n");
        return false;
    }

    ProcessHandle snapshot(toolhelp.snapshot);
    PROCESSENTRY32 processEntry{};
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (!toolhelp.first(snapshot, &processEntry)) {
        xpmgr_write("Failed to retrieve process information: ");
        xpmgr_write_uint(GetLastError());
        xpmgr_write("\n");
        return false;
    }

    do {
        if (lstrcmpiA(processEntry.szExeFile, processName) == 0) {
            ProcessHandle processHandle(OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID));
            if (processHandle == nullptr) {
                xpmgr_write("Failed to open process: ");
                xpmgr_write_uint(GetLastError());
                xpmgr_write("\n");
                return false;
            }

            if (!TerminateProcess(processHandle, 0)) {
                xpmgr_write("Failed to terminate process: ");
                xpmgr_write_uint(GetLastError());
                xpmgr_write("\n");
                return false;
            }

            return true;
        }
    } while (toolhelp.next(snapshot, &processEntry));

    xpmgr_write("Process not found: ");
    xpmgr_write(processName);
    xpmgr_write("\n");
    return false;
}

// Exported functions implementation
extern "C" {

void XPMGR_Initialize() {
    XP_LoadLicenseManager();
}

void XPMGR_Cleanup() {
    if (XP_LicenseAgent) {
        XP_LicenseAgent->Release();
        XP_LicenseAgent = nullptr;
    }
    if (XP_ComInitialized) {
        CoUninitialize();
        XP_ComInitialized = FALSE;
    }
}

const wchar_t* XPMGR_GetLastError() {
    return LastErrorMessage;
}

const wchar_t* XPMGR_GetInstallationID() {
    if (!XP_LoadLicenseManager()) {
        return XPMGR_GetLastError();
    }

    BSTR installationID = nullptr;
    const HRESULT status = XP_LicenseAgent->GenerateInstallationId(&installationID);
    if (FAILED(status) || !installationID) {
        SetLastErrorMessage(L"Failed to generate installation ID");
        return XPMGR_GetLastError();
    }
    
    static wchar_t result[256];
    lstrcpynW(result, installationID, 256);
    SysFreeString(installationID);
    return result;
}

const wchar_t* XPMGR_SetConfirmationID(const wchar_t* confirmationId) {
    if (!XP_LoadLicenseManager()) {
        return XPMGR_GetLastError();
    }

    if (!confirmationId) {
        SetLastErrorMessage(L"Invalid confirmation ID - verification failed");
        return XPMGR_GetLastError();
    }

    // Create a working copy and remove dashes
    wchar_t cleanCid[256];
    const wchar_t* src = confirmationId;
    wchar_t* dst = cleanCid;
    while (*src && (dst - cleanCid) < 255) {
        if (*src != L'-') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = 0;

    // Verify each 6-character chunk
    unsigned len = wide_length(cleanCid);
    for (unsigned i = 0; i < len; i += 6) {
        wchar_t chunk[7];
        unsigned count = 0;
        while (count < 6 && cleanCid[i + count]) {
            chunk[count] = cleanCid[i + count];
            count++;
        }
        chunk[count] = 0;
        if (!VerifyConfirmationIDChunk(XP_LicenseAgent, chunk)) {
            SetLastErrorMessage(L"Invalid confirmation ID - verification failed");
            return XPMGR_GetLastError();
        }
    }

    // Pass the original confirmation ID (with dashes if present) to the API
    BSTR bstrConfirmationId = SysAllocString(confirmationId);
    ULONG dwRetCode;
    const HRESULT status = XP_LicenseAgent->DepositConfirmationId(bstrConfirmationId, &dwRetCode);
    SysFreeString(bstrConfirmationId);

    if (FAILED(status) || dwRetCode) {
        SetLastErrorMessage(L"Failed to deposit confirmation ID");
        return XPMGR_GetLastError();
    }

    // Handle WPA notifier process
    if (IsProcessRunning("wpabaln.exe")) {
        if (!TerminateProcessByName("wpabaln.exe")) {
            xpmgr_write("Warning: Failed to terminate WPA notifier process.\n");
        }
    }

    // Clean up activation files
    HMODULE hMod = LoadLibraryA("setupapi.dll");
    if (hMod) {
        using InstallHinfSectionFunc = BOOL (WINAPI*)(HWND, HINSTANCE, PCWSTR, INT);
        if (auto installHinfSection = reinterpret_cast<InstallHinfSectionFunc>(GetProcAddress(hMod, "InstallHinfSectionW"))) {
            SetLastError(0);
            const BOOL result = installHinfSection(nullptr, nullptr, L"DEL_OOBE_ACTIVATE 132 syssetup.inf", 132);
            const DWORD error = GetLastError();
            
            if (!result && error != 0 && error != 6) {  // Ignore error 6 (ERROR_INVALID_HANDLE) as it indicates success
                xpmgr_write("Warning: Failed to remove activation reminders. Error: ");
                xpmgr_write_uint(error);
                xpmgr_write("\n");
                xpmgr_write("You can try to run the following command yourself: \n");
                xpmgr_write("rundll32 setupapi,InstallHinfSection DEL_OOBE_ACTIVATE 132 syssetup.inf\n");
            }
        }
        FreeLibrary(hMod);
    }

    return L"Successfully set confirmation ID";
}

const wchar_t* XPMGR_GetWPALeft() {
    if (!XP_LoadLicenseManager()) {
        return XPMGR_GetLastError();
    }

    ULONG dwWPALeft = 0, dwEvalLeft = 0;
    const HRESULT status = XP_LicenseAgent->GetExpirationInfo(&dwWPALeft, &dwEvalLeft);
    if (FAILED(status)) {
        SetLastErrorMessage(L"Failed to get expiration info");
        return XPMGR_GetLastError();
    }

    if (dwWPALeft == 0x7FFFFFFF) {
        return L"Windows is activated";
    }

    static wchar_t result[16];
    write_ulong_wide(dwWPALeft, result);
    return result;
}

const wchar_t* XPMGR_GetEvalLeft() {
    if (!XP_LoadLicenseManager()) {
        return XPMGR_GetLastError();
    }

    ULONG dwWPALeft = 0, dwEvalLeft = 0;
    const HRESULT status = XP_LicenseAgent->GetExpirationInfo(&dwWPALeft, &dwEvalLeft);
    if (FAILED(status)) {
        SetLastErrorMessage(L"Failed to get expiration info");
        return XPMGR_GetLastError();
    }

    if (dwEvalLeft == 0x7FFFFFFF) {
        return L"Not an evaluation copy";
    }

    static wchar_t result[16];
    write_ulong_wide(dwEvalLeft, result);
    return result;
}

const wchar_t* XPMGR_SetProductKey(const wchar_t* productKey) {
    if (!XP_LoadLicenseManager()) {
        return XPMGR_GetLastError();
    }

    if (!product_key_ok(productKey)) {
        SetLastErrorMessage(L"Invalid product key format");
        return XPMGR_GetLastError();
    }

    const HRESULT status = XP_LicenseAgent->SetProductKey(const_cast<LPWSTR>(productKey));
    if (FAILED(status)) {
        SetLastErrorMessage(L"Failed to set product key");
        return XPMGR_GetLastError();
    }

    return L"Successfully set product key";
}

const wchar_t* XPMGR_GetProductID() {
    if (!XP_LoadLicenseManager()) {
        return XPMGR_GetLastError();
    }

    BSTR productID = nullptr;
    const HRESULT status = XP_LicenseAgent->GetProductID(&productID);
    if (FAILED(status) || !productID) {
        SetLastErrorMessage(L"Failed to get product ID");
        return XPMGR_GetLastError();
    }

    static wchar_t result[256];
    lstrcpynW(result, productID, 256);
    SysFreeString(productID);
    return result;
}

} 