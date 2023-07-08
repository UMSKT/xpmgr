#define STRICT
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <commctrl.h>
#include <stdint.h>
#include <intrin.h>
#include <algorithm>
#include <atlconv.h>
#include <iostream>
#include <comutil.h>
#include <cstdlib>
#include <regex>
#include <atlstr.h>
#include <ole2.h>
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

// The magic numbers, interface, and LoadLicenseManager() function was made by diamondggg on MyDigitalLife.
// LoadLicenseManager() was modified to suit a CLI rather than a GUI.
// Everything else was either made by TheTank20 or copy pasted from StackOverflow.
static wchar_t strings[14][256];

static CLSID licdllCLSID = { 0xACADF079, 0xCBCD, 0x4032, {0x83, 0xF2, 0xFA, 0x47, 0xC4, 0xDB, 0x09, 0x6F} };
static IID licenseAgentIID = { 0xB8CBAD79, 0x3F1F, 0x481A, {0xBB, 0x0C, 0xE7, 0xBB, 0xD7, 0x7B, 0xDD, 0xD1} };
//IID for ICOMLicenseAgent2, with three extra functions
//static IID licenseAgentIID2 = {0x6A07C5A3, 0x9C67, 0x4BB6, {0xB0, 0x20, 0xEC, 0xBE, 0x7F, 0xDF, 0xD3, 0x26}};

#undef INTERFACE
#define INTERFACE ICOMLicenseAgent
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
	STDMETHOD(GetFirstName)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetFirstName)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetLastName)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetLastName)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetOrgName)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetOrgName)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetEmail)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetEmail)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetPhone)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetPhone)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetAddress1)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetAddress1)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetCity)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetCity)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetState)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetState)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetCountryCode)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetCountryCode)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetCountryDesc)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetCountryDesc)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetZip)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetZip)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetIsoLanguage)(THIS_ ULONG * pdwVal) PURE; // implemented
	STDMETHOD(SetIsoLanguage)(THIS_ ULONG dwNewVal) PURE; // implemented
	STDMETHOD(GetMSUpdate)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetMSUpdate)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetMSOffer)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetMSOffer)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetOtherOffer)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetOtherOffer)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(GetAddress2)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(SetAddress2)(THIS_ BSTR bstrNewVal) PURE; // implemented
	STDMETHOD(AsyncProcessHandshakeRequest)(THIS_ LONG bReviseCustInfo) PURE;
	STDMETHOD(AsyncProcessNewLicenseRequest)(THIS) PURE;
	STDMETHOD(AsyncProcessReissueLicenseRequest)(THIS) PURE;
	STDMETHOD(AsyncProcessReviseCustInfoRequest)(THIS) PURE;
	STDMETHOD(GetAsyncProcessReturnCode)(THIS_ ULONG * pdwRetCode) PURE;
	STDMETHOD(AsyncProcessDroppedLicenseRequest)(THIS) PURE; 
	STDMETHOD(GenerateInstallationId)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(DepositConfirmationId)(THIS_ BSTR bstrVal, ULONG * pdwRetCode) PURE; // implemented
	STDMETHOD(GetExpirationInfo)(THIS_ ULONG * pdwWPALeft, ULONG * pdwEvalLeft) PURE; // implemented
	STDMETHOD(AsyncProcessRegistrationRequest)(THIS) PURE;
	STDMETHOD(ProcessHandshakeRequest)(THIS_ LONG bReviseCustInfo) PURE;
	STDMETHOD(ProcessNewLicenseRequest)(THIS) PURE; // implemented
	STDMETHOD(ProcessDroppedLicenseRequest)(THIS) PURE; // implemented
	STDMETHOD(ProcessReissueLicenseRequest)(THIS) PURE; // implemented
	STDMETHOD(ProcessReviseCustInfoRequest)(THIS) PURE; // implemented
	STDMETHOD(EnsureInternetConnection)(THIS) PURE; // implemented
	STDMETHOD(SetProductKey)(THIS_ LPWSTR pszNewProductKey) PURE; // implemented
	STDMETHOD(GetProductID)(THIS_ BSTR * pbstrVal) PURE; // implemented
	STDMETHOD(VerifyCheckDigits)(THIS_ BSTR bstrCIDIID, LONG * pbValue) PURE;
};

