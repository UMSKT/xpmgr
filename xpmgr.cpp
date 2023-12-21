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

const char* specifiedProduct = NULL;

// This is a really weird way of making a GUID. In short, this becomes ACADF079-CBCD-4032-83F2-FA47C4DB096F. Ignore the 0x and it makes sense.
static CLSID XP_CLSID = { 0xACADF079, 0xCBCD, 0x4032, {0x83, 0xF2, 0xFA, 0x47, 0xC4, 0xDB, 0x09, 0x6F} };
static IID XP_IID = { 0xB8CBAD79, 0x3F1F, 0x481A, {0xBB, 0x0C, 0xE7, 0xBB, 0xD7, 0x7B, 0xDD, 0xD1} };

static CLSID O2003_CLSID = { 0x2DF8D04C, 0x5BFA, 0x101B, { 0xBD, 0xE5, 0x00, 0xAA, 0x00, 0x44, 0xDE, 0x52 } };
static IID O2003_IID = { 0x00194002, 0xD9C3, 0x11D3, { 0x8D, 0x59, 0x00, 0x50, 0x04, 0x83, 0x84, 0xE3 } };

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

#undef O2003_INTERFACE
#define O2003_INTERFACE ILicAgent
DECLARE_INTERFACE_(ILicAgent, IDispatch)
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

	/*** ILicAgent methods ***/
	// ChatGPT actually got this right wtf
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
	STDMETHOD(GetIsoLanguage)(THIS_ unsigned long* pdwVal) PURE;
	STDMETHOD(SetIsoLanguage)(THIS_ unsigned long dwNewVal) PURE;
	STDMETHOD(GetMSUpdate)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetMSUpdate)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetMSOffer)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetMSOffer)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetOtherOffer)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetOtherOffer)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetAddress2)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetAddress2)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(CheckSystemClock)(THIS_ unsigned long* pdwRetCode) PURE;
	STDMETHOD(GetExistingExpiryDate)(THIS_ DATE * pDateVal) PURE;
	STDMETHOD(GetNewExpiryDate)(THIS_ DATE * pDateVal) PURE;
	STDMETHOD(GetBillingFirstName)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetBillingFirstName)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetBillingLastName)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetBillingLastName)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetBillingPhone)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetBillingPhone)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetBillingAddress1)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetBillingAddress1)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetBillingAddress2)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetBillingAddress2)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetBillingCity)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetBillingCity)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetBillingState)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetBillingState)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetBillingCountryCode)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetBillingCountryCode)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(GetBillingZip)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(SetBillingZip)(THIS_ BSTR bstrNewVal) PURE;
	STDMETHOD(SaveBillingInfo)(THIS_ int bSave, unsigned long* pdwRetVal) PURE;
	STDMETHOD(IsCCRenewalCountry)(THIS_ BSTR bstrCountryCode, int* pbRetVal) PURE;
	STDMETHOD(GetVATLabel)(THIS_ BSTR bstrCountryCode, BSTR * pbstrVATLabel) PURE;
	STDMETHOD(GetCCRenewalExpiryDate)(THIS_ DATE * pDateVal) PURE;
	STDMETHOD(SetVATNumber)(THIS_ BSTR bstrVATNumber) PURE;
	STDMETHOD(SetCreditCardType)(THIS_ BSTR bstrCCCode) PURE;
	STDMETHOD(SetCreditCardNumber)(THIS_ BSTR bstrCCNumber) PURE;
	STDMETHOD(SetCreditCardExpiryYear)(THIS_ unsigned long dwCCYear) PURE;
	STDMETHOD(SetCreditCardExpiryMonth)(THIS_ unsigned long dwCCMonth) PURE;
	STDMETHOD(GetCreditCardCount)(THIS_ unsigned long* pdwCount) PURE;
	STDMETHOD(GetCreditCardCode)(THIS_ unsigned long dwIndex, BSTR * pbstrCode) PURE;
	STDMETHOD(GetCreditCardName)(THIS_ unsigned long dwIndex, BSTR * pbstrName) PURE;
	STDMETHOD(GetVATNumber)(THIS_ BSTR * pbstrVATNumber) PURE;
	STDMETHOD(GetCreditCardType)(THIS_ BSTR * pbstrCCCode) PURE;
	STDMETHOD(GetCreditCardNumber)(THIS_ BSTR * pbstrCCNumber) PURE;
	STDMETHOD(GetCreditCardExpiryYear)(THIS_ unsigned long* pdwCCYear) PURE;
	STDMETHOD(GetCreditCardExpiryMonth)(THIS_ unsigned long* pdwCCMonth) PURE;
	STDMETHOD(GetDisconnectOption)(THIS_ int* pbRetVal) PURE;
	STDMETHOD(SetDisconnectOption)(THIS_ int bNewVal) PURE;
	STDMETHOD(AsyncProcessHandshakeRequest)(THIS_ int bReviseCustInfo) PURE;
	STDMETHOD(AsyncProcessNewLicenseRequest)(THIS) PURE;
	STDMETHOD(AsyncProcessReissueLicenseRequest)(THIS) PURE;
	STDMETHOD(AsyncProcessRetailRenewalLicenseRequest)(THIS) PURE;
	STDMETHOD(AsyncProcessReviseCustInfoRequest)(THIS) PURE;
	STDMETHOD(AsyncProcessCCRenewalPriceRequest)(THIS) PURE;
	STDMETHOD(AsyncProcessCCRenewalLicenseRequest)(THIS) PURE;
	STDMETHOD(GetAsyncProcessReturnCode)(THIS_ unsigned long* pdwRetCode) PURE;
	STDMETHOD(IsUpgradeAvailable)(THIS_ int* pbUpgradeAvailable) PURE;
	STDMETHOD(WantUpgrade)(THIS_ int bWantUpgrade) PURE;
	STDMETHOD(AsyncProcessDroppedLicenseRequest)(THIS) PURE;
	STDMETHOD(GenerateInstallationId)(THIS_ BSTR * pbstrVal) PURE;
	STDMETHOD(DepositConfirmationId)(THIS_ BSTR bstrVal, unsigned long* pdwRetCode) PURE;
	STDMETHOD(VerifyCheckDigits)(THIS_ BSTR bstrCIDIID, int* pbValue) PURE;
	STDMETHOD(GetCurrentExpiryDate)(THIS_ DATE * pDateVal) PURE;
	STDMETHOD(CancelAsyncProcessRequest)(THIS_ int bIsLicenseRequest) PURE;
	STDMETHOD(GetCurrencyDescription)(THIS_ unsigned long dwCurrencyIndex, BSTR * pbstrVal) PURE;
	STDMETHOD(GetPriceItemCount)(THIS_ unsigned long* pdwCount) PURE;
	STDMETHOD(GetPriceItemLabel)(THIS_ unsigned long dwIndex, BSTR * pbstrVal) PURE;
	STDMETHOD(GetPriceItemValue)(THIS_ unsigned long dwCurrencyIndex, unsigned long dwIndex, BSTR * pbstrVal) PURE;
	STDMETHOD(GetInvoiceText)(THIS_ BSTR * pNewVal) PURE;
	STDMETHOD(GetBackendErrorMsg)(THIS_ BSTR * pbstrErrMsg) PURE;
	STDMETHOD(GetCurrencyOption)(THIS_ unsigned long* dwCurrencyOption) PURE;
	STDMETHOD(SetCurrencyOption)(THIS_ unsigned long dwCurrencyOption) PURE;
	STDMETHOD(GetEndOfLifeHtmlText)(THIS_ BSTR * pbstrHtmlText) PURE;
	STDMETHOD(DisplaySSLCert)(THIS_ unsigned long* dwRetCode) PURE;
};

