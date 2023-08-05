#include <windows.h>
#include <iostream>
#include <comutil.h>
#include <regex>
#include <TlHelp32.h>

#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "kernel32.lib")

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Internal stuff

wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
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
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

char* BstrToChar(BSTR bstr)
{
	int len = ::SysStringLen(bstr);
	int bufSize = ::WideCharToMultiByte(CP_UTF8, 0, bstr, len, NULL, 0, NULL, NULL);
	char* buffer = new char[bufSize + 1];
	::WideCharToMultiByte(CP_UTF8, 0, bstr, len, buffer, bufSize, NULL, NULL);
	buffer[bufSize] = '\0';
	return buffer;
}

ULONG ConvertToULONG(const char* str)
{
	char* end;
	ULONG value = std::strtoul(str, &end, 10);
	return value;
}

OLECHAR SizeToOLECHAR(size_t value)
{
	// Convert size_t to wstring
	std::wstring wideString = std::to_wstring(value);

	// Retrieve the first character from the wide string
	OLECHAR oChar = wideString[0];

	return oChar;
}

bool IsProcessRunning(const wchar_t* processName)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Failed to create process snapshot." << std::endl;
		return false;
	}

	PROCESSENTRY32W processEntry{};
	processEntry.dwSize = sizeof(PROCESSENTRY32W);
	if (!Process32FirstW(snapshot, &processEntry))
	{
		CloseHandle(snapshot);
		std::cerr << "Failed to retrieve process information." << std::endl;
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
			HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID);
			if (processHandle != NULL)
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

typedef struct IUnknown IUnknown;

// The magic numbers, interface, and LoadLicenseManager() function was made by diamondggg on MyDigitalLife.
// LoadLicenseManager() was modified to suit a CLI rather than a GUI.
// Everything else was either made by TheTank20 or copy pasted from StackOverflow.
static CLSID xp_licdllCLSID = { 0xACADF079, 0xCBCD, 0x4032, {0x83, 0xF2, 0xFA, 0x47, 0xC4, 0xDB, 0x09, 0x6F} };
static IID xp_licenseAgentIID = { 0xB8CBAD79, 0x3F1F, 0x481A, {0xBB, 0x0C, 0xE7, 0xBB, 0xD7, 0x7B, 0xDD, 0xD1} };
#undef XP_INTERFACE
#define XP_INTERFACE ICOMLicenseAgent
DECLARE_INTERFACE_(ICOMLicenseAgent, IDispatch)
{
	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

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
	STDMETHOD(AsyncProcessNewLicenseRequest)(THIS) PURE;
	STDMETHOD(AsyncProcessReissueLicenseRequest)(THIS) PURE;
	STDMETHOD(AsyncProcessReviseCustInfoRequest)(THIS) PURE;
	STDMETHOD(GetAsyncProcessReturnCode)(THIS_ ULONG * pdwRetCode) PURE;
	STDMETHOD(AsyncProcessDroppedLicenseRequest)(THIS) PURE; 
	STDMETHOD(GenerateInstallationId)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(DepositConfirmationId)(THIS_ BSTR bstrVal, ULONG * pdwRetCode) PURE;
	STDMETHOD(GetExpirationInfo)(THIS_ ULONG * pdwWPALeft, ULONG * pdwEvalLeft) PURE;
	STDMETHOD(AsyncProcessRegistrationRequest)(THIS) PURE;
	STDMETHOD(ProcessHandshakeRequest)(THIS_ LONG bReviseCustInfo) PURE;
	STDMETHOD(ProcessNewLicenseRequest)(THIS) PURE;
	STDMETHOD(ProcessDroppedLicenseRequest)(THIS) PURE;
	STDMETHOD(ProcessReissueLicenseRequest)(THIS) PURE;
	STDMETHOD(ProcessReviseCustInfoRequest)(THIS) PURE;
	STDMETHOD(EnsureInternetConnection)(THIS) PURE;
	STDMETHOD(SetProductKey)(THIS_ LPWSTR pszNewProductKey) PURE;
	STDMETHOD(GetProductID)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(VerifyCheckDigits)(THIS_ BSTR bstrCIDIID, LONG * pbValue) PURE;
};

static BOOL XP_ComInitialized = FALSE;
static ICOMLicenseAgent* XP_LicenseAgent = NULL;

