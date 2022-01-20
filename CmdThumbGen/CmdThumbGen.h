

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CmdThumbGen.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __CmdThumbGen_h__
#define __CmdThumbGen_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IGene_FWD_DEFINED__
#define __IGene_FWD_DEFINED__
typedef interface IGene IGene;

#endif 	/* __IGene_FWD_DEFINED__ */


#ifndef __ISetPage4ThumbnailProvider_FWD_DEFINED__
#define __ISetPage4ThumbnailProvider_FWD_DEFINED__
typedef interface ISetPage4ThumbnailProvider ISetPage4ThumbnailProvider;

#endif 	/* __ISetPage4ThumbnailProvider_FWD_DEFINED__ */


#ifndef __Gene_FWD_DEFINED__
#define __Gene_FWD_DEFINED__

#ifdef __cplusplus
typedef class Gene Gene;
#else
typedef struct Gene Gene;
#endif /* __cplusplus */

#endif 	/* __Gene_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "ShObjIdl.h"
#include "Thumbcache.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IGene_INTERFACE_DEFINED__
#define __IGene_INTERFACE_DEFINED__

/* interface IGene */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IGene;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("60E422E5-E50D-4C9C-91F3-D2810E7F2E9A")
    IGene : public IUnknown
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IGeneVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGene * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGene * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGene * This);
        
        END_INTERFACE
    } IGeneVtbl;

    interface IGene
    {
        CONST_VTBL struct IGeneVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGene_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGene_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGene_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGene_INTERFACE_DEFINED__ */


#ifndef __ISetPage4ThumbnailProvider_INTERFACE_DEFINED__
#define __ISetPage4ThumbnailProvider_INTERFACE_DEFINED__

/* interface ISetPage4ThumbnailProvider */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ISetPage4ThumbnailProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d8d5f848-8223-42fc-bbcb-c40056b71a17")
    ISetPage4ThumbnailProvider : public IThumbnailProvider
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPageCount( 
            /* [retval][out] */ DWORD *pcPages) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetActivePage( 
            /* [in] */ DWORD iPage) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISetPage4ThumbnailProviderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISetPage4ThumbnailProvider * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISetPage4ThumbnailProvider * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISetPage4ThumbnailProvider * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetThumbnail )( 
            ISetPage4ThumbnailProvider * This,
            /* [in] */ UINT cx,
            /* [out] */ HBITMAP *phbmp,
            /* [out] */ WTS_ALPHATYPE *pdwAlpha);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPageCount )( 
            ISetPage4ThumbnailProvider * This,
            /* [retval][out] */ DWORD *pcPages);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetActivePage )( 
            ISetPage4ThumbnailProvider * This,
            /* [in] */ DWORD iPage);
        
        END_INTERFACE
    } ISetPage4ThumbnailProviderVtbl;

    interface ISetPage4ThumbnailProvider
    {
        CONST_VTBL struct ISetPage4ThumbnailProviderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISetPage4ThumbnailProvider_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISetPage4ThumbnailProvider_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISetPage4ThumbnailProvider_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISetPage4ThumbnailProvider_GetThumbnail(This,cx,phbmp,pdwAlpha)	\
    ( (This)->lpVtbl -> GetThumbnail(This,cx,phbmp,pdwAlpha) ) 


#define ISetPage4ThumbnailProvider_GetPageCount(This,pcPages)	\
    ( (This)->lpVtbl -> GetPageCount(This,pcPages) ) 

#define ISetPage4ThumbnailProvider_SetActivePage(This,iPage)	\
    ( (This)->lpVtbl -> SetActivePage(This,iPage) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISetPage4ThumbnailProvider_INTERFACE_DEFINED__ */



#ifndef __CmdThumbGenLib_LIBRARY_DEFINED__
#define __CmdThumbGenLib_LIBRARY_DEFINED__

/* library CmdThumbGenLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_CmdThumbGenLib;

EXTERN_C const CLSID CLSID_Gene;

#ifdef __cplusplus

class DECLSPEC_UUID("93FB1A02-084D-43B4-A69F-65D8B86F2AB3")
Gene;
#endif
#endif /* __CmdThumbGenLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


