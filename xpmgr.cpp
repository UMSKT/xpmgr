// ReSharper disable CppEnforceOverridingFunctionStyle
typedef struct IUnknown IUnknown;

#include <windows.h>
#include <iostream>
#include <comdef.h>
#include <regex>
#include <TlHelp32.h>
#include <string>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Forward declarations of helper functions
BSTR FormatErrorMessage(const char* context, HRESULT status, ULONG returnCode = 0);
std::string ConvertToStdString(BSTR bstr);
BSTR ConvertCharToBSTR(const char* charString);

const char* specifiedProduct = nullptr;

// This is a really weird way of making a GUID. In short, this becomes ACADF079-CBCD-4032-83F2-FA47C4DB096F. Ignore the 0x and it makes sense.
static CLSID XP_CLSID = { 0xACADF079, 0xCBCD, 0x4032, {0x83, 0xF2, 0xFA, 0x47, 0xC4, 0xDB, 0x09, 0x6F} };
static IID XP_IID = { 0xB8CBAD79, 0x3F1F, 0x481A, { 0xBB, 0x0C, 0xE7, 0xBB, 0xD7, 0x7B, 0xDD, 0xD1 } };

#undef XP_INTERFACE
#define XP_INTERFACE ICOMLicenseAgent
DECLARE_INTERFACE_(ICOMLicenseAgent, IDispatch)
{
protected:
	~ICOMLicenseAgent() = default;

public:

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
	STDMETHOD_(ULONG, AddRef)() PURE;
	STDMETHOD_(ULONG, Release)() PURE;

	/*** IDispatch methods ***/
	STDMETHOD(GetTypeInfoCount)(THIS_ UINT FAR * pctinfo) PURE;
	STDMETHOD(GetTypeInfo)(THIS_ UINT itinfo, LCID lcid, ITypeInfo FAR * FAR * pptinfo) PURE;
	STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, OLECHAR FAR * FAR * rgszNames, UINT cNames, LCID lcid, DISPID FAR * rgdispid) PURE;
	STDMETHOD(Invoke)(THIS_ DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR * pdispparams, VARIANT FAR * pvarResult, EXCEPINFO FAR * pexcepinfo, UINT FAR * puArgErr) PURE;

	/*** ICOMLicenseAgent methods ***/
	STDMETHOD(Initialize)(THIS_ ULONG dwBPC, ULONG dwMode, BSTR bstrLicSource, ULONG * pdwRetCode) PURE;
	STDMETHOD(GetFirstName)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetFirstName)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetLastName)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetLastName)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetOrgName)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetOrgName)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetEmail)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetEmail)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetPhone)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetPhone)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetAddress1)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetAddress1)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetCity)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetCity)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetState)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetState)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetCountryCode)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetCountryCode)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetCountryDesc)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetCountryDesc)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetZip)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetZip)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetIsoLanguage)(THIS_ ULONG * pdwVal) PURE;
	STDMETHOD(SetIsoLanguage)(THIS_ ULONG dwNewVal) PURE;
	STDMETHOD(GetMSUpdate)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetMSUpdate)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetMSOffer)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetMSOffer)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetOtherOffer)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetOtherOffer)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetAddress2)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetAddress2)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(AsyncProcessHandshakeRequest)(THIS_ LONG bReviseCustInfo) PURE;
	STDMETHOD(AsyncProcessNewLicenseRequest)() PURE;
	STDMETHOD(AsyncProcessReissueLicenseRequest)() PURE;
	STDMETHOD(AsyncProcessReviseCustInfoRequest)() PURE;
	STDMETHOD(GetAsyncProcessReturnCode)(THIS_ ULONG * pdwRetCode) PURE;
	STDMETHOD(AsyncProcessDroppedLicenseRequest)() PURE;
	STDMETHOD(GenerateInstallationId)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(DepositConfirmationId)(THIS_ BSTR bstrVal, ULONG * pdwRetCode) PURE;
	STDMETHOD(GetExpirationInfo)(THIS_ ULONG * pdwWPALeft, ULONG * pdwEvalLeft) PURE;
	STDMETHOD(AsyncProcessRegistrationRequest)() PURE;
	STDMETHOD(ProcessHandshakeRequest)(THIS_ LONG bReviseCustInfo) PURE;
	STDMETHOD(ProcessNewLicenseRequest)() PURE;
	STDMETHOD(ProcessDroppedLicenseRequest)() PURE;
	STDMETHOD(ProcessReissueLicenseRequest)() PURE;
	STDMETHOD(ProcessReviseCustInfoRequest)() PURE;
	STDMETHOD(EnsureInternetConnection)() PURE;
	STDMETHOD(SetProductKey)(THIS_ LPWSTR pszNewProductKey) PURE;
	STDMETHOD(GetProductID)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(VerifyCheckDigits)(THIS_ BSTR bstrCIDIID, LONG * pbValue) PURE;
};

