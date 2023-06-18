#define STRICT
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <commctrl.h>
#include <stdint.h>
#include <intrin.h>
#include <iostream>
#include <comutil.h>

#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "kernel32.lib")

// The magic numbers, interface, and LoadLicenseManager() function was made by diamondggg on MyDigitalLife.
// LoadLicenseManager() was modified to suit a CLI rather than a GUI.
// Everything else was made by TheTank20.
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
	ULONG dwWPALeft = 0, dwEvalLeft = 0;
	HRESULT status = LicenseAgent->GetExpirationInfo(&dwWPALeft, &dwEvalLeft);
	if (FAILED(status)) {
		std::cout << "An error occurred at GetExpirationInfo: " + status;
		LicenseAgent->Release();
		LicenseAgent = NULL;
		return FALSE;
	}
	if (dwWPALeft == 0x7FFFFFFF) {
		std::cout << "An error occurred: dwWPALeft is 0x7FFFFFFF";
		LicenseAgent->Release();
		LicenseAgent = NULL;
		return FALSE;
	}
	return TRUE;
}

static BSTR SetConfirmationID(BSTR confirmationID) {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"Failed to load license manager");
	}

	ULONG dwRetCode;
	HRESULT status = LicenseAgent->DepositConfirmationId(confirmationID, &dwRetCode);
	if (FAILED(status) || dwRetCode) {
		std::cout << "An error occurred at DepositConfirmationId: " + status + dwRetCode;
		return SysAllocString(L"Failed to set confirmation ID");
	}
	return SysAllocString(L"Successfully set confirmation ID");
}

static BSTR GetInstallationID() {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"Failed to load license manager");
	}

	BSTR installationID = NULL;
	HRESULT status = LicenseAgent->GenerateInstallationId(&installationID);
	if (FAILED(status) || !installationID) {
		std::cout << "An error occurred at GenerateInstallationId: " + status;
		return SysAllocString(L"Failed to get installation ID");
	}
	else {
		return installationID;
	}
}

static BSTR SetProductKey(LPWSTR productKey) {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"Failed to load license manager");
	}

	HRESULT status = LicenseAgent->SetProductKey(productKey);
	if (FAILED(status)) {
		std::cout << "An error occurred at SetProductKey: " + status;
		return SysAllocString(L"Failed to set product key");
	}
	else {
		return SysAllocString(L"Successfully set product key");
	}
}

static BSTR GetProductID() {
	if (!LoadLicenseManager()) {
		return SysAllocString(L"Failed to load license manager");
	}
	
	BSTR productID = NULL;

	HRESULT status = LicenseAgent->GetProductID(&productID);
	if (FAILED(status)) {
		std::cout << "An error occurred at GetProductID: " + status;
		return SysAllocString(L"Failed to get product ID");
	}
	else {
		return productID;
	}
}

wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

int main(int argc, char* argv[])
{
	if (std::string(argv[1]) == "--GetInstallationID") {
		std::cout << _com_util::ConvertBSTRToString(GetInstallationID());
		return 0;
	}
	else if (std::string(argv[1]) == "--SetConfirmationID") {
		std::cout << _com_util::ConvertBSTRToString(SetConfirmationID(_com_util::ConvertStringToBSTR(argv[2])));
		return 0;
	}
	else if (std::string(argv[1]) == "--SetProductKey") {
		std::cout << _com_util::ConvertBSTRToString(SetProductKey(convertCharArrayToLPCWSTR(argv[2])));
		return 0;
	}
	else if (std::string(argv[1]) == "--GetProductID") {
		std::cout << _com_util::ConvertBSTRToString(GetProductID());
	}
	else {
		std::cout << "Please put in arguments";
		return 0;
	}
}