static BOOL LoadLicenseManager()
{
	if (!XP_ComInitialized) {
		HRESULT status = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (FAILED(status)) {
			std::cout << "An error occurred at CoInitializeEx: ", status;
			return FALSE;
		}
		XP_ComInitialized = TRUE;
	}
	if (!XP_LicenseAgent) {
		HRESULT status = CoCreateInstance(xp_licdllCLSID, NULL, CLSCTX_INPROC_SERVER, xp_licenseAgentIID, (void**)&XP_LicenseAgent);
		int good = 0;
		if (SUCCEEDED(status)) {
			ULONG dwRetCode;
			status = XP_LicenseAgent->Initialize(0xC475, 3, 0, &dwRetCode);
			if (SUCCEEDED(status) && dwRetCode == 0) {
				good = 1;
			}
			else {
				XP_LicenseAgent->Release();
				XP_LicenseAgent = NULL;
			}
		}
		if (!good) {
			std::cout << "An error occurred at CoCreateInstance: " + status;
			return FALSE;
		}
	}
	return TRUE;
}

static BSTR GetWPALeft() {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
	}

	ULONG dwWPALeft = 0, dwEvalLeft = 0;
	HRESULT status = XP_LicenseAgent->GetExpirationInfo(&dwWPALeft, &dwEvalLeft);
	if (FAILED(status)) {
		XP_LicenseAgent->Release();
		XP_LicenseAgent = NULL;
		return SysAllocString(L"An error occurred at GetExpirationInfo: " + status);
	}
	if (dwWPALeft == 0x7FFFFFFF) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}
	return SysAllocString(_bstr_t(dwWPALeft).Detach());
}

static BSTR GetEvalLeft() {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
	}

	ULONG dwWPALeft = 0, dwEvalLeft = 0;
	HRESULT status = XP_LicenseAgent->GetExpirationInfo(&dwWPALeft, &dwEvalLeft);
	if (FAILED(status)) {
		XP_LicenseAgent->Release();
		XP_LicenseAgent = NULL;
		return SysAllocString(L"An error occurred at GetExpirationInfo: " + status);
	}
	if (dwEvalLeft == 0x7FFFFFFF) {
		return SysAllocString(L"An error occurred at GetEvalLeft: Not an evaluation copy");
	}
	return SysAllocString(_bstr_t(dwEvalLeft).Detach());
}

static BSTR VerifyCheckDigits(BSTR cidChunk) {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
	}

	if (0 == wcscmp(GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}

	LONG pbValue;
	HRESULT status = XP_LicenseAgent->VerifyCheckDigits(cidChunk, &pbValue);
	if (FAILED(status) || !pbValue) {
		return SysAllocString(L"An error occurred at VerifyCheckDigits: " + pbValue);
	}
	return SysAllocString(L"Successfully verified CID chunk");
}

static BSTR SetConfirmationID(BSTR confirmationID) {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
	}

	if (0 == wcscmp(GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}

	// 
	int length = SysStringLen(confirmationID);
	char* str = new char[length + 1];
	WideCharToMultiByte(CP_UTF8, 0, confirmationID, length, str, length, NULL, NULL);
	str[length] = '\0';

	std::string inputString(str);
	inputString.erase(std::remove(inputString.begin(), inputString.end(), '-'), inputString.end()); // remove -'s
	for (size_t i = 0; i < inputString.length(); i += 6) {
		std::string substring = inputString.substr(i, 6);
		const char* cstr = substring.c_str();
		if (0 != wcscmp(VerifyCheckDigits(_com_util::ConvertStringToBSTR(cstr)), L"Successfully verified CID chunk")) {
			return SysAllocString(L"An error occurred at VerifyCheckDigits: Check for misspelling");
		}

		free(&cstr);
		free(&substring);
	}

	free(&inputString);

	ULONG dwRetCode;
	HRESULT status = XP_LicenseAgent->DepositConfirmationId(confirmationID, &dwRetCode);
	if (FAILED(status) || dwRetCode) {
		return SysAllocString(L"An error occurred at DepositConfirmationId: " + status + dwRetCode);
	}

	system("rundll32 setupapi,InstallHinfSection DEL_OOBE_ACTIVATE 132 syssetup.inf"); // remove activation shortcuts

	if (IsProcessRunning(L"wpabaln.exe")) { // end WPA notifier process if there
		TerminateProcessByName(L"wpabaln.exe");
	}

	return SysAllocString(L"Successfully set confirmation ID");
}