static BOOL XP_ComInitialized = FALSE;
static ICOMLicenseAgent* XP_LicenseAgent = nullptr;

static BOOL XP_LoadLicenseManager() {
    if (!XP_ComInitialized) {
        const HRESULT status = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (FAILED(status)) {
            std::cout << ConvertToStdString(FormatErrorMessage("CoInitializeEx", status));
            return FALSE;
        }
        XP_ComInitialized = TRUE;
    }

    if (!XP_LicenseAgent) {
        HRESULT status = CoCreateInstance(XP_CLSID, nullptr, CLSCTX_INPROC_SERVER, XP_IID, reinterpret_cast<void **>(&XP_LicenseAgent));
        if (SUCCEEDED(status)) {
            ULONG dwRetCode;
            status = XP_LicenseAgent->Initialize(0xC475, 3, nullptr, &dwRetCode);
            if (SUCCEEDED(status) && dwRetCode == 0) {
                return TRUE;
            }
            XP_LicenseAgent->Release();
            XP_LicenseAgent = nullptr;
        }
        std::cout << ConvertToStdString(FormatErrorMessage("CoCreateInstance", status));
        return FALSE;
    }
    return TRUE;
}

#pragma region Internals

// Add function to read from stdin
std::string readFromStdin() {
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

wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
	auto* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

char* getCmdOption(char** begin, char** end, const std::string& option)
{
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return nullptr;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

char* BstrToChar(BSTR bstr)
{
	const int len = static_cast<int>(::SysStringLen(bstr));
	const int bufSize = ::WideCharToMultiByte(CP_UTF8, 0, bstr, len, nullptr, 0, nullptr, nullptr);
	const auto buffer = new char[bufSize + 1];
	::WideCharToMultiByte(CP_UTF8, 0, bstr, len, buffer, bufSize, nullptr, nullptr);
	buffer[bufSize] = '\0';
	return buffer;
}

ULONG ConvertToULONG(const char* str)
{
	char* end;
	const ULONG value = std::strtoul(str, &end, 10);
	return value;
}

OLECHAR SizeToOLECHAR(size_t value)
{
	// Convert size_t to wstring
	const std::wstring wideString = std::to_wstring(value);

	// Retrieve the first character from the wide string
	const OLECHAR oChar = wideString[0];

	return oChar;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
void safeStrncat(char* destination, const char* source, size_t size) {
    const size_t destLen = strlen(destination);
    const size_t srcLen = strlen(source);

    if (destLen + srcLen < size) {
        // fuck off Microsoft
        strncat(destination, source, size - destLen - 1); // -1 for the null terminator
    } else {
        // Handle buffer overflow error
        std::cout << "An error occurred at safeStrncat: Buffer overflow during strncat operation." << std::endl;
    }
}
#pragma clang diagnostic pop

#pragma region Process Management

struct ProcessHandle {
    HANDLE handle;
    explicit ProcessHandle(HANDLE h) : handle(h) {}
    ~ProcessHandle() { if (handle != INVALID_HANDLE_VALUE) CloseHandle(handle); }
    operator HANDLE() const { return handle; }
};

bool IsProcessRunning(const wchar_t* processName) {
    ProcessHandle snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create process snapshot: " << GetLastError() << std::endl;
        return false;
    }

    PROCESSENTRY32W processEntry{};
    processEntry.dwSize = sizeof(PROCESSENTRY32W);
    
    if (!Process32FirstW(snapshot, &processEntry)) {
        std::cout << "Failed to retrieve process information: " << GetLastError() << std::endl;
        return false;
    }

    do {
        if (wcscmp(processEntry.szExeFile, processName) == 0) {
            return true;
        }
    } while (Process32NextW(snapshot, &processEntry));

    return false;
}

bool TerminateProcessByName(const wchar_t* processName) {
    ProcessHandle snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create process snapshot: " << GetLastError() << std::endl;
        return false;
    }

    PROCESSENTRY32W processEntry{};
    processEntry.dwSize = sizeof(PROCESSENTRY32W);
    
    if (!Process32FirstW(snapshot, &processEntry)) {
        std::cout << "Failed to retrieve process information: " << GetLastError() << std::endl;
        return false;
    }

    do {
        if (wcscmp(processEntry.szExeFile, processName) == 0) {
            ProcessHandle processHandle(OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID));
            if (processHandle == nullptr) {
                std::cout << "Failed to open process: " << GetLastError() << std::endl;
                return false;
            }

            if (!TerminateProcess(processHandle, 0)) {
                std::cout << "Failed to terminate process: " << GetLastError() << std::endl;
                return false;
            }

            return true;
        }
    } while (Process32NextW(snapshot, &processEntry));

    std::cout << "Process not found: " << ConvertToStdString(ConvertCharToBSTR(reinterpret_cast<const char*>(processName))) << std::endl;
    return false;
}