static BOOL XP_ComInitialized = FALSE;
static ICOMLicenseAgent* XP_LicenseAgent = NULL;

static BOOL O2003_ComInitialized = FALSE;
static ILicAgent* O2003_LicenseAgent = NULL;

static BOOL XP_LoadLicenseManager()
{
	if (!XP_ComInitialized) {
		HRESULT status = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (FAILED(status)) {
            const char* errorString = "An error occurred at CoInitializeEx: ";
            int bufferSize = snprintf(nullptr, 0, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
            char* result = new char[bufferSize + 1];
            snprintf(result, bufferSize + 1, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
			std::cout << result;
			return FALSE;
		}
		XP_ComInitialized = TRUE;
	}
	if (!XP_LicenseAgent) {
		HRESULT status = CoCreateInstance(XP_CLSID, NULL, CLSCTX_INPROC_SERVER, XP_IID, (void**)&XP_LicenseAgent);
		int good = 0;
		if (SUCCEEDED(status)) {
			ULONG dwRetCode;
			status = XP_LicenseAgent->Initialize(0xC475 /* This needs to be changed, I believe it's causing the crashing.*/ , 3, 0, &dwRetCode);
			if (SUCCEEDED(status) && dwRetCode == 0) {
				good = 1;
			}
			else {
				XP_LicenseAgent->Release();
				XP_LicenseAgent = NULL;
			}
		}
		if (!good) {
            const char* errorString = "An error occurred at CoCreateInstance: ";
            int bufferSize = snprintf(nullptr, 0, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
            char* result = new char[bufferSize + 1];
            snprintf(result, bufferSize + 1, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
            std::cout << result;
			return FALSE;
		}
	}
	return TRUE;
}

static BOOL O2003_LoadLicenseManager()
{
	if (!O2003_ComInitialized) {
		HRESULT status = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (FAILED(status)) {
            const char* errorString = "An error occurred at CoInitializeEx: ";
            int bufferSize = snprintf(nullptr, 0, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
            char* result = new char[bufferSize + 1];
            snprintf(result, bufferSize + 1, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
            std::cout << result;
			return FALSE;
		}
		O2003_ComInitialized = TRUE;
	}
	if (!O2003_LicenseAgent) {
		HRESULT status = CoCreateInstance(O2003_CLSID, NULL, CLSCTX_INPROC_SERVER, O2003_IID, (void**)&O2003_LicenseAgent);
		int good = 0;
		if (SUCCEEDED(status)) {
			ULONG dwRetCode;
			status = O2003_LicenseAgent->Initialize(0xC475, 3, 0, &dwRetCode);
			if (SUCCEEDED(status) && dwRetCode == 0) {
				good = 1;
			}
			else {
				O2003_LicenseAgent->Release();
				O2003_LicenseAgent = NULL;
			}
		}
		if (!good) {
            const char* errorString = "An error occurred at CoCreateInstance: ";
            int bufferSize = snprintf(nullptr, 0, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
            char* result = new char[bufferSize + 1];
            snprintf(result, bufferSize + 1, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
            std::cout << result;
			return FALSE;
		}
	}
	return TRUE;
}

#pragma region Internals
bool RegistryKeyExists(HKEY rootKey, const char* keyPath) {
	HKEY hKey;
	LSTATUS result = RegOpenKeyEx(rootKey, keyPath, 0, KEY_READ, &hKey);

	if (result == ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return true;
	}
	else if (result == ERROR_FILE_NOT_FOUND) {
		return false;
	}
	else {
		std::cerr << "Error opening registry key. Error code: " << result << std::endl;
		return false;
	}
}

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

BSTR ConvertToBSTR(const std::string& str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    BSTR bstr = SysAllocStringLen(NULL, size - 1);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, bstr, size);
    return bstr;
}

std::string ConvertToStdString(BSTR bstr) {
    int size = WideCharToMultiByte(CP_UTF8, 0, bstr, -1, NULL, 0, NULL, NULL);
    char* buffer = new char[size];
    WideCharToMultiByte(CP_UTF8, 0, bstr, -1, buffer, size, NULL, NULL);

    std::string result(buffer);
    delete[] buffer;

    return result;
}

BSTR ConvertCharToBSTR(const char* charString) {
    int size = MultiByteToWideChar(CP_UTF8, 0, charString, -1, NULL, 0);
    BSTR bstr = SysAllocStringLen(NULL, size - 1);
    MultiByteToWideChar(CP_UTF8, 0, charString, -1, bstr, size);
    return bstr;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
void safeStrncat(char* destination, const char* source, size_t size) {
    size_t destLen = strlen(destination);
    size_t srcLen = strlen(source);

    if (destLen + srcLen < size) {
        // fuck off Microsoft
        strncat(destination, source, size - destLen - 1); // -1 for the null terminator
    } else {
        // Handle buffer overflow error
        std::cerr << "Error: Buffer overflow during strncat operation." << std::endl;
    }
}
#pragma clang diagnostic pop

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

#pragma endregion

#pragma region Windows XP
static BSTR XP_GetWPALeft() {
	if (!XP_LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
	}

	ULONG dwWPALeft = 0, dwEvalLeft = 0;
	HRESULT status = XP_LicenseAgent->GetExpirationInfo(&dwWPALeft, &dwEvalLeft);
	if (FAILED(status)) {
		XP_LicenseAgent->Release();
		XP_LicenseAgent = NULL;
        const char* errorString = "An error occurred at CoInitializeEx: ";
        int bufferSize = snprintf(nullptr, 0, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        OLECHAR* wideResult = new OLECHAR[wideCharSize];
        MultiByteToWideChar(CP_UTF8, 0, result, -1, wideResult, wideCharSize);
		return SysAllocString(wideResult);
	}
	if (dwWPALeft == 0x7FFFFFFF) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}
	return SysAllocString(_bstr_t(dwWPALeft).Detach());
}

static BSTR XP_GetEvalLeft() {
	if (!XP_LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
	}

	ULONG dwWPALeft = 0, dwEvalLeft = 0;
	HRESULT status = XP_LicenseAgent->GetExpirationInfo(&dwWPALeft, &dwEvalLeft);
	if (FAILED(status)) {
		XP_LicenseAgent->Release();
		XP_LicenseAgent = NULL;
        const char* errorString = "An error occurred at GetExpirationInfo: ";
        int bufferSize = snprintf(nullptr, 0, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        OLECHAR* wideResult = new OLECHAR[wideCharSize];
        MultiByteToWideChar(CP_UTF8, 0, result, -1, wideResult, wideCharSize);
        return SysAllocString(wideResult);
	}
	if (dwEvalLeft == 0x7FFFFFFF) {
		return SysAllocString(L"An error occurred at GetEvalLeft: Not an evaluation copy");
	}
	return SysAllocString(_bstr_t(dwEvalLeft).Detach());
}

static BSTR XP_VerifyCheckDigits(BSTR cidChunk) {
	if (!XP_LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at XP_LoadLicenseManager: Failed to load");
	}

	if (0 == wcscmp(XP_GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}

	LONG pbValue;
	HRESULT status = XP_LicenseAgent->VerifyCheckDigits(cidChunk, &pbValue);
	if (FAILED(status) || !pbValue) {
        char errorMessage[70] = "An error occurred at XP_VerifyCheckDigits: ";
        char pbValueChar[20];
        snprintf(errorMessage, sizeof(errorMessage), "%ld", pbValue);
        safeStrncat(errorMessage, pbValueChar, sizeof(errorMessage));
        int len = MultiByteToWideChar(CP_UTF8, 0, errorMessage, -1, NULL, 0);
        OLECHAR* oleCharString = new OLECHAR[len];
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
		if (0 != wcscmp(XP_VerifyCheckDigits(ConvertCharToBSTR(cstr)), L"Successfully verified CID chunk")) {
			return SysAllocString(L"An error occurred at XP_VerifyCheckDigits: Check for misspelling");
		}
	}

	ULONG dwRetCode;
	HRESULT status = XP_LicenseAgent->DepositConfirmationId(confirmationID, &dwRetCode);
	if (FAILED(status) || dwRetCode) {
        const char* errorString = "An error occurred at DepositConfirmationId: ";
        int bufferSize = snprintf(nullptr, 0, "%s 0x%081X %lu", errorString, static_cast<unsigned int>(status), dwRetCode);
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%081X %lu", errorString, static_cast<unsigned int>(status), dwRetCode);

        int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        OLECHAR* wideResult = new OLECHAR[wideCharSize];
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

	BSTR installationID = NULL;
	HRESULT status = XP_LicenseAgent->GenerateInstallationId(&installationID);
	if (FAILED(status) || !installationID) {
        const char* errorString = "An error occurred at GenerateInstallationId: ";
        int bufferSize = snprintf(nullptr, 0, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        OLECHAR* wideResult = new OLECHAR[wideCharSize];
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
	std::string productKeyToRegex = std::string(ws.begin(), ws.end());
	std::regex pattern("[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}-[2346789BCDFGHJKMPQRTVWXY]{5}");

	if (!std::regex_match(productKeyToRegex, pattern)) {
		return SysAllocString(L"An error occurred at regex_match: Product key is invalid");
	}

	if (0 == wcscmp(XP_GetWPALeft(), L"An error occurred at GetWPALeft: Windows is activated")) {
		return SysAllocString(L"An error occurred at GetWPALeft: Windows is activated");
	}

	HRESULT status = XP_LicenseAgent->SetProductKey(productKey);
	if (FAILED(status)) {
        const char* errorString = "An error occurred at SetProductKey: ";
        int bufferSize = snprintf(nullptr, 0, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        OLECHAR* wideResult = new OLECHAR[wideCharSize];
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

	BSTR productID = NULL;

	HRESULT status = XP_LicenseAgent->GetProductID(&productID);
	if (FAILED(status)) {
        const char* errorString = "An error occurred at GetProductID: ";
        int bufferSize = snprintf(nullptr, 0, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        OLECHAR* wideResult = new OLECHAR[wideCharSize];
        MultiByteToWideChar(CP_UTF8, 0, result, -1, wideResult, wideCharSize);
        return SysAllocString(wideResult);
	}
	else {
		return productID;
	}
}
#pragma endregion

#pragma region Office 2003
static BSTR O2003_VerifyCheckDigits(BSTR cidChunk) {
	if (!O2003_LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at O2003_LoadLicenseManager: Failed to load");
	}

	// TODO: find a way to check office 2003 activation status

	int pbValue;
	HRESULT status = O2003_LicenseAgent->VerifyCheckDigits(cidChunk, &pbValue);
	if (FAILED(status) || !pbValue) {

        wchar_t error[] = L"An error occurred at XP_VerifyCheckDigits: ";
        std::wstring numberStr = std::to_wstring(pbValue);
        std::wstring result = error + numberStr;
        wchar_t buffer[100];

        swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%s 0x%X %d", error, status, pbValue);

		return SysAllocString(buffer);
	}
	return SysAllocString(L"Successfully verified CID chunk");
}

static BSTR O2003_SetConfirmationID(BSTR confirmationID) {
	if (!O2003_LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at O2003_LoadLicenseManager: Failed to load");
	}

	// TODO: find a way to check office 2003 activation status

	int length = SysStringLen(confirmationID);
	char* str = new char[length + 1];
	WideCharToMultiByte(CP_UTF8, 0, confirmationID, length, str, length, NULL, NULL);
	str[length] = '\0';

	std::string inputString(str);
	inputString.erase(std::remove(inputString.begin(), inputString.end(), '-'), inputString.end()); // remove -'s
	for (size_t i = 0; i < inputString.length(); i += 6) {
		std::string substring = inputString.substr(i, 6);
		const char* cstr = substring.c_str();
		if (0 != wcscmp(XP_VerifyCheckDigits(ConvertToBSTR(cstr)), L"Successfully verified CID chunk")) {
			return SysAllocString(L"An error occurred at XP_VerifyCheckDigits: Check for misspelling");
		}
	}

	ULONG dwRetCode;
	HRESULT status = O2003_LicenseAgent->DepositConfirmationId(confirmationID, &dwRetCode);
	if (FAILED(status) || dwRetCode) {
        const char* errorString = "An error occurred at DepositConfirmationId: ";
        int bufferSize = snprintf(nullptr, 0, "%s 0x%081X %lu", errorString, static_cast<unsigned int>(status), dwRetCode);
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%081X %lu", errorString, static_cast<unsigned int>(status), dwRetCode);
        int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        OLECHAR* wideResult = new OLECHAR[wideCharSize];
        MultiByteToWideChar(CP_UTF8, 0, result, -1, wideResult, wideCharSize);
        return SysAllocString(wideResult);
	}

	return SysAllocString(L"Successfully set confirmation ID");
}

static BSTR O2003_GetInstallationID() {
	if (!O2003_LoadLicenseManager()) {
		return SysAllocString(L"An error occurred at O2003_LoadLicenseManager: Failed to load");
	}
	else {
		std::cout << "DEBUG: LoadLicenseManager checked";
	}
 
	// TODO: find a way to check office 2003 activation status

	BSTR installationID = NULL;
	std::cout << "DEBUG: installationID = NULL";
	HRESULT status = O2003_LicenseAgent->GenerateInstallationId(&installationID);
	std::cout << "DEBUG: GenerateInstallationID executed";
	if (FAILED(status) || !installationID) {
        const char* errorString = "An error occurred at GenerateInstallationId: ";
        int bufferSize = snprintf(nullptr, 0, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        char* result = new char[bufferSize + 1];
        snprintf(result, bufferSize + 1, "%s 0x%081X", errorString, static_cast<unsigned int>(status));
        int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, result, -1, nullptr, 0);
        OLECHAR* wideResult = new OLECHAR[wideCharSize];
        MultiByteToWideChar(CP_UTF8, 0, result, -1, wideResult, wideCharSize);
        return SysAllocString(wideResult);
	}
	else {
		return installationID;
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
		"--GetInstallationID: Gets the Installation ID\n"
		"--SetConfirmationID [cid]: Sets Confirmation ID (If successful, activates Windows/Office)\n"
		"--GetWPALeft: Gets the days before activation is required (Grace period)\n"
		"--GetEvalLeft: Gets the days before the evaluation period expires (Eval. copies only)\n"
		"--SetProductKey [pkey]: Sets/changes product key\n"
		"--GetProductID: Gets the product ID of Windows\n"
		"--Office2003: Manages Office 2003 rather than Windows [EXPERIMENTAL] (32-bit xpmgr only)\n"
		"--GetUsage: Displays this message";

	if (cmdOptionExists(argv, argv + argc, "--GetUsage")) {
		std::cout << text;
		return 0;
	}

	if (cmdOptionExists(argv, argv + argc, "--Office2003")) {
#ifdef ENVIRONMENT32

        if (cmdOptionExists(argv, argv + argc, "--BypassInstallCheck")) {
            specifiedProduct = "Office2003";
        }
        else if (RegistryKeyExists(HKEY_CLASSES_ROOT, "CLSID\\{000C0114-0000-0000-C000-000000000046}")) {
			specifiedProduct = "Office2003";
		}
		else {
			std::cout << "An error occurred at RegistryKeyExists: Office 2003 isn't detected (specifically, it's COM Module, responsible for activation). Please (re)install Office 2003, since you wouldn't be able to activate via the GUI anyway.";
			return 0;
		}
	}
	else {
		specifiedProduct = "WindowsNT5x";
	}
#endif
#ifdef ENVIRONMENT64
		std::cout << "An error occurred at ENVIRONMENT64: Please switch to the x86 version of xpmgr to manage Office 2003.";
		return 0;
	}
#endif

#ifdef ENVIRONMENT64
	specifiedProduct = "WindowsNT5x";
#endif

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
				std::cout << "An error occurred at systemInfo: Incorrect version of xpmgr. Go to https://umskt.zulipchat.com if you want to help us make a version for your platform!";
			}
			return 0;
	}
#endif
		OSVERSIONINFOEX info;
		memset(&info, 0, sizeof(OSVERSIONINFOEX));
		info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		GetVersionEx((LPOSVERSIONINFO)&info);

		if (info.dwMajorVersion != 5) {
			std::cout << "An error occurred at OSVERSIONINFOEX: Windows management only works on Windows NT 5.1 and 5.2.";
			if (info.dwMajorVersion > 5) {
				std::cout << " Use slmgr instead.";
				return 0;
			}
			return 0;
		}
		else {
			if (info.dwMinorVersion != 1 && info.dwMinorVersion != 2) {
				if (info.dwMinorVersion == 0) {
					std::cout << " You should be fine anyways, since Windows 2000 doesn't use Product Activation.";
					return 0;
				}
				std::cout << "An error occurred OSVERSIONINFOEX: Windows management only works on Windows NT 5.1 and 5.2.";
				return 0;
			}
		}
	}

	if (cmdOptionExists(argv, argv + argc, "--GetInstallationID")) {
		if (std::strcmp(specifiedProduct, "Office2003") == 0) {
			std::cout << ConvertToStdString(O2003_GetInstallationID());
			return 0;
		}
		std::cout << ConvertToStdString(XP_GetInstallationID());
		return 0;
	}
	else if (cmdOptionExists(argv, argv + argc, "--SetConfirmationID")) {
		if (std::strcmp(specifiedProduct, "Office2003") == 0) {
			std::cout << ConvertToStdString(O2003_SetConfirmationID(ConvertCharToBSTR(getCmdOption(argv, argv + argc, "--SetConfirmationID"))));
			return 0;
		}
		std::cout << ConvertToStdString(XP_SetConfirmationID(ConvertCharToBSTR(getCmdOption(argv, argv + argc, "--SetConfirmationID"))));
		return 0;
	}

	else if (cmdOptionExists(argv, argv + argc, "--GetWPALeft")) {
		if (std::strcmp(specifiedProduct, "WindowsNT5x") != 0) {
			std::cout << "An error occurred at specifiedProduct: This command is for Windows management only.";
			return 0;
		}
		std::cout << ConvertToStdString(XP_GetWPALeft());
		return 0;
	}
	else if (cmdOptionExists(argv, argv + argc, "--GetEvalLeft")) {
		if (std::strcmp(specifiedProduct, "WindowsNT5x") != 0) {
			std::cout << "An error occurred at specifiedProduct: This command is for Windows management only.";
			return 0;
		}
		std::cout << ConvertToStdString(XP_GetEvalLeft());
		return 0;
	}

	else if (cmdOptionExists(argv, argv + argc, "--SetProductKey")) {
		if (std::strcmp(specifiedProduct, "WindowsNT5x") != 0) {
			std::cout << "An error occurred at specifiedProduct: This command is for Windows management only.";
			return 0;
		}
		std::cout << ConvertToStdString(XP_SetProductKey(convertCharArrayToLPCWSTR(getCmdOption(argv, argv + argc, "--SetProductKey"))));
		return 0;
	}
	else if (cmdOptionExists(argv, argv + argc, "--GetProductID")) {
		if (std::strcmp(specifiedProduct, "WindowsNT5x") != 0) {
			std::cout << "An error occurred at specifiedProduct: This command is for Windows management only.";
			return 0;
		}
		std::cout << ConvertToStdString(XP_GetProductID());
		return 0;
	}
	else {
		std::cout << "An error occurred at main: No arguments listed\n\n";
		std::cout << text;
		return 0;
	}
}
#pragma clang diagnostic pop