static BSTR GetInstallationID() {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
	}

	if (0 == wcscmp(GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}

	BSTR installationID = NULL;
	HRESULT status = XP_LicenseAgent->GenerateInstallationId(&installationID);
	if (FAILED(status) || !installationID) {
		return SysAllocString(L"An error occurred at GenerateInstallationId: " + status);
	}
	else {
		return installationID;
	}
}

static BSTR SetProductKey(LPWSTR productKey) {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
	}

	std::wstring ws(productKey);
	std::string productKeyToRegex = std::string(ws.begin(), ws.end());
	std::regex pattern("[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}");

	if (!std::regex_match(productKeyToRegex, pattern)) {
		return SysAllocString(L"An error occurred at regex_match: Product key is invalid");
	}

	if (0 == wcscmp(GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}

	HRESULT status = XP_LicenseAgent->SetProductKey(productKey);
	if (FAILED(status)) {
		return SysAllocString(L"An error occurred at SetProductKey: " + status);
	}
	else {
		return SysAllocString(L"Successfully set product key");
	}
}

static BSTR GetProductID() {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
	}

	BSTR productID = NULL;

	HRESULT status = XP_LicenseAgent->GetProductID(&productID);
	if (FAILED(status)) {
		return SysAllocString(L"An error occurred at GetProductID: " + status);
	}
	else {
		return productID;
	}
}

int main(int argc, char* argv[])
{
	const char* text =
		"Usage: \n"
		"--GetInstallationID: Gets the Installation ID\n"
		"--SetConfirmationID [cid]: Sets Confirmation ID (If successful, activates Windows)\n"
		"--GetWPALeft: Gets the days before activation is required (Grace period)\n"
		"--GetEvalLeft: Gets the days before the evaluation period expires (Eval. copies only)\n"
		"--SetProductKey [pkey]: Sets/changes product key\n"
		"--GetProductID: Gets the product ID of Windows\n"
		"--GetUsage: Displays this message";

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
			std::cout << "An error occurred at systemInfo: Incorrect version of xpmgr. Go to https://umskt.zulipchat.com if you want to help us make a version for your platform!";
		}
		return 0;
	}
