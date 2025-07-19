// ReSharper disable CppEnforceOverridingFunctionStyle
typedef struct IUnknown IUnknown;

#include <windows.h>
#include <iostream>
#include <comdef.h>
#include <regex>
#include <TlHelp32.h>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

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

static BOOL XP_LoadLicenseManager()
{
	if (!XP_ComInitialized) {
		const HRESULT status = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		if (FAILED(status)) {
            const char* errorString = "An error occurred at CoInitializeEx:";
            const int bufferSize = snprintf(nullptr, 0, "%s 0x%08X\n", errorString, static_cast<unsigned int>(status));
            char* result = new char[bufferSize + 1];
            snprintf(result, bufferSize + 1, "%s 0x%08X\n", errorString, static_cast<unsigned int>(status));
			std::cout << result;
			return FALSE;
		}
		XP_ComInitialized = TRUE;
	}
	if (!XP_LicenseAgent) {
		HRESULT status = CoCreateInstance(XP_CLSID, nullptr, CLSCTX_INPROC_SERVER, XP_IID, reinterpret_cast<void **>(&XP_LicenseAgent));
		int good = 0;
		if (SUCCEEDED(status)) {
			ULONG dwRetCode;
			status = XP_LicenseAgent->Initialize(0xC475 /* This needs to be changed, I believe it's causing the crashing.*/ , 3, nullptr, &dwRetCode);
			if (SUCCEEDED(status) && dwRetCode == 0) {
				good = 1;
			}
			else {
				XP_LicenseAgent->Release();
				XP_LicenseAgent = nullptr;
			}
		}
		if (!good) {
            const char* errorString = "An error occurred at CoCreateInstance:";
            const int bufferSize = snprintf(nullptr, 0, "%s 0x%08X\n", errorString, static_cast<unsigned int>(status));
            char* result = new char[bufferSize + 1];
            snprintf(result, bufferSize + 1, "%s 0x%08X\n", errorString, static_cast<unsigned int>(status));
            std::cout << result;
			return FALSE;
		}
	}
	return TRUE;
}

#pragma region Internals

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

BSTR ConvertToBSTR(const std::string& str) {
	const int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	// ReSharper disable once CppLocalVariableMayBeConst
	BSTR bstr = SysAllocStringLen(nullptr, size - 1);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, bstr, size);
    return bstr;
}

std::string ConvertToStdString(BSTR bstr) {
    const int size = WideCharToMultiByte(CP_UTF8, 0, bstr, -1, nullptr, 0, nullptr, nullptr);
    const auto buffer = new char[size];
    WideCharToMultiByte(CP_UTF8, 0, bstr, -1, buffer, size, nullptr, nullptr);

    std::string result(buffer);
    delete[] buffer;

    return result;
}

BSTR ConvertCharToBSTR(const char* charString) {
    const int size = MultiByteToWideChar(CP_UTF8, 0, charString, -1, nullptr, 0);
    // ReSharper disable once CppLocalVariableMayBeConst
    BSTR bstr = SysAllocStringLen(nullptr, size - 1);
    MultiByteToWideChar(CP_UTF8, 0, charString, -1, bstr, size);
    return bstr;
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

bool IsProcessRunning(const wchar_t* processName)
{
	// ReSharper disable once CppLocalVariableMayBeConst
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
	{
		std::cout << "An error occurred at IsProcessRunning: Failed to create process snapshot." << std::endl;
		return false;
	}

	PROCESSENTRY32W processEntry{};
	processEntry.dwSize = sizeof(PROCESSENTRY32W);
	if (!Process32FirstW(snapshot, &processEntry))
	{
		CloseHandle(snapshot);
		std::cout << "An error occurred at IsProcessRunning: Failed to retrieve process information." << std::endl;
		return false;
	}

	while (Process32NextW(snapshot, &processEntry))
	{
		if (wcscmp(processEntry.szExeFile, processName) == 0)
		{
			CloseHandle(snapshot);
			return true;  // Process found
		}
	}

	CloseHandle(snapshot);
	return false;  // Process not found
}

bool TerminateProcessByName(const wchar_t* processName)
{
	// ReSharper disable once CppLocalVariableMayBeConst
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	PROCESSENTRY32W processEntry{};
	processEntry.dwSize = sizeof(PROCESSENTRY32W);
	if (!Process32FirstW(snapshot, &processEntry))
	{
		CloseHandle(snapshot);
		return false;
	}

	while (Process32NextW(snapshot, &processEntry))
	{
		if (wcscmp(processEntry.szExeFile, processName) == 0)
		{
			// ReSharper disable once CppLocalVariableMayBeConst
			HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID);
			if (processHandle != nullptr)
			{
				if (TerminateProcess(processHandle, 0))
				{
					CloseHandle(processHandle);
					CloseHandle(snapshot);
					return true;  // Process terminated successfully
				}
				else
				{
					CloseHandle(processHandle);
					CloseHandle(snapshot);
					return false;  // Failed to terminate the process
				}
			}
			else
			{
				CloseHandle(snapshot);
				return false;  // Failed to open process handle
			}
		}
	}

	CloseHandle(snapshot);
	return false;  // Process not found
}

