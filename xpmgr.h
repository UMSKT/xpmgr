#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>

// Core functions that match command line options
const wchar_t* XPMGR_GetInstallationID();  // /dti
const wchar_t* XPMGR_SetConfirmationID(const wchar_t* confirmationId);  // /atp
const wchar_t* XPMGR_GetWPALeft();  // /xpr
const wchar_t* XPMGR_GetEvalLeft();  // /xpr-eval
const wchar_t* XPMGR_SetProductKey(const wchar_t* productKey);  // /ipk
const wchar_t* XPMGR_GetProductID();  // /dli

// Additional utility functions
const wchar_t* XPMGR_GetLastError();
void XPMGR_Initialize();
void XPMGR_Cleanup();

#ifdef __cplusplus
}
#endif 