static BOOL ComInitialized = FALSE;
static ICOMLicenseAgent* LicenseAgent = NULL;

static BOOL LoadLicenseManager()
{
	if (!ComInitialized) {
		HRESULT status = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (FAILED(status)) {
			std::cout << "An error occurred at CoInitializeEx: ", status;
			return FALSE;
		}
		ComInitialized = TRUE;
	}
	if (!LicenseAgent) {
		HRESULT status = CoCreateInstance(licdllCLSID, NULL, CLSCTX_INPROC_SERVER, licenseAgentIID, (void**)&LicenseAgent);
		int good = 0;
		if (SUCCEEDED(status)) {
			ULONG dwRetCode;
			status = LicenseAgent->Initialize(0xC475, 3, 0, &dwRetCode);
			if (SUCCEEDED(status) && dwRetCode == 0) {
				good = 1;
			}
			else {
				LicenseAgent->Release();
				LicenseAgent = NULL;
			}
		}
		if (!good) {
			std::cout << "An error occurred at CoCreateInstance: " + status;
			return FALSE;
		}
	}
	return TRUE;
}

#pragma region Internals
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

	PROCESSENTRY32W processEntry;
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
		std::cerr << "Failed to create process snapshot." << std::endl;
		return false;
	}

	PROCESSENTRY32W processEntry;
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
					std::cerr << "Failed to terminate the process." << std::endl;
					CloseHandle(processHandle);
					CloseHandle(snapshot);
					return false;  // Failed to terminate the process
				}
			}
			else
			{
				std::cerr << "Failed to open process handle." << std::endl;
				CloseHandle(snapshot);
				return false;  // Failed to open process handle
			}
		}
	}

	CloseHandle(snapshot);
	return false;  // Process not found
}

#pragma endregion