#pragma endregion

#pragma region Helper Functions

BSTR ConvertCharToBSTR(const char* charString) {
    if (!charString) return nullptr;
    const int size = MultiByteToWideChar(CP_UTF8, 0, charString, -1, nullptr, 0);
    BSTR bstr = SysAllocStringLen(nullptr, size - 1);
    MultiByteToWideChar(CP_UTF8, 0, charString, -1, bstr, size);
    return bstr;
}

std::string ConvertToStdString(BSTR bstr) {
    if (!bstr) return "";
    const int size = WideCharToMultiByte(CP_UTF8, 0, bstr, -1, nullptr, 0, nullptr, nullptr);
    std::string result(size - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, bstr, -1, &result[0], size, nullptr, nullptr);
    return result;
}

BSTR FormatErrorMessage(const char* context, HRESULT status, ULONG returnCode) {
    std::string errorMsg = "An error occurred at " + std::string(context) + ": ";
    char statusBuffer[32];
    snprintf(statusBuffer, sizeof(statusBuffer), "0x%08X", static_cast<unsigned int>(status));
    errorMsg += statusBuffer;
    
    if (returnCode != 0) {
        char returnCodeBuffer[32];
        snprintf(returnCodeBuffer, sizeof(returnCodeBuffer), " %lu", returnCode);
        errorMsg += returnCodeBuffer;
    }

    return ConvertCharToBSTR(errorMsg.c_str());
}

#pragma region Windows XP
static BSTR XP_GetWPALeft() {
    if (!XP_LoadLicenseManager()) {
        return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
    }

    ULONG dwWPALeft = 0, dwEvalLeft = 0;
    const HRESULT status = XP_LicenseAgent->GetExpirationInfo(&dwWPALeft, &dwEvalLeft);
    if (FAILED(status)) {
        XP_LicenseAgent->Release();
        XP_LicenseAgent = nullptr;
        return FormatErrorMessage("GetExpirationInfo", status);
    }
    if (dwWPALeft == 0x7FFFFFFF) {
        return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
    }
    wchar_t buffer[16];
    swprintf(buffer, L"%lu", dwWPALeft);
    return SysAllocString(buffer);
}

static BSTR XP_GetEvalLeft() {
    if (!XP_LoadLicenseManager()) {
        return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
    }

    ULONG dwWPALeft = 0, dwEvalLeft = 0;
    const HRESULT status = XP_LicenseAgent->GetExpirationInfo(&dwWPALeft, &dwEvalLeft);
    if (FAILED(status)) {
        XP_LicenseAgent->Release();
        XP_LicenseAgent = nullptr;
        return FormatErrorMessage("GetExpirationInfo", status);
    }
    if (dwEvalLeft == 0x7FFFFFFF) {
        return SysAllocString(L"An error occurred at GetEvalLeft: Not an evaluation copy");
    }
    wchar_t buffer[16];
    swprintf(buffer, L"%lu", dwEvalLeft);
    return SysAllocString(buffer);
}

#pragma region Windows XP Activation

