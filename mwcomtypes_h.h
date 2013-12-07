

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Thu Jun 13 10:48:49 2013
 */
/* Compiler settings for Source\matlab_engine\mwcomtypes.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
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
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef __mwcomtypes_h_h__
#define __mwcomtypes_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IObjectInfo_FWD_DEFINED__
#define __IObjectInfo_FWD_DEFINED__
typedef interface IObjectInfo IObjectInfo;

#endif 	/* __IObjectInfo_FWD_DEFINED__ */


#ifndef __IMWArrayFormatFlags_FWD_DEFINED__
#define __IMWArrayFormatFlags_FWD_DEFINED__
typedef interface IMWArrayFormatFlags IMWArrayFormatFlags;

#endif 	/* __IMWArrayFormatFlags_FWD_DEFINED__ */


#ifndef __IMWDataConversionFlags_FWD_DEFINED__
#define __IMWDataConversionFlags_FWD_DEFINED__
typedef interface IMWDataConversionFlags IMWDataConversionFlags;

#endif 	/* __IMWDataConversionFlags_FWD_DEFINED__ */


#ifndef __IMWFlags_FWD_DEFINED__
#define __IMWFlags_FWD_DEFINED__
typedef interface IMWFlags IMWFlags;

#endif 	/* __IMWFlags_FWD_DEFINED__ */


#ifndef __IMWField_FWD_DEFINED__
#define __IMWField_FWD_DEFINED__
typedef interface IMWField IMWField;

#endif 	/* __IMWField_FWD_DEFINED__ */


#ifndef __IMWStruct_FWD_DEFINED__
#define __IMWStruct_FWD_DEFINED__
typedef interface IMWStruct IMWStruct;

#endif 	/* __IMWStruct_FWD_DEFINED__ */


#ifndef __IMWComplex_FWD_DEFINED__
#define __IMWComplex_FWD_DEFINED__
typedef interface IMWComplex IMWComplex;

#endif 	/* __IMWComplex_FWD_DEFINED__ */


#ifndef __IMWSparse_FWD_DEFINED__
#define __IMWSparse_FWD_DEFINED__
typedef interface IMWSparse IMWSparse;

#endif 	/* __IMWSparse_FWD_DEFINED__ */


#ifndef __IMWArg_FWD_DEFINED__
#define __IMWArg_FWD_DEFINED__
typedef interface IMWArg IMWArg;

#endif 	/* __IMWArg_FWD_DEFINED__ */


#ifndef __IMWEnumVararg_FWD_DEFINED__
#define __IMWEnumVararg_FWD_DEFINED__
typedef interface IMWEnumVararg IMWEnumVararg;

#endif 	/* __IMWEnumVararg_FWD_DEFINED__ */


#ifndef __IMWMethodArgInfo_FWD_DEFINED__
#define __IMWMethodArgInfo_FWD_DEFINED__
typedef interface IMWMethodArgInfo IMWMethodArgInfo;

#endif 	/* __IMWMethodArgInfo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_mwcomtypes_0000_0000 */
/* [local] */ 

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("9E8F9403-92FE-4ab2-B133-C15107442932") 
enum mwArrayFormat
    {
        mwArrayFormatAsIs	= 0,
        mwArrayFormatMatrix	= 1,
        mwArrayFormatCell	= 2
    } 	mwArrayFormat;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("E47B19A6-F1A5-4ec5-A517-BAD4413BC880") 
enum mwDataType
    {
        mwTypeDefault	= 0,
        mwTypeLogical	= 3,
        mwTypeChar	= 4,
        mwTypeDouble	= 6,
        mwTypeSingle	= 7,
        mwTypeInt8	= 8,
        mwTypeUint8	= 9,
        mwTypeInt16	= 10,
        mwTypeUint16	= 11,
        mwTypeInt32	= 12,
        mwTypeUint32	= 13
    } 	mwDataType;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("A2B29D15-C4D3-4d4b-BC7E-92A22873491E") 
enum mwDateFormat
    {
        mwDateFormatNumeric	= 0,
        mwDateFormatString	= 1
    } 	mwDateFormat;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("9FF4FA38-500F-47ba-8F89-F7E3500A26DC") 
enum mwReplaceMissingData
    {
        mwReplaceZero	= 0,
        mwReplaceNaN	= 1
    } 	mwReplaceMissingData;



extern RPC_IF_HANDLE __MIDL_itf_mwcomtypes_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mwcomtypes_0000_0000_v0_0_s_ifspec;

#ifndef __IObjectInfo_INTERFACE_DEFINED__
#define __IObjectInfo_INTERFACE_DEFINED__

/* interface IObjectInfo */
/* [unique][helpstring][hidden][uuid][object] */ 