#endif

	OSVERSIONINFOEX info;
	ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((LPOSVERSIONINFO)&info);

	if (info.dwMajorVersion != 5) {
		std::cout << "An error occurred at OSVERSIONINFOEX: This tool only works on Windows NT 5.1 and 5.2.";
		return 0;
	}
	else {
		if (info.dwMinorVersion != 1 && info.dwMinorVersion != 2) {
			std::cout << "An error occurred OSVERSIONINFOEX: This tool only works on Windows NT 5.1 and 5.2.";
			return 0;
		}
	}
	free(&info);

	// For some reason, anything Microsoft Registration related just doesn't work
	//if (cmdOptionExists(argv, argv + argc, "--GetFirstName")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetFirstName());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetFirstName")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetFirstName(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetFirstName"))));
	//	return 0;
	//}
	//
	//else if (cmdOptionExists(argv, argv + argc, "--GetLastName")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetLastName());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetLastName")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetLastName(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetLastName"))));
	//	return 0;
	//}
	//
	//else if (cmdOptionExists(argv, argv + argc, "--GetOrgName")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetOrgName());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetOrgName")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetOrgName(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetOrgName"))));
	//	return 0;
	//}
	//
	//else if (cmdOptionExists(argv, argv + argc, "--GetEmail")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetEmail());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetEmail")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetEmail(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetEmail"))));
	//	return 0;
	//}
	//
	//else if (cmdOptionExists(argv, argv + argc, "--GetPhone")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetPhoneNumber());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetPhone")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetPhoneNumber(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetPhone"))));
	//	return 0;
	//}
	//
	//else if (cmdOptionExists(argv, argv + argc, "--GetAddress1")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetAddress1());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetAddress1")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetAddress1(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetAddress1"))));
	//	return 0;
	//}
	//
	//else if (cmdOptionExists(argv, argv + argc, "--GetAddress2")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetAddress2());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetAddress2")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetAddress2(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetAddress2"))));
	//	return 0;
	//}
	//
	//else if (cmdOptionExists(argv, argv + argc, "--GetCity")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetCity());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetCity")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetCity(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetCity"))));
	//	return 0;
	//}
	//
	//else if (cmdOptionExists(argv, argv + argc, "--GetState")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetState());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetState")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetState(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetState"))));
	//	return 0;
	//}
	//
	//else if (cmdOptionExists(argv, argv + argc, "--GetCountryCode")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetCountryCode());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetCountryCode")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetCountryCode(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetCountryCode"))));
	//	return 0;
	//}
	//
	//else if (cmdOptionExists(argv, argv + argc, "--GetCountryDesc")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetCountryDesc());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetCountryDesc")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetCountryDesc(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetCountryDesc"))));
	//	return 0;
	//}
	// 
	//else if (cmdOptionExists(argv, argv + argc, "--GetZip")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetZipCode());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetZip")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetZipCode(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetZipCode"))));
	//	return 0;
	//}
	// 
	//else if (cmdOptionExists(argv, argv + argc, "--GetIsoLanguage")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetIsoLanguage());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetIsoLanguage")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetIsoLanguage(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetIsoLanguage"))));
	//	return 0;
	//}
	// 
	//else if (cmdOptionExists(argv, argv + argc, "--GetMSUpdate")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetMSUpdate());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetMSUpdate")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetMSUpdate(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetMSUpdate"))));
	//	return 0;
	//}
	// 
	//else if (cmdOptionExists(argv, argv + argc, "--GetMSOffer")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetMSOffer());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetMSOffer")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetMSOffer(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetMSOffer"))));
	//	return 0;
	//}
	// 
	//else if (cmdOptionExists(argv, argv + argc, "--GetOtherOffer")) {
	//	std::cout << _com_util::ConvertBSTRToString(GetOtherOffer());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--SetOtherOffer")) {
	//	std::cout << _com_util::ConvertBSTRToString(SetOtherOffer(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetOtherOffer"))));
	//	return 0;
	//}

	/*else*/ if (cmdOptionExists(argv, argv + argc, "--GetInstallationID")) {
		std::cout << _com_util::ConvertBSTRToString(GetInstallationID());
		return 0;
	}
	else if (cmdOptionExists(argv, argv + argc, "--SetConfirmationID")) {
		std::cout << _com_util::ConvertBSTRToString(SetConfirmationID(_com_util::ConvertStringToBSTR(getCmdOption(argv, argv + argc, "--SetConfirmationID"))));
		return 0;
	}

	else if (cmdOptionExists(argv, argv + argc, "--GetWPALeft")) {
		std::cout << _com_util::ConvertBSTRToString(GetWPALeft());
		return 0;
	}
	else if (cmdOptionExists(argv, argv + argc, "--GetEvalLeft")) {
		std::cout << _com_util::ConvertBSTRToString(GetEvalLeft());
		return 0;
	}

	//else if (cmdOptionExists(argv, argv + argc, "--ProcNewLicense")) {
	//	std::cout << _com_util::ConvertBSTRToString(NewLicenseRequest());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--ProcDropLicense")) {
	//	std::cout << _com_util::ConvertBSTRToString(DroppedLicenseRequest());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--ProcReisLicense")) {
	//	std::cout << _com_util::ConvertBSTRToString(ReissueLicenseRequest());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--ReviseCustInfo")) {
	//	std::cout << _com_util::ConvertBSTRToString(ReviseCustomerInfo());
	//	return 0;
	//}
	//else if (cmdOptionExists(argv, argv + argc, "--CheckOnline")) {
	//	std::cout << _com_util::ConvertBSTRToString(EnsureInternetConnection());
	//	return 0;
	//}

	else if (cmdOptionExists(argv, argv + argc, "--SetProductKey")) {
		std::cout << _com_util::ConvertBSTRToString(SetProductKey(convertCharArrayToLPCWSTR(getCmdOption(argv, argv + argc, "--SetProductKey"))));
		return 0;
	}
	else if (cmdOptionExists(argv, argv + argc, "--GetProductID")) {
		std::cout << _com_util::ConvertBSTRToString(GetProductID());
		return 0;
	}

	else if (cmdOptionExists(argv, argv + argc, "--GetUsage")) {
		std::cout << text;
		return 0;
	}
	else {
		std::cout << "An error occurred at main: No arguments listed\n\n";
		std::cout << text;
		return 0;
	}
	return 0;
}