enum class VerificationResult {
    Success,
    InvalidFormat,
    VerificationFailed,
    AlreadyActivated,
    LoadError
};

static VerificationResult VerifyConfirmationIDChunk(const std::string& chunk) {
    if (chunk.length() != 6) {
        return VerificationResult::InvalidFormat;
    }

    LONG pbValue;
    const HRESULT status = XP_LicenseAgent->VerifyCheckDigits(ConvertCharToBSTR(chunk.c_str()), &pbValue);
    return (SUCCEEDED(status) && pbValue) ? VerificationResult::Success : VerificationResult::VerificationFailed;
}

static BSTR XP_VerifyCheckDigits(BSTR cidChunk) {
    if (!XP_LoadLicenseManager()) {
        return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
    }

    const std::string activationCheck = ConvertToStdString(XP_GetWPALeft());
    if (activationCheck == "An error occurred at GetWPALeft: Windows is activated") {
        return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
    }

    LONG pbValue;
    const HRESULT status = XP_LicenseAgent->VerifyCheckDigits(cidChunk, &pbValue);
    return SUCCEEDED(status) && pbValue 
        ? SysAllocString(L"Successfully verified CID chunk")
        : FormatErrorMessage("XP_VerifyCheckDigits", status);
}

static BSTR XP_SetConfirmationID(BSTR confirmationID) {
    if (!XP_LoadLicenseManager()) { 
        return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
    }

    const std::string activationCheck = ConvertToStdString(XP_GetWPALeft());
    if (activationCheck == "An error occurred at GetWPALeft: Windows is activated") {
        return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
    }

    // Convert and clean up the confirmation ID
    std::string cidStr = ConvertToStdString(confirmationID);
    cidStr.erase(std::remove(cidStr.begin(), cidStr.end(), '-'), cidStr.end());

    // Verify each 6-character chunk
    for (size_t i = 0; i < cidStr.length(); i += 6) {
        const std::string chunk = cidStr.substr(i, 6);
        const auto result = VerifyConfirmationIDChunk(chunk);
        
        switch (result) {
            case VerificationResult::InvalidFormat:
                return SysAllocString(L"An error occurred: Invalid confirmation ID format");
            case VerificationResult::VerificationFailed:
                return SysAllocString(L"An error occurred: Confirmation ID verification failed");
            case VerificationResult::Success:
                continue;
            default:
                return SysAllocString(L"An error occurred: Unexpected verification result");
        }
    }

    // Deposit the full confirmation ID
    ULONG dwRetCode;
    const HRESULT status = XP_LicenseAgent->DepositConfirmationId(confirmationID, &dwRetCode);
    if (FAILED(status) || dwRetCode) {
        return FormatErrorMessage("DepositConfirmationId", status, dwRetCode);
    }

    // Handle WPA notifier process
    if (IsProcessRunning(L"wpabaln.exe")) {
        if (!TerminateProcessByName(L"wpabaln.exe")) {
            std::cout << "Warning: Failed to terminate WPA notifier process" << std::endl;
        }
    }

    // Clean up activation files
    HMODULE hMod = LoadLibraryW(L"setupapi.dll");
    if (hMod) {
        using InstallHinfSectionFunc = BOOL (WINAPI*)(HWND, HINSTANCE, PCWSTR, INT);
        if (auto installHinfSection = reinterpret_cast<InstallHinfSectionFunc>(GetProcAddress(hMod, "InstallHinfSectionW"))) {
            SetLastError(0);
            const BOOL result = installHinfSection(nullptr, nullptr, L"DEL_OOBE_ACTIVATE 132 syssetup.inf", 132);
            const DWORD error = GetLastError();
            
            if (!result && error != 0 && error != 6) {  // Ignore error 6 (ERROR_INVALID_HANDLE) as it indicates success
                std::cout << "Warning: Failed to remove activation reminders. Error: " << error << std::endl;
				std::cout << "You can try to run the following command yourself: " << std::endl;
				std::cout << "rundll32 setupapi,InstallHinfSection DEL_OOBE_ACTIVATE 132 syssetup.inf" << std::endl;
            }
        }
        FreeLibrary(hMod);
    }

    return SysAllocString(L"Successfully set confirmation ID");
}