//#pragma region FirstName
//static BSTR GetFirstName() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR firstName = NULL;
//	HRESULT status = LicenseAgent->GetFirstName(&firstName);
//	if (FAILED(status) || !firstName) {
//		std::cout << "An error occurred at GetFirstName: " + status;
//		return SysAllocString(L"Failed to get first name");
//	}
//	else {
//		return firstName;
//	}
//}
//
//static BSTR SetFirstName(BSTR firstName) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetFirstName(firstName);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetFirstName: " + status;
//		return SysAllocString(L"Failed to set first name");
//	}
//	return SysAllocString(L"Successfully set first name");
//}
//#pragma endregion
//
//#pragma region LastName
//static BSTR GetLastName() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR lastName = NULL;
//	HRESULT status = LicenseAgent->GetLastName(&lastName);
//	if (FAILED(status) || !lastName) {
//		std::cout << "An error occurred at GetLastName: " + status;
//		return SysAllocString(L"Failed to get last name");
//	}
//	else {
//		return lastName;
//	}
//}
//
//static BSTR SetLastName(BSTR lastName) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetLastName(lastName);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetLastName: " + status;
//		return SysAllocString(L"Failed to set last name");
//	}
//	return SysAllocString(L"Successfully set last name");
//}
//#pragma endregion
//
//#pragma region OrgName
//static BSTR GetOrgName() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR orgName = NULL;
//	HRESULT status = LicenseAgent->GetOrgName(&orgName);
//	if (FAILED(status) || !orgName) {
//		std::cout << "An error occurred at GetOrgName: " + status;
//		return SysAllocString(L"Failed to get org name");
//	}
//	else {
//		return orgName;
//	}
//}
//
//static BSTR SetOrgName(BSTR orgName) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetOrgName(orgName);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetOrgName: " + status;
//		return SysAllocString(L"Failed to set org name");
//	}
//	return SysAllocString(L"Successfully set org name");
//}
//#pragma endregion
//
//#pragma region Email
//static BSTR GetEmail() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR email = NULL;
//	HRESULT status = LicenseAgent->GetEmail(&email);
//	if (FAILED(status) || !email) {
//		std::cout << "An error occurred at GetEmail: " + status;
//		return SysAllocString(L"Failed to get email");
//	}
//	else {
//		return email;
//	}
//}
//
//static BSTR SetEmail(BSTR email) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetEmail(email);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetEmail: " + status;
//		return SysAllocString(L"Failed to set email");
//	}
//	return SysAllocString(L"Successfully set email");
//}
//#pragma endregion
//
//#pragma region Addresses
//static BSTR GetAddress1() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR address1 = NULL;
//	HRESULT status = LicenseAgent->GetAddress1(&address1);
//	if (FAILED(status) || !address1) {
//		std::cout << "An error occurred at GetAddress1: " + status;
//		return SysAllocString(L"Failed to get 1st address");
//	}
//	else {
//		return address1;
//	}
//}
//
//static BSTR SetAddress1(BSTR address1) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetAddress1(address1);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetAddress1: " + status;
//		return SysAllocString(L"Failed to set 1st address");
//	}
//	return SysAllocString(L"Successfully set 1st address");
//}
//
//static BSTR GetAddress2() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR address2 = NULL;
//	HRESULT status = LicenseAgent->GetAddress2(&address2);
//	if (FAILED(status) || !address2) {
//		std::cout << "An error occurred at GetAddress2: " + status;
//		return SysAllocString(L"Failed to get 2nd address");
//	}
//	else {
//		return address2;
//	}
//}
//
//static BSTR SetAddress2(BSTR address2) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetAddress2(address2);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetAddress2: " + status;
//		return SysAllocString(L"Failed to set 2nd address");
//	}
//	return SysAllocString(L"Successfully set 2nd address");
//}
//#pragma endregion
//
//#pragma region PhoneNumber
//static BSTR GetPhoneNumber() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR phoneNumber = NULL;
//	HRESULT status = LicenseAgent->GetPhone(&phoneNumber);
//	if (FAILED(status) || !phoneNumber) {
//		std::cout << "An error occurred at GetPhone: " + status;
//		return SysAllocString(L"Failed to get phone number");
//	}
//	else {
//		return phoneNumber;
//	}
//}
//
//static BSTR SetPhoneNumber(BSTR phoneNumber) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetPhone(phoneNumber);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetPhone: " + status;
//		return SysAllocString(L"Failed to set phone number");
//	}
//	return SysAllocString(L"Successfully set phone number");
//}
//#pragma endregion
//
//#pragma region City
//static BSTR GetCity() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR city = NULL;
//	HRESULT status = LicenseAgent->GetCity(&city);
//	if (FAILED(status) || !city) {
//		std::cout << "An error occurred at GetCity: " + status;
//		return SysAllocString(L"Failed to get city");
//	}
//	else {
//		return city;
//	}
//}
//
//static BSTR SetCity(BSTR city) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetCity(city);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetCity: " + status;
//		return SysAllocString(L"Failed to set city");
//	}
//	return SysAllocString(L"Successfully set city");
//}
//#pragma endregion
//
//#pragma region State
//static BSTR GetState() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR state = NULL;
//	HRESULT status = LicenseAgent->GetState(&state);
//	if (FAILED(status) || !state) {
//		std::cout << "An error occurred at GetState: " + status;
//		return SysAllocString(L"Failed to get state");
//	}
//	else {
//		return state;
//	}
//}
//
//static BSTR SetState(BSTR state) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetState(state);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetState: " + status;
//		return SysAllocString(L"Failed to set state");
//	}
//	return SysAllocString(L"Successfully set state");
//}
//#pragma endregion
//
//#pragma region CountryCode
//static BSTR GetCountryCode() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR countryCode = NULL;
//	HRESULT status = LicenseAgent->GetCountryCode(&countryCode);
//	if (FAILED(status) || !countryCode) {
//		std::cout << "An error occurred at GetCountryCode: " + status;
//		return SysAllocString(L"Failed to get country code");
//	}
//	else {
//		return countryCode;
//	}
//}
//
//static BSTR SetCountryCode(BSTR countryCode) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetCountryCode(countryCode);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetCountryCode: " + status;
//		return SysAllocString(L"Failed to set country code");
//	}
//	return SysAllocString(L"Successfully set country code");
//}
//#pragma endregion
//
//#pragma region CountryDesc
//static BSTR GetCountryDesc() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR countryDesc = NULL;
//	HRESULT status = LicenseAgent->GetCountryDesc(&countryDesc);
//	if (FAILED(status) || !countryDesc) {
//		std::cout << "An error occurred at GetCountryDesc: " + status;
//		return SysAllocString(L"Failed to get country desc");
//	}
//	else {
//		return countryDesc;
//	}
//}
//
//static BSTR SetCountryDesc(BSTR countryDesc) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetCountryDesc(countryDesc);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetCountryDesc: " + status;
//		return SysAllocString(L"Failed to set country desc");
//	}
//	return SysAllocString(L"Successfully set country desc");
//}
//#pragma endregion
//
//#pragma region ZipCode
//static BSTR GetZipCode() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR zipCode = NULL;
//	HRESULT status = LicenseAgent->GetZip(&zipCode);
//	if (FAILED(status) || !zipCode) {
//		std::cout << "An error occurred at GetZip: " + status;
//		return SysAllocString(L"Failed to get zip code");
//	}
//	else {
//		return zipCode;
//	}
//}
//
//static BSTR SetZipCode(BSTR zipCode) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetZip(zipCode);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetZip: " + status;
//		return SysAllocString(L"Failed to set zip code");
//	}
//	return SysAllocString(L"Successfully set zip code");
//}
//#pragma endregion
//
//#pragma region IsoLanguage
//static BSTR GetIsoLanguage() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	ULONG isoLanguage = 0;
//	HRESULT status = LicenseAgent->GetIsoLanguage(&isoLanguage);
//	if (FAILED(status) || !isoLanguage) {
//		std::cout << "An error occurred at GetIsoLanguage: " + status;
//		return SysAllocString(L"Failed to get ISO language");
//	}
//	else {
//		return SysAllocString(_bstr_t(isoLanguage).Detach());
//	}
//}
//
//static BSTR SetIsoLanguage(BSTR isoLanguage) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetIsoLanguage(ConvertToULONG(BstrToChar(isoLanguage)));
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetIsoLangage: " + status;
//		return SysAllocString(L"Failed to set zip code");
//	}
//	return SysAllocString(L"Successfully set zip codes");
//}
//#pragma endregion
//
//#pragma region UpdatesAndOffers
//static BSTR GetMSUpdate() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR msUpdate = NULL;
//	HRESULT status = LicenseAgent->GetMSUpdate(&msUpdate);
//	if (FAILED(status) || !msUpdate) {
//		std::cout << "An error occurred at GetMSUpdate: " + status;
//		return SysAllocString(L"Failed to get MS Update");
//	}
//	else {
//		return msUpdate;
//	}
//}
//
//static BSTR SetMSUpdate(BSTR msUpdate) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetMSUpdate(msUpdate);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetMSUpdate: " + status;
//		return SysAllocString(L"Failed to set MS Update");
//	}
//	return SysAllocString(L"Successfully set MS Update");
//}
//
//static BSTR GetMSOffer() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR msOffer = NULL;
//	HRESULT status = LicenseAgent->GetMSOffer(&msOffer);
//	if (FAILED(status) || !msOffer) {
//		std::cout << "An error occurred at GetMSOffer: " + status;
//		return SysAllocString(L"Failed to get MS Offer");
//	}
//	else {
//		return msOffer;
//	}
//}
//
//static BSTR SetMSOffer(BSTR msOffer) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetMSOffer(msOffer);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetMSOffer: " + status;
//		return SysAllocString(L"Failed to set MS Offer");
//	}
//	return SysAllocString(L"Successfully set MS Offer");
//}
//
//static BSTR GetOtherOffer() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	BSTR otherOffer = NULL;
//	HRESULT status = LicenseAgent->GetOtherOffer(&otherOffer);
//	if (FAILED(status) || !otherOffer) {
//		std::cout << "An error occurred at GetOtherOffer: " + status;
//		return SysAllocString(L"Failed to get other offer");
//	}
//	else {
//		return otherOffer;
//	}
//}
//
//static BSTR SetOtherOffer(BSTR otherOffer) {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->SetOtherOffer(otherOffer);
//	if (FAILED(status)) {
//		std::cout << "An error occurred at SetOtherOffer: " + status;
//		return SysAllocString(L"Failed to set other offer");
//	}
//	return SysAllocString(L"Successfully set other offer");
//}
//#pragma endregion