#pragma endregion

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
        const char* errorString = "An error occurred at CoInitializeEx:";
		const int bufferSize = snprintf(nullptr, 0, "%s 0x%08X", errorString, static_cast<unsigned int>(status));
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%08X", errorString, static_cast<unsigned int>(status));
		const int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        auto* wideResult = new OLECHAR[wideCharSize];
        MultiByteToWideChar(CP_UTF8, 0, result, -1, wideResult, wideCharSize);
		return SysAllocString(wideResult);
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
        const char* errorString = "An error occurred at GetExpirationInfo:";
		const int bufferSize = snprintf(nullptr, 0, "%s 0x%08X", errorString, static_cast<unsigned int>(status));
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%08X", errorString, static_cast<unsigned int>(status));
		const int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        auto* wideResult = new OLECHAR[wideCharSize];
        MultiByteToWideChar(CP_UTF8, 0, result, -1, wideResult, wideCharSize);
        return SysAllocString(wideResult);
	}
	if (dwEvalLeft == 0x7FFFFFFF) {
		return SysAllocString(L"An error occurred at GetEvalLeft: Not an evaluation copy");
	}
	wchar_t buffer[16];
	swprintf(buffer, L"%lu", dwWPALeft);
	return SysAllocString(buffer);
}

static BSTR XP_VerifyCheckDigits(BSTR cidChunk) {
	if (!XP_LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
	}

	if (0 == wcscmp(XP_GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}

	LONG pbValue;
	const HRESULT status = XP_LicenseAgent->VerifyCheckDigits(cidChunk, &pbValue);
	if (FAILED(status) || !pbValue) {
        char errorMessage[70] = "An error occurred at XP_VerifyCheckDigits:";
        char pbValueChar[20];
        snprintf(errorMessage, sizeof(errorMessage), "%ld", pbValue);
        safeStrncat(errorMessage, pbValueChar, sizeof(errorMessage));
        const int len = MultiByteToWideChar(CP_UTF8, 0, errorMessage, -1, nullptr, 0);
        auto* oleCharString = new OLECHAR[len];
        MultiByteToWideChar(CP_UTF8, 0, errorMessage, -1, oleCharString, len);
		return SysAllocString(oleCharString);
	}
	return SysAllocString(L"Successfully verified CID chunk");
}

static BSTR XP_SetConfirmationID(BSTR confirmationID) {
	if (!XP_LoadLicenseManager()) { 
		return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
	}

	if (0 == wcscmp(XP_GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}

	const int length = static_cast<int>(SysStringLen(confirmationID));
	char* str = new char[length + 1];
	WideCharToMultiByte(CP_UTF8, 0, confirmationID, length, str, length, nullptr, nullptr);
	str[length] = '\0';

	std::string inputString(str);
	inputString.erase(std::remove(inputString.begin(), inputString.end(), '-'), inputString.end()); // remove -'s
	for (size_t i = 0; i < inputString.length(); i += 6) {
		std::string substring = inputString.substr(i, 6);
		const char* cstr = substring.c_str();
		if (0 != wcscmp(XP_VerifyCheckDigits(ConvertCharToBSTR(cstr)), L"Successfully verified CID chunk")) {
			return SysAllocString(L"An error occurred at XP_VerifyCheckDigits: Check for misspelling");
		}
	}

	ULONG dwRetCode;
	const HRESULT status = XP_LicenseAgent->DepositConfirmationId(confirmationID, &dwRetCode);
	if (FAILED(status) || dwRetCode) {
        const char* errorString = "An error occurred at DepositConfirmationId:";
        const int bufferSize = snprintf(nullptr, 0, "%s 0x%08X %lu", errorString, static_cast<unsigned int>(status), dwRetCode);
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%08X %lu", errorString, static_cast<unsigned int>(status), dwRetCode);

        const int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        auto* wideResult = new OLECHAR[wideCharSize];
        MultiByteToWideChar(CP_UTF8, 0, result, -1, wideResult, wideCharSize);
        return SysAllocString(wideResult);
	}

	system("rundll32 setupapi,InstallHinfSection DEL_OOBE_ACTIVATE 132 syssetup.inf"); // remove activation shortcuts

	if (IsProcessRunning(L"wpabaln.exe")) { // end WPA notifier process if there
		TerminateProcessByName(L"wpabaln.exe");
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
        const char* errorString = "An error occurred at GenerateInstallationId:";
        const int bufferSize = snprintf(nullptr, 0, "%s 0x%08X", errorString, static_cast<unsigned int>(status));
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%08X", errorString, static_cast<unsigned int>(status));
        const int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        auto* wideResult = new OLECHAR[wideCharSize];
        MultiByteToWideChar(CP_UTF8, 0, result, -1, wideResult, wideCharSize);
        return SysAllocString(wideResult);
	}
	else {
		return installationID;
	}
}

static BSTR XP_SetProductKey(LPWSTR productKey) {
	if (!XP_LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
	}

	std::wstring ws(productKey);
	const std::string productKeyToRegex = std::string(ws.begin(), ws.end());
	const std::regex pattern("[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}");

	if (!std::regex_match(productKeyToRegex, pattern)) {
		return SysAllocString(L"An error occurred at regex_match: Product key is invalid");
	}

	if (0 == wcscmp(XP_GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}

	const HRESULT status = XP_LicenseAgent->SetProductKey(productKey);
	if (FAILED(status)) {
        const char* errorString = "An error occurred at SetProductKey:";
        const int bufferSize = snprintf(nullptr, 0, "%s 0x%08X", errorString, static_cast<unsigned int>(status));
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%08X", errorString, static_cast<unsigned int>(status));
        const int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        auto* wideResult = new OLECHAR[wideCharSize];
        MultiByteToWideChar(CP_UTF8, 0, result, -1, wideResult, wideCharSize);
        return SysAllocString(wideResult);
	}
	else {
		return SysAllocString(L"Successfully set product key");
	}
}

static BSTR XP_GetProductID() {
	if (!XP_LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
	}

	BSTR productID = nullptr;

	const HRESULT status = XP_LicenseAgent->GetProductID(&productID);
	if (FAILED(status)) {
        const char* errorString = "An error occurred at GetProductID:";
        const int bufferSize = snprintf(nullptr, 0, "%s 0x%08X", errorString, static_cast<unsigned int>(status));
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%08X", errorString, static_cast<unsigned int>(status));
        const int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        auto* wideResult = new OLECHAR[wideCharSize];
        MultiByteToWideChar(CP_UTF8, 0, result, -1, wideResult, wideCharSize);
        return SysAllocString(wideResult);
	}
	else {
		return productID;
	}
}
#pragma endregion

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
int main(int argc, char* argv[])
{
	const char* text =
		"xpmgr - Windows XP License Manager (compiled on " __DATE__ " " __TIME__ ")\n"
		"\n"
		"Usage: \n"
		"/dti: Gets the Installation ID\n"
		"/atp [cid]: Sets Confirmation ID (If successful, activates Windows/Office)\n"
		"/xpr: Gets the days before activation is required (Grace period)\n"
		"/xpr-eval: Gets the days before the evaluation period expires (Eval. copies only)\n"
		"/ipk [pkey]: Sets/changes product key\n"
		"/dli: Gets the product ID of Windows\n"
		"/?: Displays this message";

	if (cmdOptionExists(argv, argv + argc, "--GetUsage")) {
		std::cout << text;
		return 0;
	}

	specifiedProduct = "WindowsNT5x";

	if (std::strcmp(specifiedProduct, "WindowsNT5x") == 0) {
		SYSTEM_INFO systemInfo;
		GetNativeSystemInfo(&systemInfo);
#ifdef ENVIRONMENT32
		if (systemInfo.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_INTEL) { // running under WOW64
			if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) { // is AMD64
				std::cout << "An error occurred at systemInfo: Incorrect version of xpmgr. You need to download the x64 version.";
		}
			else if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) { // is IA64
				std::cout << "An error occurred at systemInfo: Windows Product Activation does not exist on this platform.";
			}
			else { // is PPC, megafart 386, whatever else
				std::cout << "An error occurred at systemInfo: Incorrect version of xpmgr. Send an issue at https://github.com/UMSKT/xpmgr/issues if you want to help us make a version for your platform!";
			}
			return 1;
	}
#endif
		OSVERSIONINFOEX info = {};
		info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		GetVersionEx(reinterpret_cast<LPOSVERSIONINFO>(&info));

		if (info.dwMajorVersion != 5) {
			std::cout << "An error occurred at OSVERSIONINFOEX: Windows management only works on Windows NT 5.1 and 5.2.";
			if (info.dwMajorVersion > 5) {
				std::cout << " Use slmgr instead: https://learn.microsoft.com/en-us/windows-server/get-started/activation-slmgr-vbs-options";
				return 3;
			}
			return 2;
		}
		else {
			if (info.dwMinorVersion != 1 && info.dwMinorVersion != 2) {
				std::cout << "An error occurred at OSVERSIONINFOEX: Windows management only works on Windows NT 5.1 and 5.2.";
				if (info.dwMinorVersion == 0) {
					std::cout << " You should be fine anyways, since Windows 2000 doesn't use Product Activation.";
					return 4;
				}
				return 5;
			}
		}
	}

	if (cmdOptionExists(argv, argv + argc, "/dti")) {
		std::cout << ConvertToStdString(XP_GetInstallationID());
		return 0;
	}
	else if (cmdOptionExists(argv, argv + argc, "/atp")) {
		std::cout << ConvertToStdString(XP_SetConfirmationID(ConvertCharToBSTR(getCmdOption(argv, argv + argc, "--SetConfirmationID"))));
		return 0;
	}
	else if (cmdOptionExists(argv, argv + argc, "/xpr")) {
		std::cout << ConvertToStdString(XP_GetWPALeft());
		return 0;
	}
	else if (cmdOptionExists(argv, argv + argc, "/xpr-eval")) {
		std::cout << ConvertToStdString(XP_GetEvalLeft());
		return 0;
	}

	else if (cmdOptionExists(argv, argv + argc, "/ipk")) {
		std::cout << ConvertToStdString(XP_SetProductKey(convertCharArrayToLPCWSTR(getCmdOption(argv, argv + argc, "--SetProductKey"))));
		return 0;
	}
	else if (cmdOptionExists(argv, argv + argc, "/dli")) {
		std::cout << ConvertToStdString(XP_GetProductID());
		return 0;
	}
	else {
		std::cout << "An error occurred at main: No arguments listed\n\n";
		std::cout << text;
		return 7;
	}
}
#pragma clang diagnostic pop