static BSTR XP_GetInstallationID() {
    if (!XP_LoadLicenseManager()) {
        return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
    }
    if (0 == wcscmp(XP_GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
        return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
    }

    BSTR installationID = nullptr;
    const HRESULT status = XP_LicenseAgent->GenerateInstallationId(&installationID);
    if (FAILED(status) || !installationID) {
        return FormatErrorMessage("GenerateInstallationId", status);
    }
    return installationID;
}

static BSTR XP_SetProductKey(LPWSTR productKey) {
    if (!XP_LoadLicenseManager()) {
        return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
    }

    if (0 == wcscmp(XP_GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
        return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
    }

    // Validate product key format
    std::wstring ws(productKey);
    const std::string productKeyToRegex = std::string(ws.begin(), ws.end());
    const std::regex pattern("[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}");

    if (!std::regex_match(productKeyToRegex, pattern)) {
        return SysAllocString(L"An error occurred at regex_match: Product key is invalid");
    }

    const HRESULT status = XP_LicenseAgent->SetProductKey(productKey);
    if (FAILED(status)) {
        return FormatErrorMessage("SetProductKey", status);
    }
    return SysAllocString(L"Successfully set product key");
}

static BSTR XP_GetProductID() {
    if (!XP_LoadLicenseManager()) {
        return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
    }

    BSTR productID = nullptr;
    const HRESULT status = XP_LicenseAgent->GetProductID(&productID);
    if (FAILED(status)) {
        return FormatErrorMessage("GetProductID", status);
    }
    return productID;
}
#pragma endregion

#pragma region Command Line Handling

struct CommandLineArgs {
    static const char* getCmdOption(char** begin, char** end, const std::string& option) {
        char** itr = std::find(begin, end, option);
        if (itr != end && ++itr != end) {
            return *itr;
        }
        return nullptr;
    }

    static bool cmdOptionExists(char** begin, char** end, const std::string& option) {
        return std::find(begin, end, option) != end;
    }

    static std::string readFromStdin() {
        std::string input;
        if (!std::cin.eof() && std::getline(std::cin, input)) {
            input.erase(std::find_if(input.rbegin(), input.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), input.end());
            return input;
        }
        return "";
    }
};

#pragma endregion

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
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

    // Handle commands
    if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/dti")) {
        std::cout << ConvertToStdString(XP_GetInstallationID());
    }
    else if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/atp")) {
        const char* confirmationId = CommandLineArgs::getCmdOption(argv, argv + argc, "/atp");
        if (!confirmationId) {
            std::string pipedInput = CommandLineArgs::readFromStdin();
            if (!pipedInput.empty()) {
                std::cout << ConvertToStdString(XP_SetConfirmationID(ConvertCharToBSTR(pipedInput.c_str())));
            } else {
                std::cout << "An error occurred: No confirmation ID provided via argument or pipe\n\n" << USAGE_TEXT;
                return 7;
            }
        } else {
            std::cout << ConvertToStdString(XP_SetConfirmationID(ConvertCharToBSTR(confirmationId)));
        }
    }
    else if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/xpr")) {
        std::cout << ConvertToStdString(XP_GetWPALeft());
    }
    else if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/xpr-eval")) {
        std::cout << ConvertToStdString(XP_GetEvalLeft());
    }
    else if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/ipk")) {
        const char* productKey = CommandLineArgs::getCmdOption(argv, argv + argc, "/ipk");
        if (!productKey) {
            std::string pipedInput = CommandLineArgs::readFromStdin();
            if (!pipedInput.empty()) {
                std::cout << ConvertToStdString(XP_SetProductKey(convertCharArrayToLPCWSTR(pipedInput.c_str())));
            } else {
                std::cout << "An error occurred: No product key provided via argument or pipe\n\n" << USAGE_TEXT;
                return 7;
            }
        } else {
            std::cout << ConvertToStdString(XP_SetProductKey(convertCharArrayToLPCWSTR(productKey)));
        }
    }
    else if (CommandLineArgs::cmdOptionExists(argv, argv + argc, "/dli")) {
        std::cout << ConvertToStdString(XP_GetProductID());
    }
    else {
        std::cout << "An error occurred: No arguments listed\n\n" << USAGE_TEXT;
        return 7;
    }

    return 0;
}
#pragma clang diagnostic pop