#pragma region ExpirationDate
static BSTR GetWPALeft() {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
	}

	ULONG dwWPALeft = 0, dwEvalLeft = 0;
	HRESULT status = LicenseAgent->GetExpirationInfo(&dwWPALeft, &dwEvalLeft);
	if (FAILED(status)) {
		LicenseAgent->Release();
		LicenseAgent = NULL;
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
	HRESULT status = LicenseAgent->GetExpirationInfo(&dwWPALeft, &dwEvalLeft);
	if (FAILED(status)) {
		LicenseAgent->Release();
		LicenseAgent = NULL;
		return SysAllocString(L"An error occurred at GetExpirationInfo: " + status);
	}
	if (dwEvalLeft == 0x7FFFFFFF) {
		return SysAllocString(L"An error occurred at GetEvalLeft: Not an evaluation copy");
	}
	return SysAllocString(_bstr_t(dwEvalLeft).Detach());
}
#pragma endregion

#pragma region OfflineActivation

static BSTR VerifyCheckDigits(BSTR cidChunk) {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
	}

	if (0 == wcscmp(GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}

	LONG pbValue;
	HRESULT status = LicenseAgent->VerifyCheckDigits(cidChunk, &pbValue);
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
	HRESULT status = LicenseAgent->DepositConfirmationId(confirmationID, &dwRetCode);
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
	HRESULT status = LicenseAgent->GenerateInstallationId(&installationID);
	if (FAILED(status) || !installationID) {
		return SysAllocString(L"An error occurred at GenerateInstallationId: " + status);
	}
	else {
		return installationID;
	}
}
#pragma endregion

