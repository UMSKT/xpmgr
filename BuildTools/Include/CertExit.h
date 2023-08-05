

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* Compiler settings for certexit.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __certexit_h__
#define __certexit_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICertExit_FWD_DEFINED__
#define __ICertExit_FWD_DEFINED__
typedef interface ICertExit ICertExit;
#endif 	/* __ICertExit_FWD_DEFINED__ */


#ifndef __ICertExit2_FWD_DEFINED__
#define __ICertExit2_FWD_DEFINED__
typedef interface ICertExit2 ICertExit2;
#endif 	/* __ICertExit2_FWD_DEFINED__ */


/* header files for imported files */
#include "wtypes.h"
#include "certmod.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_certexit_0000_0000 */
/* [local] */ 

#define EXITEVENT_INVALID               ( 0x0 )  // Invalid event
#define EXITEVENT_CERTISSUED            ( 0x1 )  // Certificate issued
#define EXITEVENT_CERTPENDING           ( 0x2 )  // Certificate pending
#define EXITEVENT_CERTDENIED            ( 0x4 )  // Certificate denied
#define EXITEVENT_CERTREVOKED           ( 0x8 )  // Certificate revoked
#define EXITEVENT_CERTRETRIEVEPENDING   ( 0x10 ) // Certificate retrieval
#define EXITEVENT_CRLISSUED             ( 0x20 ) // CRL issued
#define EXITEVENT_SHUTDOWN              ( 0x40 ) // Service shutdown
#define EXITEVENT_STARTUP               ( 0x80 ) // Service startup, fake event generated by exit module
#define EXITEVENT_CERTIMPORTED          ( 0x200 ) // Certificate Imported


extern RPC_IF_HANDLE __MIDL_itf_certexit_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_certexit_0000_0000_v0_0_s_ifspec;

#ifndef __ICertExit_INTERFACE_DEFINED__
#define __ICertExit_INTERFACE_DEFINED__

/* interface ICertExit */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICertExit;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e19ae1a0-7364-11d0-8816-00a0c903b83c")
    ICertExit : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ __RPC__in const BSTR strConfig,
            /* [retval][out] */ __RPC__out LONG *pEventMask) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Notify( 
            /* [in] */ LONG ExitEvent,
            /* [in] */ LONG Context) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDescription( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pstrDescription) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICertExitVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in ICertExit * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in ICertExit * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in ICertExit * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            __RPC__in ICertExit * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            __RPC__in ICertExit * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            __RPC__in ICertExit * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ __RPC__in_range(0,16384) UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICertExit * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            __RPC__in ICertExit * This,
            /* [in] */ __RPC__in const BSTR strConfig,
            /* [retval][out] */ __RPC__out LONG *pEventMask);
        
        HRESULT ( STDMETHODCALLTYPE *Notify )( 
            __RPC__in ICertExit * This,
            /* [in] */ LONG ExitEvent,
            /* [in] */ LONG Context);
        
        HRESULT ( STDMETHODCALLTYPE *GetDescription )( 
            __RPC__in ICertExit * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pstrDescription);
        
        END_INTERFACE
    } ICertExitVtbl;

    interface ICertExit
    {
        CONST_VTBL struct ICertExitVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICertExit_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICertExit_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICertExit_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICertExit_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICertExit_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICertExit_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICertExit_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICertExit_Initialize(This,strConfig,pEventMask)	\
    ( (This)->lpVtbl -> Initialize(This,strConfig,pEventMask) ) 

#define ICertExit_Notify(This,ExitEvent,Context)	\
    ( (This)->lpVtbl -> Notify(This,ExitEvent,Context) ) 

#define ICertExit_GetDescription(This,pstrDescription)	\
    ( (This)->lpVtbl -> GetDescription(This,pstrDescription) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICertExit_INTERFACE_DEFINED__ */


#ifndef __ICertExit2_INTERFACE_DEFINED__
#define __ICertExit2_INTERFACE_DEFINED__

/* interface ICertExit2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICertExit2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0abf484b-d049-464d-a7ed-552e7529b0ff")
    ICertExit2 : public ICertExit
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetManageModule( 
            /* [retval][out] */ __RPC__deref_out_opt ICertManageModule **ppManageModule) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICertExit2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in ICertExit2 * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in ICertExit2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in ICertExit2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            __RPC__in ICertExit2 * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            __RPC__in ICertExit2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            __RPC__in ICertExit2 * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ __RPC__in_range(0,16384) UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICertExit2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            __RPC__in ICertExit2 * This,
            /* [in] */ __RPC__in const BSTR strConfig,
            /* [retval][out] */ __RPC__out LONG *pEventMask);
        
        HRESULT ( STDMETHODCALLTYPE *Notify )( 
            __RPC__in ICertExit2 * This,
            /* [in] */ LONG ExitEvent,
            /* [in] */ LONG Context);
        
        HRESULT ( STDMETHODCALLTYPE *GetDescription )( 
            __RPC__in ICertExit2 * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pstrDescription);
        
        HRESULT ( STDMETHODCALLTYPE *GetManageModule )( 
            __RPC__in ICertExit2 * This,
            /* [retval][out] */ __RPC__deref_out_opt ICertManageModule **ppManageModule);
        
        END_INTERFACE
    } ICertExit2Vtbl;

    interface ICertExit2
    {
        CONST_VTBL struct ICertExit2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICertExit2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICertExit2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICertExit2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICertExit2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICertExit2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICertExit2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICertExit2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICertExit2_Initialize(This,strConfig,pEventMask)	\
    ( (This)->lpVtbl -> Initialize(This,strConfig,pEventMask) ) 

#define ICertExit2_Notify(This,ExitEvent,Context)	\
    ( (This)->lpVtbl -> Notify(This,ExitEvent,Context) ) 

#define ICertExit2_GetDescription(This,pstrDescription)	\
    ( (This)->lpVtbl -> GetDescription(This,pstrDescription) ) 


#define ICertExit2_GetManageModule(This,ppManageModule)	\
    ( (This)->lpVtbl -> GetManageModule(This,ppManageModule) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICertExit2_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     __RPC__in unsigned long *, unsigned long            , __RPC__in BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  __RPC__in unsigned long *, __RPC__inout_xcount(0) unsigned char *, __RPC__in BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(__RPC__in unsigned long *, __RPC__in_xcount(0) unsigned char *, __RPC__out BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     __RPC__in unsigned long *, __RPC__in BSTR * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     __RPC__in unsigned long *, unsigned long            , __RPC__in BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  __RPC__in unsigned long *, __RPC__inout_xcount(0) unsigned char *, __RPC__in BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(__RPC__in unsigned long *, __RPC__in_xcount(0) unsigned char *, __RPC__out BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     __RPC__in unsigned long *, __RPC__in BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