EXTERN_C const IID IID_IObjectInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("92EA75F7-994E-4925-8C88-2F9C56F87756")
    IObjectInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetIsRange( 
            /* [in] */ long n,
            /* [out] */ VARIANT_BOOL *pbIsRange) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIsObject( 
            /* [in] */ long n,
            /* [out] */ VARIANT_BOOL *pbIsObject) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IObjectInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IObjectInfo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IObjectInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IObjectInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIsRange )( 
            IObjectInfo * This,
            /* [in] */ long n,
            /* [out] */ VARIANT_BOOL *pbIsRange);
        
        HRESULT ( STDMETHODCALLTYPE *GetIsObject )( 
            IObjectInfo * This,
            /* [in] */ long n,
            /* [out] */ VARIANT_BOOL *pbIsObject);
        
        END_INTERFACE
    } IObjectInfoVtbl;

    interface IObjectInfo
    {
        CONST_VTBL struct IObjectInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjectInfo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IObjectInfo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IObjectInfo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IObjectInfo_GetIsRange(This,n,pbIsRange)	\
    ( (This)->lpVtbl -> GetIsRange(This,n,pbIsRange) ) 

#define IObjectInfo_GetIsObject(This,n,pbIsObject)	\
    ( (This)->lpVtbl -> GetIsObject(This,n,pbIsObject) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IObjectInfo_INTERFACE_DEFINED__ */


#ifndef __IMWArrayFormatFlags_INTERFACE_DEFINED__
#define __IMWArrayFormatFlags_INTERFACE_DEFINED__

/* interface IMWArrayFormatFlags */
/* [unique][helpstring][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_IMWArrayFormatFlags;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("30C8EBCB-1A50-4dee-A5E8-0C6F7DD52D4C")
    IMWArrayFormatFlags : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_InputArrayFormat( 
            /* [retval][out] */ mwArrayFormat *pInputFmt) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_InputArrayFormat( 
            /* [in] */ mwArrayFormat InputFmt) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_InputArrayIndFlag( 
            /* [retval][out] */ long *pnInputInd) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_InputArrayIndFlag( 
            /* [in] */ long nInputInd) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_OutputArrayFormat( 
            /* [retval][out] */ mwArrayFormat *pOutputFmt) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_OutputArrayFormat( 
            /* [in] */ mwArrayFormat OutputFmt) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_OutputArrayIndFlag( 
            /* [retval][out] */ long *pnOutputInd) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_OutputArrayIndFlag( 
            /* [in] */ long nOutputInd) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AutoResizeOutput( 
            /* [retval][out] */ VARIANT_BOOL *pbResize) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AutoResizeOutput( 
            /* [in] */ VARIANT_BOOL bResize) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TransposeOutput( 
            /* [retval][out] */ VARIANT_BOOL *pbTranspose) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TransposeOutput( 
            /* [in] */ VARIANT_BOOL bTranspose) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMWArrayFormatFlagsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMWArrayFormatFlags * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMWArrayFormatFlags * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMWArrayFormatFlags * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMWArrayFormatFlags * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMWArrayFormatFlags * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMWArrayFormatFlags * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMWArrayFormatFlags * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InputArrayFormat )( 
            IMWArrayFormatFlags * This,
            /* [retval][out] */ mwArrayFormat *pInputFmt);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_InputArrayFormat )( 
            IMWArrayFormatFlags * This,
            /* [in] */ mwArrayFormat InputFmt);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InputArrayIndFlag )( 
            IMWArrayFormatFlags * This,
            /* [retval][out] */ long *pnInputInd);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_InputArrayIndFlag )( 
            IMWArrayFormatFlags * This,
            /* [in] */ long nInputInd);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OutputArrayFormat )( 
            IMWArrayFormatFlags * This,
            /* [retval][out] */ mwArrayFormat *pOutputFmt);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_OutputArrayFormat )( 
            IMWArrayFormatFlags * This,
            /* [in] */ mwArrayFormat OutputFmt);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OutputArrayIndFlag )( 
            IMWArrayFormatFlags * This,
            /* [retval][out] */ long *pnOutputInd);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_OutputArrayIndFlag )( 
            IMWArrayFormatFlags * This,
            /* [in] */ long nOutputInd);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AutoResizeOutput )( 
            IMWArrayFormatFlags * This,
            /* [retval][out] */ VARIANT_BOOL *pbResize);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AutoResizeOutput )( 
            IMWArrayFormatFlags * This,
            /* [in] */ VARIANT_BOOL bResize);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TransposeOutput )( 
            IMWArrayFormatFlags * This,
            /* [retval][out] */ VARIANT_BOOL *pbTranspose);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TransposeOutput )( 
            IMWArrayFormatFlags * This,
            /* [in] */ VARIANT_BOOL bTranspose);
        
        END_INTERFACE
    } IMWArrayFormatFlagsVtbl;

    interface IMWArrayFormatFlags
    {
        CONST_VTBL struct IMWArrayFormatFlagsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMWArrayFormatFlags_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMWArrayFormatFlags_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMWArrayFormatFlags_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMWArrayFormatFlags_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMWArrayFormatFlags_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMWArrayFormatFlags_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMWArrayFormatFlags_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMWArrayFormatFlags_get_InputArrayFormat(This,pInputFmt)	\
    ( (This)->lpVtbl -> get_InputArrayFormat(This,pInputFmt) ) 

#define IMWArrayFormatFlags_put_InputArrayFormat(This,InputFmt)	\
    ( (This)->lpVtbl -> put_InputArrayFormat(This,InputFmt) ) 

#define IMWArrayFormatFlags_get_InputArrayIndFlag(This,pnInputInd)	\
    ( (This)->lpVtbl -> get_InputArrayIndFlag(This,pnInputInd) ) 

#define IMWArrayFormatFlags_put_InputArrayIndFlag(This,nInputInd)	\
    ( (This)->lpVtbl -> put_InputArrayIndFlag(This,nInputInd) ) 

#define IMWArrayFormatFlags_get_OutputArrayFormat(This,pOutputFmt)	\
    ( (This)->lpVtbl -> get_OutputArrayFormat(This,pOutputFmt) ) 

#define IMWArrayFormatFlags_put_OutputArrayFormat(This,OutputFmt)	\
    ( (This)->lpVtbl -> put_OutputArrayFormat(This,OutputFmt) ) 

#define IMWArrayFormatFlags_get_OutputArrayIndFlag(This,pnOutputInd)	\
    ( (This)->lpVtbl -> get_OutputArrayIndFlag(This,pnOutputInd) ) 

#define IMWArrayFormatFlags_put_OutputArrayIndFlag(This,nOutputInd)	\
    ( (This)->lpVtbl -> put_OutputArrayIndFlag(This,nOutputInd) ) 

#define IMWArrayFormatFlags_get_AutoResizeOutput(This,pbResize)	\
    ( (This)->lpVtbl -> get_AutoResizeOutput(This,pbResize) ) 

#define IMWArrayFormatFlags_put_AutoResizeOutput(This,bResize)	\
    ( (This)->lpVtbl -> put_AutoResizeOutput(This,bResize) ) 

#define IMWArrayFormatFlags_get_TransposeOutput(This,pbTranspose)	\
    ( (This)->lpVtbl -> get_TransposeOutput(This,pbTranspose) ) 

#define IMWArrayFormatFlags_put_TransposeOutput(This,bTranspose)	\
    ( (This)->lpVtbl -> put_TransposeOutput(This,bTranspose) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMWArrayFormatFlags_INTERFACE_DEFINED__ */


#ifndef __IMWDataConversionFlags_INTERFACE_DEFINED__
#define __IMWDataConversionFlags_INTERFACE_DEFINED__

/* interface IMWDataConversionFlags */
/* [unique][helpstring][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_IMWDataConversionFlags;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5E0BBECB-CE72-4461-ADB8-0446AB32CF51")
    IMWDataConversionFlags : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CoerceNumericToType( 
            /* [retval][out] */ mwDataType *pType) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CoerceNumericToType( 
            /* [in] */ mwDataType Type) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_InputDateFormat( 
            /* [retval][out] */ mwDateFormat *pDateFmt) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_InputDateFormat( 
            /* [in] */ mwDateFormat DateFmt) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_OutputAsDate( 
            /* [retval][out] */ VARIANT_BOOL *pbDate) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_OutputAsDate( 
            /* [in] */ VARIANT_BOOL bDate) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DateBias( 
            /* [retval][out] */ long *pnBias) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DateBias( 
            /* [in] */ long nBias) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ReplaceMissing( 
            /* [retval][out] */ mwReplaceMissingData *pnReplace) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ReplaceMissing( 
            /* [in] */ mwReplaceMissingData nReplace) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMWDataConversionFlagsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMWDataConversionFlags * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMWDataConversionFlags * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMWDataConversionFlags * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMWDataConversionFlags * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMWDataConversionFlags * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMWDataConversionFlags * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMWDataConversionFlags * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CoerceNumericToType )( 
            IMWDataConversionFlags * This,
            /* [retval][out] */ mwDataType *pType);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CoerceNumericToType )( 
            IMWDataConversionFlags * This,
            /* [in] */ mwDataType Type);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InputDateFormat )( 
            IMWDataConversionFlags * This,
            /* [retval][out] */ mwDateFormat *pDateFmt);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_InputDateFormat )( 
            IMWDataConversionFlags * This,
            /* [in] */ mwDateFormat DateFmt);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OutputAsDate )( 
            IMWDataConversionFlags * This,
            /* [retval][out] */ VARIANT_BOOL *pbDate);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_OutputAsDate )( 
            IMWDataConversionFlags * This,
            /* [in] */ VARIANT_BOOL bDate);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DateBias )( 
            IMWDataConversionFlags * This,
            /* [retval][out] */ long *pnBias);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DateBias )( 
            IMWDataConversionFlags * This,
            /* [in] */ long nBias);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ReplaceMissing )( 
            IMWDataConversionFlags * This,
            /* [retval][out] */ mwReplaceMissingData *pnReplace);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ReplaceMissing )( 
            IMWDataConversionFlags * This,
            /* [in] */ mwReplaceMissingData nReplace);
        
        END_INTERFACE
    } IMWDataConversionFlagsVtbl;

    interface IMWDataConversionFlags
    {
        CONST_VTBL struct IMWDataConversionFlagsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMWDataConversionFlags_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMWDataConversionFlags_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMWDataConversionFlags_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMWDataConversionFlags_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMWDataConversionFlags_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMWDataConversionFlags_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMWDataConversionFlags_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMWDataConversionFlags_get_CoerceNumericToType(This,pType)	\
    ( (This)->lpVtbl -> get_CoerceNumericToType(This,pType) ) 

#define IMWDataConversionFlags_put_CoerceNumericToType(This,Type)	\
    ( (This)->lpVtbl -> put_CoerceNumericToType(This,Type) ) 

#define IMWDataConversionFlags_get_InputDateFormat(This,pDateFmt)	\
    ( (This)->lpVtbl -> get_InputDateFormat(This,pDateFmt) ) 

#define IMWDataConversionFlags_put_InputDateFormat(This,DateFmt)	\
    ( (This)->lpVtbl -> put_InputDateFormat(This,DateFmt) ) 

#define IMWDataConversionFlags_get_OutputAsDate(This,pbDate)	\
    ( (This)->lpVtbl -> get_OutputAsDate(This,pbDate) ) 

#define IMWDataConversionFlags_put_OutputAsDate(This,bDate)	\
    ( (This)->lpVtbl -> put_OutputAsDate(This,bDate) ) 

#define IMWDataConversionFlags_get_DateBias(This,pnBias)	\
    ( (This)->lpVtbl -> get_DateBias(This,pnBias) ) 

#define IMWDataConversionFlags_put_DateBias(This,nBias)	\
    ( (This)->lpVtbl -> put_DateBias(This,nBias) ) 

#define IMWDataConversionFlags_get_ReplaceMissing(This,pnReplace)	\
    ( (This)->lpVtbl -> get_ReplaceMissing(This,pnReplace) ) 

#define IMWDataConversionFlags_put_ReplaceMissing(This,nReplace)	\
    ( (This)->lpVtbl -> put_ReplaceMissing(This,nReplace) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMWDataConversionFlags_INTERFACE_DEFINED__ */


#ifndef __IMWFlags_INTERFACE_DEFINED__
#define __IMWFlags_INTERFACE_DEFINED__

/* interface IMWFlags */
/* [unique][helpstring][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_IMWFlags;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0A295776-23A1-410a-94BD-0C6C61B891B7")
    IMWFlags : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ArrayFormatFlags( 
            /* [retval][out] */ IMWArrayFormatFlags **ppFlags) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ArrayFormatFlags( 
            /* [in] */ IMWArrayFormatFlags *pFlags) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DataConversionFlags( 
            /* [retval][out] */ IMWDataConversionFlags **ppFlags) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DataConversionFlags( 
            /* [in] */ IMWDataConversionFlags *pFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clone( 
            IMWFlags **ppFlags) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMWFlagsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMWFlags * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMWFlags * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMWFlags * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMWFlags * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMWFlags * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMWFlags * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMWFlags * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ArrayFormatFlags )( 
            IMWFlags * This,
            /* [retval][out] */ IMWArrayFormatFlags **ppFlags);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ArrayFormatFlags )( 
            IMWFlags * This,
            /* [in] */ IMWArrayFormatFlags *pFlags);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DataConversionFlags )( 
            IMWFlags * This,
            /* [retval][out] */ IMWDataConversionFlags **ppFlags);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DataConversionFlags )( 
            IMWFlags * This,
            /* [in] */ IMWDataConversionFlags *pFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IMWFlags * This,
            IMWFlags **ppFlags);
        
        END_INTERFACE
    } IMWFlagsVtbl;

    interface IMWFlags
    {
        CONST_VTBL struct IMWFlagsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMWFlags_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMWFlags_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMWFlags_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMWFlags_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMWFlags_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMWFlags_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMWFlags_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMWFlags_get_ArrayFormatFlags(This,ppFlags)	\
    ( (This)->lpVtbl -> get_ArrayFormatFlags(This,ppFlags) ) 

#define IMWFlags_put_ArrayFormatFlags(This,pFlags)	\
    ( (This)->lpVtbl -> put_ArrayFormatFlags(This,pFlags) ) 

#define IMWFlags_get_DataConversionFlags(This,ppFlags)	\
    ( (This)->lpVtbl -> get_DataConversionFlags(This,ppFlags) ) 

#define IMWFlags_put_DataConversionFlags(This,pFlags)	\
    ( (This)->lpVtbl -> put_DataConversionFlags(This,pFlags) ) 

#define IMWFlags_Clone(This,ppFlags)	\
    ( (This)->lpVtbl -> Clone(This,ppFlags) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMWFlags_INTERFACE_DEFINED__ */


#ifndef __IMWField_INTERFACE_DEFINED__
#define __IMWField_INTERFACE_DEFINED__

/* interface IMWField */
/* [unique][helpstring][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_IMWField;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0AEC9FFB-AB0D-4a09-BE9C-78857A1B8580")
    IMWField : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [retval][out] */ VARIANT *pvarValue) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ VARIANT varValue) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MWFlags( 
            /* [retval][out] */ IMWFlags **ppFlags) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MWFlags( 
            /* [in] */ IMWFlags *pFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IMWField **ppField) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMWFieldVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMWField * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMWField * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMWField * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMWField * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMWField * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMWField * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMWField * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            IMWField * This,
            /* [retval][out] */ VARIANT *pvarValue);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            IMWField * This,
            /* [in] */ VARIANT varValue);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IMWField * This,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MWFlags )( 
            IMWField * This,
            /* [retval][out] */ IMWFlags **ppFlags);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MWFlags )( 
            IMWField * This,
            /* [in] */ IMWFlags *pFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IMWField * This,
            /* [out] */ IMWField **ppField);
        
        END_INTERFACE
    } IMWFieldVtbl;

    interface IMWField
    {
        CONST_VTBL struct IMWFieldVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMWField_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMWField_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMWField_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMWField_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMWField_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMWField_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMWField_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMWField_get_Value(This,pvarValue)	\
    ( (This)->lpVtbl -> get_Value(This,pvarValue) ) 

#define IMWField_put_Value(This,varValue)	\
    ( (This)->lpVtbl -> put_Value(This,varValue) ) 

#define IMWField_get_Name(This,pbstrName)	\
    ( (This)->lpVtbl -> get_Name(This,pbstrName) ) 

#define IMWField_get_MWFlags(This,ppFlags)	\
    ( (This)->lpVtbl -> get_MWFlags(This,ppFlags) ) 

#define IMWField_put_MWFlags(This,pFlags)	\
    ( (This)->lpVtbl -> put_MWFlags(This,pFlags) ) 

#define IMWField_Clone(This,ppField)	\
    ( (This)->lpVtbl -> Clone(This,ppField) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMWField_INTERFACE_DEFINED__ */


#ifndef __IMWStruct_INTERFACE_DEFINED__
#define __IMWStruct_INTERFACE_DEFINED__

/* interface IMWStruct */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMWStruct;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6C70BB67-3F76-4176-8809-C66393B1FA23")
    IMWStruct : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [optional][in] */ VARIANT i0,
            /* [optional][in] */ VARIANT i1,
            /* [optional][in] */ VARIANT i2,
            /* [optional][in] */ VARIANT i3,
            /* [optional][in] */ VARIANT i4,
            /* [optional][in] */ VARIANT i5,
            /* [optional][in] */ VARIANT i6,
            /* [optional][in] */ VARIANT i7,
            /* [optional][in] */ VARIANT i8,
            /* [optional][in] */ VARIANT i9,
            /* [optional][in] */ VARIANT i10,
            /* [optional][in] */ VARIANT i11,
            /* [optional][in] */ VARIANT i12,
            /* [optional][in] */ VARIANT i13,
            /* [optional][in] */ VARIANT i14,
            /* [optional][in] */ VARIANT i15,
            /* [optional][in] */ VARIANT i16,
            /* [optional][in] */ VARIANT i17,
            /* [optional][in] */ VARIANT i18,
            /* [optional][in] */ VARIANT i19,
            /* [optional][in] */ VARIANT i20,
            /* [optional][in] */ VARIANT i21,
            /* [optional][in] */ VARIANT i22,
            /* [optional][in] */ VARIANT i23,
            /* [optional][in] */ VARIANT i24,
            /* [optional][in] */ VARIANT i25,
            /* [optional][in] */ VARIANT i26,
            /* [optional][in] */ VARIANT i27,
            /* [optional][in] */ VARIANT i28,
            /* [optional][in] */ VARIANT i29,
            /* [optional][in] */ VARIANT i30,
            /* [optional][in] */ VARIANT i31,
            /* [retval][out] */ IMWField **ppField) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [optional][in] */ VARIANT varDims,
            /* [optional][in] */ VARIANT varFieldNames) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumberOfFields( 
            /* [retval][out] */ long *pnFields) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumberOfDims( 
            /* [retval][out] */ long *pnDims) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Dims( 
            /* [retval][out] */ VARIANT *pvarDims) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FieldNames( 
            /* [retval][out] */ VARIANT *pvarFieldNames) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IMWStruct **ppStruct) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMWStructVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMWStruct * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMWStruct * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMWStruct * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMWStruct * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMWStruct * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMWStruct * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMWStruct * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IMWStruct * This,
            /* [optional][in] */ VARIANT i0,
            /* [optional][in] */ VARIANT i1,
            /* [optional][in] */ VARIANT i2,
            /* [optional][in] */ VARIANT i3,
            /* [optional][in] */ VARIANT i4,
            /* [optional][in] */ VARIANT i5,
            /* [optional][in] */ VARIANT i6,
            /* [optional][in] */ VARIANT i7,
            /* [optional][in] */ VARIANT i8,
            /* [optional][in] */ VARIANT i9,
            /* [optional][in] */ VARIANT i10,
            /* [optional][in] */ VARIANT i11,
            /* [optional][in] */ VARIANT i12,
            /* [optional][in] */ VARIANT i13,
            /* [optional][in] */ VARIANT i14,
            /* [optional][in] */ VARIANT i15,
            /* [optional][in] */ VARIANT i16,
            /* [optional][in] */ VARIANT i17,
            /* [optional][in] */ VARIANT i18,
            /* [optional][in] */ VARIANT i19,
            /* [optional][in] */ VARIANT i20,
            /* [optional][in] */ VARIANT i21,
            /* [optional][in] */ VARIANT i22,
            /* [optional][in] */ VARIANT i23,
            /* [optional][in] */ VARIANT i24,
            /* [optional][in] */ VARIANT i25,
            /* [optional][in] */ VARIANT i26,
            /* [optional][in] */ VARIANT i27,
            /* [optional][in] */ VARIANT i28,
            /* [optional][in] */ VARIANT i29,
            /* [optional][in] */ VARIANT i30,
            /* [optional][in] */ VARIANT i31,
            /* [retval][out] */ IMWField **ppField);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IMWStruct * This,
            /* [optional][in] */ VARIANT varDims,
            /* [optional][in] */ VARIANT varFieldNames);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumberOfFields )( 
            IMWStruct * This,
            /* [retval][out] */ long *pnFields);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumberOfDims )( 
            IMWStruct * This,
            /* [retval][out] */ long *pnDims);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Dims )( 
            IMWStruct * This,
            /* [retval][out] */ VARIANT *pvarDims);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FieldNames )( 
            IMWStruct * This,
            /* [retval][out] */ VARIANT *pvarFieldNames);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IMWStruct * This,
            /* [out] */ IMWStruct **ppStruct);
        
        END_INTERFACE
    } IMWStructVtbl;

    interface IMWStruct
    {
        CONST_VTBL struct IMWStructVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMWStruct_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMWStruct_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMWStruct_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMWStruct_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMWStruct_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMWStruct_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMWStruct_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMWStruct_get_Item(This,i0,i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12,i13,i14,i15,i16,i17,i18,i19,i20,i21,i22,i23,i24,i25,i26,i27,i28,i29,i30,i31,ppField)	\
    ( (This)->lpVtbl -> get_Item(This,i0,i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12,i13,i14,i15,i16,i17,i18,i19,i20,i21,i22,i23,i24,i25,i26,i27,i28,i29,i30,i31,ppField) ) 

#define IMWStruct_Initialize(This,varDims,varFieldNames)	\
    ( (This)->lpVtbl -> Initialize(This,varDims,varFieldNames) ) 

#define IMWStruct_get_NumberOfFields(This,pnFields)	\
    ( (This)->lpVtbl -> get_NumberOfFields(This,pnFields) ) 

#define IMWStruct_get_NumberOfDims(This,pnDims)	\
    ( (This)->lpVtbl -> get_NumberOfDims(This,pnDims) ) 

#define IMWStruct_get_Dims(This,pvarDims)	\
    ( (This)->lpVtbl -> get_Dims(This,pvarDims) ) 

#define IMWStruct_get_FieldNames(This,pvarFieldNames)	\
    ( (This)->lpVtbl -> get_FieldNames(This,pvarFieldNames) ) 

#define IMWStruct_Clone(This,ppStruct)	\
    ( (This)->lpVtbl -> Clone(This,ppStruct) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMWStruct_INTERFACE_DEFINED__ */


#ifndef __IMWComplex_INTERFACE_DEFINED__
#define __IMWComplex_INTERFACE_DEFINED__

/* interface IMWComplex */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMWComplex;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8CDD4BBF-A84C-4C50-B600-F5E90008491D")
    IMWComplex : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Real( 
            /* [retval][out] */ VARIANT *pvarValue) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Real( 
            /* [in] */ VARIANT varValue) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Imag( 
            /* [retval][out] */ VARIANT *pvarValue) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Imag( 
            /* [in] */ VARIANT varValue) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MWFlags( 
            /* [retval][out] */ IMWFlags **ppFlags) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MWFlags( 
            /* [in] */ IMWFlags *pFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IMWComplex **ppComplex) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMWComplexVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMWComplex * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMWComplex * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMWComplex * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMWComplex * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMWComplex * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMWComplex * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMWComplex * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Real )( 
            IMWComplex * This,
            /* [retval][out] */ VARIANT *pvarValue);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Real )( 
            IMWComplex * This,
            /* [in] */ VARIANT varValue);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Imag )( 
            IMWComplex * This,
            /* [retval][out] */ VARIANT *pvarValue);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Imag )( 
            IMWComplex * This,
            /* [in] */ VARIANT varValue);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MWFlags )( 
            IMWComplex * This,
            /* [retval][out] */ IMWFlags **ppFlags);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MWFlags )( 
            IMWComplex * This,
            /* [in] */ IMWFlags *pFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IMWComplex * This,
            /* [out] */ IMWComplex **ppComplex);
        
        END_INTERFACE
    } IMWComplexVtbl;

    interface IMWComplex
    {
        CONST_VTBL struct IMWComplexVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMWComplex_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMWComplex_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMWComplex_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMWComplex_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMWComplex_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMWComplex_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMWComplex_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMWComplex_get_Real(This,pvarValue)	\
    ( (This)->lpVtbl -> get_Real(This,pvarValue) ) 

#define IMWComplex_put_Real(This,varValue)	\
    ( (This)->lpVtbl -> put_Real(This,varValue) ) 

#define IMWComplex_get_Imag(This,pvarValue)	\
    ( (This)->lpVtbl -> get_Imag(This,pvarValue) ) 

#define IMWComplex_put_Imag(This,varValue)	\
    ( (This)->lpVtbl -> put_Imag(This,varValue) ) 

#define IMWComplex_get_MWFlags(This,ppFlags)	\
    ( (This)->lpVtbl -> get_MWFlags(This,ppFlags) ) 

#define IMWComplex_put_MWFlags(This,pFlags)	\
    ( (This)->lpVtbl -> put_MWFlags(This,pFlags) ) 

#define IMWComplex_Clone(This,ppComplex)	\
    ( (This)->lpVtbl -> Clone(This,ppComplex) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMWComplex_INTERFACE_DEFINED__ */


#ifndef __IMWSparse_INTERFACE_DEFINED__
#define __IMWSparse_INTERFACE_DEFINED__

/* interface IMWSparse */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMWSparse;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6D4874AD-38A1-4676-A191-BAA75F01D216")
    IMWSparse : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Array( 
            /* [retval][out] */ VARIANT *pvarArray) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Array( 
            /* [in] */ VARIANT varArray) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumRows( 
            /* [retval][out] */ long *pnRows) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NumRows( 
            /* [in] */ long nRows) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumColumns( 
            /* [retval][out] */ long *pnColumns) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NumColumns( 
            /* [in] */ long nColumns) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RowIndex( 
            /* [retval][out] */ VARIANT *pIndex) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RowIndex( 
            /* [in] */ VARIANT Index) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ColumnIndex( 
            /* [retval][out] */ VARIANT *pIndex) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ColumnIndex( 
            /* [in] */ VARIANT Index) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MWFlags( 
            /* [retval][out] */ IMWFlags **ppFlags) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MWFlags( 
            /* [in] */ IMWFlags *pFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IMWSparse **ppSparse) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMWSparseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMWSparse * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMWSparse * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMWSparse * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMWSparse * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMWSparse * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMWSparse * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMWSparse * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Array )( 
            IMWSparse * This,
            /* [retval][out] */ VARIANT *pvarArray);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Array )( 
            IMWSparse * This,
            /* [in] */ VARIANT varArray);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumRows )( 
            IMWSparse * This,
            /* [retval][out] */ long *pnRows);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NumRows )( 
            IMWSparse * This,
            /* [in] */ long nRows);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumColumns )( 
            IMWSparse * This,
            /* [retval][out] */ long *pnColumns);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NumColumns )( 
            IMWSparse * This,
            /* [in] */ long nColumns);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RowIndex )( 
            IMWSparse * This,
            /* [retval][out] */ VARIANT *pIndex);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RowIndex )( 
            IMWSparse * This,
            /* [in] */ VARIANT Index);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ColumnIndex )( 
            IMWSparse * This,
            /* [retval][out] */ VARIANT *pIndex);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ColumnIndex )( 
            IMWSparse * This,
            /* [in] */ VARIANT Index);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MWFlags )( 
            IMWSparse * This,
            /* [retval][out] */ IMWFlags **ppFlags);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MWFlags )( 
            IMWSparse * This,
            /* [in] */ IMWFlags *pFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IMWSparse * This,
            /* [out] */ IMWSparse **ppSparse);
        
        END_INTERFACE
    } IMWSparseVtbl;

    interface IMWSparse
    {
        CONST_VTBL struct IMWSparseVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMWSparse_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMWSparse_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMWSparse_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMWSparse_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMWSparse_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMWSparse_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMWSparse_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMWSparse_get_Array(This,pvarArray)	\
    ( (This)->lpVtbl -> get_Array(This,pvarArray) ) 

#define IMWSparse_put_Array(This,varArray)	\
    ( (This)->lpVtbl -> put_Array(This,varArray) ) 

#define IMWSparse_get_NumRows(This,pnRows)	\
    ( (This)->lpVtbl -> get_NumRows(This,pnRows) ) 

#define IMWSparse_put_NumRows(This,nRows)	\
    ( (This)->lpVtbl -> put_NumRows(This,nRows) ) 

#define IMWSparse_get_NumColumns(This,pnColumns)	\
    ( (This)->lpVtbl -> get_NumColumns(This,pnColumns) ) 

#define IMWSparse_put_NumColumns(This,nColumns)	\
    ( (This)->lpVtbl -> put_NumColumns(This,nColumns) ) 

#define IMWSparse_get_RowIndex(This,pIndex)	\
    ( (This)->lpVtbl -> get_RowIndex(This,pIndex) ) 

#define IMWSparse_put_RowIndex(This,Index)	\
    ( (This)->lpVtbl -> put_RowIndex(This,Index) ) 

#define IMWSparse_get_ColumnIndex(This,pIndex)	\
    ( (This)->lpVtbl -> get_ColumnIndex(This,pIndex) ) 

#define IMWSparse_put_ColumnIndex(This,Index)	\
    ( (This)->lpVtbl -> put_ColumnIndex(This,Index) ) 

#define IMWSparse_get_MWFlags(This,ppFlags)	\
    ( (This)->lpVtbl -> get_MWFlags(This,ppFlags) ) 

#define IMWSparse_put_MWFlags(This,pFlags)	\
    ( (This)->lpVtbl -> put_MWFlags(This,pFlags) ) 

#define IMWSparse_Clone(This,ppSparse)	\
    ( (This)->lpVtbl -> Clone(This,ppSparse) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMWSparse_INTERFACE_DEFINED__ */


#ifndef __IMWArg_INTERFACE_DEFINED__
#define __IMWArg_INTERFACE_DEFINED__

/* interface IMWArg */
/* [unique][helpstring][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_IMWArg;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9C77EA62-6498-4CF5-AF36-35886B2F0570")
    IMWArg : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [retval][out] */ VARIANT *pvarValue) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ VARIANT varValue) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MWFlags( 
            /* [retval][out] */ IMWFlags **ppFlags) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MWFlags( 
            /* [in] */ IMWFlags *pFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IMWArg **ppArg) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMWArgVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMWArg * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMWArg * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMWArg * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMWArg * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMWArg * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMWArg * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMWArg * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            IMWArg * This,
            /* [retval][out] */ VARIANT *pvarValue);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            IMWArg * This,
            /* [in] */ VARIANT varValue);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MWFlags )( 
            IMWArg * This,
            /* [retval][out] */ IMWFlags **ppFlags);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MWFlags )( 
            IMWArg * This,
            /* [in] */ IMWFlags *pFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IMWArg * This,
            /* [out] */ IMWArg **ppArg);
        
        END_INTERFACE
    } IMWArgVtbl;

    interface IMWArg
    {
        CONST_VTBL struct IMWArgVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMWArg_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMWArg_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMWArg_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMWArg_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMWArg_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMWArg_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMWArg_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMWArg_get_Value(This,pvarValue)	\
    ( (This)->lpVtbl -> get_Value(This,pvarValue) ) 

#define IMWArg_put_Value(This,varValue)	\
    ( (This)->lpVtbl -> put_Value(This,varValue) ) 

#define IMWArg_get_MWFlags(This,ppFlags)	\
    ( (This)->lpVtbl -> get_MWFlags(This,ppFlags) ) 

#define IMWArg_put_MWFlags(This,pFlags)	\
    ( (This)->lpVtbl -> put_MWFlags(This,pFlags) ) 

#define IMWArg_Clone(This,ppArg)	\
    ( (This)->lpVtbl -> Clone(This,ppArg) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMWArg_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mwcomtypes_0000_0009 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_mwcomtypes_0000_0009_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mwcomtypes_0000_0009_v0_0_s_ifspec;

#ifndef __IMWEnumVararg_INTERFACE_DEFINED__
#define __IMWEnumVararg_INTERFACE_DEFINED__

/* interface IMWEnumVararg */
/* [unique][helpstring][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_IMWEnumVararg;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D4D229FA-87A2-4a22-B58D-DB5898B4D92F")
    IMWEnumVararg : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long nIndex,
            /* [retval][out] */ IMWMethodArgInfo **ppInfo) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [retval][out] */ VARIANT *pvarValue) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ VARIANT varValue) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pnCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ long nSkipElms) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Next( 
            /* [out] */ IMWMethodArgInfo **ppInfo,
            /* [out] */ long *pnRetElms) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ VARIANT varValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ long nIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAt( 
            /* [in] */ long nIndex,
            /* [in] */ VARIANT varValue) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CopyToRange( 
            /* [in] */ VARIANT_BOOL bToRange) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CopyToRange( 
            /* [retval][out] */ VARIANT_BOOL *pbToRange) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IMWEnumVararg **ppEnum) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMWEnumVarargVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMWEnumVararg * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMWEnumVararg * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMWEnumVararg * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMWEnumVararg * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMWEnumVararg * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMWEnumVararg * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMWEnumVararg * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IMWEnumVararg * This,
            /* [in] */ long nIndex,
            /* [retval][out] */ IMWMethodArgInfo **ppInfo);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IMWEnumVararg * This,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IMWEnumVararg * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            IMWEnumVararg * This,
            /* [retval][out] */ VARIANT *pvarValue);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            IMWEnumVararg * This,
            /* [in] */ VARIANT varValue);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IMWEnumVararg * This,
            /* [retval][out] */ long *pnCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IMWEnumVararg * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IMWEnumVararg * This,
            /* [in] */ long nSkipElms);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Next )( 
            IMWEnumVararg * This,
            /* [out] */ IMWMethodArgInfo **ppInfo,
            /* [out] */ long *pnRetElms);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IMWEnumVararg * This,
            /* [in] */ VARIANT varValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IMWEnumVararg * This,
            /* [in] */ long nIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IMWEnumVararg * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAt )( 
            IMWEnumVararg * This,
            /* [in] */ long nIndex,
            /* [in] */ VARIANT varValue);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CopyToRange )( 
            IMWEnumVararg * This,
            /* [in] */ VARIANT_BOOL bToRange);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CopyToRange )( 
            IMWEnumVararg * This,
            /* [retval][out] */ VARIANT_BOOL *pbToRange);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IMWEnumVararg * This,
            /* [out] */ IMWEnumVararg **ppEnum);
        
        END_INTERFACE
    } IMWEnumVarargVtbl;

    interface IMWEnumVararg
    {
        CONST_VTBL struct IMWEnumVarargVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMWEnumVararg_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMWEnumVararg_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMWEnumVararg_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMWEnumVararg_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMWEnumVararg_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMWEnumVararg_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMWEnumVararg_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMWEnumVararg_get_Item(This,nIndex,ppInfo)	\
    ( (This)->lpVtbl -> get_Item(This,nIndex,ppInfo) ) 

#define IMWEnumVararg_get_Name(This,pbstrName)	\
    ( (This)->lpVtbl -> get_Name(This,pbstrName) ) 

#define IMWEnumVararg_put_Name(This,bstrName)	\
    ( (This)->lpVtbl -> put_Name(This,bstrName) ) 

#define IMWEnumVararg_get_Value(This,pvarValue)	\
    ( (This)->lpVtbl -> get_Value(This,pvarValue) ) 

#define IMWEnumVararg_put_Value(This,varValue)	\
    ( (This)->lpVtbl -> put_Value(This,varValue) ) 

#define IMWEnumVararg_get_Count(This,pnCount)	\
    ( (This)->lpVtbl -> get_Count(This,pnCount) ) 

#define IMWEnumVararg_Reset(This)	\
    ( (This)->lpVtbl -> Reset(This) ) 

#define IMWEnumVararg_Skip(This,nSkipElms)	\
    ( (This)->lpVtbl -> Skip(This,nSkipElms) ) 

#define IMWEnumVararg_Next(This,ppInfo,pnRetElms)	\
    ( (This)->lpVtbl -> Next(This,ppInfo,pnRetElms) ) 

#define IMWEnumVararg_Add(This,varValue)	\
    ( (This)->lpVtbl -> Add(This,varValue) ) 

#define IMWEnumVararg_Remove(This,nIndex)	\
    ( (This)->lpVtbl -> Remove(This,nIndex) ) 

#define IMWEnumVararg_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#define IMWEnumVararg_SetAt(This,nIndex,varValue)	\
    ( (This)->lpVtbl -> SetAt(This,nIndex,varValue) ) 

#define IMWEnumVararg_put_CopyToRange(This,bToRange)	\
    ( (This)->lpVtbl -> put_CopyToRange(This,bToRange) ) 

#define IMWEnumVararg_get_CopyToRange(This,pbToRange)	\
    ( (This)->lpVtbl -> get_CopyToRange(This,pbToRange) ) 

#define IMWEnumVararg_Clone(This,ppEnum)	\
    ( (This)->lpVtbl -> Clone(This,ppEnum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMWEnumVararg_INTERFACE_DEFINED__ */


#ifndef __IMWMethodArgInfo_INTERFACE_DEFINED__
#define __IMWMethodArgInfo_INTERFACE_DEFINED__

/* interface IMWMethodArgInfo */
/* [unique][helpstring][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_IMWMethodArgInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6D9F6760-7A70-4524-882D-E8E56A21E4D5")
    IMWMethodArgInfo : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [retval][out] */ VARIANT *pvarValue) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ VARIANT varValue) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ short *pnType) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Flags( 
            /* [retval][out] */ short *pnFlags) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Indirection( 
            /* [retval][out] */ long *pnInd) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsRange( 
            /* [retval][out] */ VARIANT_BOOL *pbRange) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsVararg( 
            /* [retval][out] */ VARIANT_BOOL *pbVararg) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MWFlags( 
            /* [retval][out] */ IMWFlags **ppFlags) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MWFlags( 
            /* [in] */ IMWFlags *pFlags) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Selected( 
            /* [retval][out] */ VARIANT_BOOL *pbSelected) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Vararg( 
            /* [retval][out] */ IMWEnumVararg **ppVararg) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Vararg( 
            /* [in] */ IMWEnumVararg *pVararg) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CopyToRange( 
            /* [in] */ VARIANT_BOOL bToRange) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CopyToRange( 
            /* [retval][out] */ VARIANT_BOOL *pbToRange) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Select( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsListening( 
            /* [retval][out] */ VARIANT_BOOL *pbListen) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsListening( 
            /* [in] */ VARIANT_BOOL bListen) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Offset( 
            /* [in] */ long nRows,
            /* [in] */ long nColumns,
            /* [in] */ VARIANT_BOOL bCopyContents,
            /* [in] */ VARIANT_BOOL bDeleteOriginal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IMWMethodArgInfo **ppInfo) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Index( 
            /* [retval][out] */ long *pnIndex) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMWMethodArgInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMWMethodArgInfo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMWMethodArgInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMWMethodArgInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMWMethodArgInfo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMWMethodArgInfo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMWMethodArgInfo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMWMethodArgInfo * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ VARIANT *pvarValue);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            IMWMethodArgInfo * This,
            /* [in] */ VARIANT varValue);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IMWMethodArgInfo * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ short *pnType);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Flags )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ short *pnFlags);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Indirection )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ long *pnInd);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsRange )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ VARIANT_BOOL *pbRange);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsVararg )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ VARIANT_BOOL *pbVararg);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MWFlags )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ IMWFlags **ppFlags);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MWFlags )( 
            IMWMethodArgInfo * This,
            /* [in] */ IMWFlags *pFlags);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Selected )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ VARIANT_BOOL *pbSelected);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Vararg )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ IMWEnumVararg **ppVararg);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Vararg )( 
            IMWMethodArgInfo * This,
            /* [in] */ IMWEnumVararg *pVararg);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CopyToRange )( 
            IMWMethodArgInfo * This,
            /* [in] */ VARIANT_BOOL bToRange);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CopyToRange )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ VARIANT_BOOL *pbToRange);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Select )( 
            IMWMethodArgInfo * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsListening )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ VARIANT_BOOL *pbListen);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsListening )( 
            IMWMethodArgInfo * This,
            /* [in] */ VARIANT_BOOL bListen);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Offset )( 
            IMWMethodArgInfo * This,
            /* [in] */ long nRows,
            /* [in] */ long nColumns,
            /* [in] */ VARIANT_BOOL bCopyContents,
            /* [in] */ VARIANT_BOOL bDeleteOriginal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IMWMethodArgInfo * This,
            /* [out] */ IMWMethodArgInfo **ppInfo);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Index )( 
            IMWMethodArgInfo * This,
            /* [retval][out] */ long *pnIndex);
        
        END_INTERFACE
    } IMWMethodArgInfoVtbl;

    interface IMWMethodArgInfo
    {
        CONST_VTBL struct IMWMethodArgInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMWMethodArgInfo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMWMethodArgInfo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMWMethodArgInfo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMWMethodArgInfo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMWMethodArgInfo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMWMethodArgInfo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMWMethodArgInfo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMWMethodArgInfo_get_Value(This,pvarValue)	\
    ( (This)->lpVtbl -> get_Value(This,pvarValue) ) 

#define IMWMethodArgInfo_put_Value(This,varValue)	\
    ( (This)->lpVtbl -> put_Value(This,varValue) ) 

#define IMWMethodArgInfo_get_Name(This,pbstrName)	\
    ( (This)->lpVtbl -> get_Name(This,pbstrName) ) 

#define IMWMethodArgInfo_put_Name(This,bstrName)	\
    ( (This)->lpVtbl -> put_Name(This,bstrName) ) 

#define IMWMethodArgInfo_get_Type(This,pnType)	\
    ( (This)->lpVtbl -> get_Type(This,pnType) ) 

#define IMWMethodArgInfo_get_Flags(This,pnFlags)	\
    ( (This)->lpVtbl -> get_Flags(This,pnFlags) ) 

#define IMWMethodArgInfo_get_Indirection(This,pnInd)	\
    ( (This)->lpVtbl -> get_Indirection(This,pnInd) ) 

#define IMWMethodArgInfo_get_IsRange(This,pbRange)	\
    ( (This)->lpVtbl -> get_IsRange(This,pbRange) ) 

#define IMWMethodArgInfo_get_IsVararg(This,pbVararg)	\
    ( (This)->lpVtbl -> get_IsVararg(This,pbVararg) ) 

#define IMWMethodArgInfo_get_MWFlags(This,ppFlags)	\
    ( (This)->lpVtbl -> get_MWFlags(This,ppFlags) ) 

#define IMWMethodArgInfo_put_MWFlags(This,pFlags)	\
    ( (This)->lpVtbl -> put_MWFlags(This,pFlags) ) 

#define IMWMethodArgInfo_get_Selected(This,pbSelected)	\
    ( (This)->lpVtbl -> get_Selected(This,pbSelected) ) 

#define IMWMethodArgInfo_get_Vararg(This,ppVararg)	\
    ( (This)->lpVtbl -> get_Vararg(This,ppVararg) ) 

#define IMWMethodArgInfo_put_Vararg(This,pVararg)	\
    ( (This)->lpVtbl -> put_Vararg(This,pVararg) ) 

#define IMWMethodArgInfo_put_CopyToRange(This,bToRange)	\
    ( (This)->lpVtbl -> put_CopyToRange(This,bToRange) ) 

#define IMWMethodArgInfo_get_CopyToRange(This,pbToRange)	\
    ( (This)->lpVtbl -> get_CopyToRange(This,pbToRange) ) 

#define IMWMethodArgInfo_Select(This)	\
    ( (This)->lpVtbl -> Select(This) ) 

#define IMWMethodArgInfo_get_IsListening(This,pbListen)	\
    ( (This)->lpVtbl -> get_IsListening(This,pbListen) ) 

#define IMWMethodArgInfo_put_IsListening(This,bListen)	\
    ( (This)->lpVtbl -> put_IsListening(This,bListen) ) 

#define IMWMethodArgInfo_Offset(This,nRows,nColumns,bCopyContents,bDeleteOriginal)	\
    ( (This)->lpVtbl -> Offset(This,nRows,nColumns,bCopyContents,bDeleteOriginal) ) 

#define IMWMethodArgInfo_Clone(This,ppInfo)	\
    ( (This)->lpVtbl -> Clone(This,ppInfo) ) 

#define IMWMethodArgInfo_get_Index(This,pnIndex)	\
    ( (This)->lpVtbl -> get_Index(This,pnIndex) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMWMethodArgInfo_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