#pragma region ProductKeyAndID
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

	HRESULT status = LicenseAgent->SetProductKey(productKey);
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

	HRESULT status = LicenseAgent->GetProductID(&productID);
	if (FAILED(status)) {
		return SysAllocString(L"An error occurred at GetProductID: " + status);
	}
	else {
		return productID;
	}
}
#pragma endregion

//#pragma region OnlineActivation
//static BSTR EnsureInternetConnection() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	HRESULT status = LicenseAgent->EnsureInternetConnection();
//	if (FAILED(status)) {
//		std::cout << "An error occurred at EnsureInternetConnection: " + status;
//		return SysAllocString(L"Failed to ensure internet connection (offline?)");
//	}
//	else {
//		return SysAllocString(L"Connected to the Internet");
//	}
//}
//
//static BSTR NewLicenseRequest() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	if (EnsureInternetConnection() != SysAllocString(L"Connected to the Internet")) {
//		return SysAllocString(L"Internet required to process new license request");
//	}
//
//	HRESULT status = LicenseAgent->ProcessNewLicenseRequest();
//	if (FAILED(status)) {
//		std::cout << "An error occurred at ProcessNewLicenseRequest: " + status;
//		return SysAllocString(L"Failed to process new license request");
//	}
//	else {
//		return SysAllocString(L"Successfully processed new license request");
//	}
//}
//
//static BSTR DroppedLicenseRequest() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	if (EnsureInternetConnection() != SysAllocString(L"Connected to the Internet")) {
//		return SysAllocString(L"Internet required to process dropped license request");
//	}
//
//	HRESULT status = LicenseAgent->ProcessDroppedLicenseRequest();
//	if (FAILED(status)) {
//		std::cout << "An error occurred at ProcessDroppedLicenseRequest: " + status;
//		return SysAllocString(L"Failed to process dropped license request");
//	}
//	else {
//		return SysAllocString(L"Successfully processed dropped license request");
//	}
//}
//
//static BSTR ReissueLicenseRequest() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	if (EnsureInternetConnection() != SysAllocString(L"Connected to the Internet")) {
//		return SysAllocString(L"Internet required to process reissue license request");
//	}
//
//	HRESULT status = LicenseAgent->ProcessReissueLicenseRequest();
//	if (FAILED(status)) {
//		std::cout << "An error occurred at ProcessReissueLicenseRequest: " + status;
//		return SysAllocString(L"Failed to process reissue license request");
//	}
//	else {
//		return SysAllocString(L"Successfully processed reissue license request");
//	}
//}
//
//static BSTR ReviseCustomerInfo() {
//	if (!LoadLicenseManager()) {
//		return SysAllocString(L"An error occurred at LoadLicenseManager: Failed to load");
//	}
//
//	if (EnsureInternetConnection() != SysAllocString(L"Connected to the Internet")) {
//		return SysAllocString(L"Internet required to revise customer info");
//	}
//
//	HRESULT status = LicenseAgent->ProcessReviseCustInfoRequest();
//	if (FAILED(status)) {
//		std::cout << "An error occurred at ProcessReviseCustInfoRequest: " + status;
//		return SysAllocString(L"Failed to revise customer info");
//	}
//	else {
//		return SysAllocString(L"Successfully revised customer info");
//	}
//}
//
//#pragma endregion



int main(int argc, char* argv[])
{
	const char* text =
		"Usage: \n"
		//"--GetFirstName: Gets first name registered in the system\n"
		//"--SetFirstName [name]: Sets first name registered in the system\n"
		//"--GetLastName: Gets last name registered in the system\n"
		//"--SetLastName [name]: Sets last name registered in the system\n"
		//"--GetOrgName: Gets organization name registered in the system\n"
		//"--SetOrgName [name]: Sets organization name registered in the system\n"
		//"--GetEmail: Gets email registered in the system\n"
		//"--SetEmail [email@domain.net]: Sets email registered in the system\n"
		//"--GetPhone: Gets phone number registered in the system\n"
		//"--SetPhone [##########]: Sets phone number registered in the system\n"
		//"--GetAddress1: Gets 1st address registered in the system\n"
		//"--SetAddress1 [any string]: Sets 1st address registered in the system\n"
		//"--GetAddress2: Gets 2nd address registered in the system\n"
		//"--SetAddress2 [any string]: Sets 2nd address registered in the system\n"
		//"--GetCity: Gets city registered in the system\n"
		//"--SetCity [any string]: Sets city registered in the system\n"
		//"--GetState: Gets state/province registered in the system\n"
		//"--SetState [any string]: Sets state/province registered in the system\n"
		//"--GetCountryCode: Gets country/region code registered in the system\n"
		//"--SetCountryCode [xx-XX]: Sets country/region code registered in the system\n"
		//"--GetCountryDesc: Gets country desc registered in the system\n"
		//"--SetCountryDesc [?]: Sets country desc registered in the system\n"
		//"--GetZip: Gets zip code registered in the system\n"
		//"--SetZip [#####]: Sets zip code registered in the system\n"
		//"--GetIsoLanguage: Gets ISO 3166-1 language code registered in the system\n"
		//"--SetIsoLanguage [###]: Sets ISO 3166-1 language code registered in the system\n"
		//"--GetMSUpdate: Gets whether Windows Update is enabled or not (?)\n"
		//"--SetMSUpdate [?]: Sets whether Windows Update is enabled or not (?)\n"
		//"--GetMSOffer: Gets if the user registered for offers from Microsoft or not\n"
		//"--SetMSOffer [?]: Sets if the user registered for offers from Microsoft or not\n"
		//"--GetOtherOffer: Gets if the user registered for offers from MS partners or not\n"
		//"--SetOtherOffer [?]: Sets if the user registered for offers from MS partners or not\n"
		"--GetInstallationID: Gets the Installation ID\n"
		"--SetConfirmationID [cid]: Sets Confirmation ID (If successful, activates Windows)\n"
		"--GetWPALeft: Gets the days before activation is required (Grace period)\n"
		"--GetEvalLeft: Gets the days before the evaluation period expires (Eval. copies only)\n"
		//"--ProcNewLicense: Processes new license request (Requires Internet connection)\n"
		//"--ProcDropLicense: Processes dropped license request (Requires Internet connection)\n"
		//"--ProcReisLicense: Processes reissue license request (Requires Internet connection)\n"
		//"--ReviseCustInfo: Revise customer info (Requires Internet connection)\n"
		//"--CheckOnline: Checks if connected to Microsoft's servers\n"
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