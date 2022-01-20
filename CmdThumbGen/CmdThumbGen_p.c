

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "CmdThumbGen.h"

#define TYPE_FORMAT_STRING_SIZE   7                                 
#define PROC_FORMAT_STRING_SIZE   73                                
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _CmdThumbGen_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } CmdThumbGen_MIDL_TYPE_FORMAT_STRING;

typedef struct _CmdThumbGen_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } CmdThumbGen_MIDL_PROC_FORMAT_STRING;

typedef struct _CmdThumbGen_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } CmdThumbGen_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const CmdThumbGen_MIDL_TYPE_FORMAT_STRING CmdThumbGen__MIDL_TypeFormatString;
extern const CmdThumbGen_MIDL_PROC_FORMAT_STRING CmdThumbGen__MIDL_ProcFormatString;
extern const CmdThumbGen_MIDL_EXPR_FORMAT_STRING CmdThumbGen__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IGene_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IGene_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ISetPage4ThumbnailProvider_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ISetPage4ThumbnailProvider_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif
#if !(TARGET_IS_NT60_OR_LATER)
#error You need Windows Vista or later to run this stub because it uses these features:
#error   forced complex structure or array, compiled for Windows Vista.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const CmdThumbGen_MIDL_PROC_FORMAT_STRING CmdThumbGen__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure GetPageCount */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x4 ),	/* 4 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pcPages */

/* 24 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetActivePage */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x5 ),	/* 5 */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	NdrFcShort( 0x8 ),	/* 8 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 52 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter iPage */

/* 60 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 66 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const CmdThumbGen_MIDL_TYPE_FORMAT_STRING CmdThumbGen__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IGene, ver. 0.0,
   GUID={0x60E422E5,0xE50D,0x4C9C,{0x91,0xF3,0xD2,0x81,0x0E,0x7F,0x2E,0x9A}} */

#pragma code_seg(".orpc")
static const unsigned short IGene_FormatStringOffsetTable[] =
    {
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IGene_ProxyInfo =
    {
    &Object_StubDesc,
    CmdThumbGen__MIDL_ProcFormatString.Format,
    &IGene_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IGene_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    CmdThumbGen__MIDL_ProcFormatString.Format,
    &IGene_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(3) _IGeneProxyVtbl = 
{
    0,
    &IID_IGene,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy
};

const CInterfaceStubVtbl _IGeneStubVtbl =
{
    &IID_IGene,
    &IGene_ServerInfo,
    3,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IThumbnailProvider, ver. 0.0,
   GUID={0xe357fccd,0xa995,0x4576,{0xb0,0x1f,0x23,0x46,0x30,0x15,0x4e,0x96}} */


/* Object interface: ISetPage4ThumbnailProvider, ver. 0.0,
   GUID={0xd8d5f848,0x8223,0x42fc,{0xbb,0xcb,0xc4,0x00,0x56,0xb7,0x1a,0x17}} */

#pragma code_seg(".orpc")
static const unsigned short ISetPage4ThumbnailProvider_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    0,
    36
    };

static const MIDL_STUBLESS_PROXY_INFO ISetPage4ThumbnailProvider_ProxyInfo =
    {
    &Object_StubDesc,
    CmdThumbGen__MIDL_ProcFormatString.Format,
    &ISetPage4ThumbnailProvider_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ISetPage4ThumbnailProvider_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    CmdThumbGen__MIDL_ProcFormatString.Format,
    &ISetPage4ThumbnailProvider_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(6) _ISetPage4ThumbnailProviderProxyVtbl = 
{
    &ISetPage4ThumbnailProvider_ProxyInfo,
    &IID_ISetPage4ThumbnailProvider,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IThumbnailProvider::GetThumbnail */ ,
    (void *) (INT_PTR) -1 /* ISetPage4ThumbnailProvider::GetPageCount */ ,
    (void *) (INT_PTR) -1 /* ISetPage4ThumbnailProvider::SetActivePage */
};


static const PRPC_STUB_FUNCTION ISetPage4ThumbnailProvider_table[] =
{
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _ISetPage4ThumbnailProviderStubVtbl =
{
    &IID_ISetPage4ThumbnailProvider,
    &ISetPage4ThumbnailProvider_ServerInfo,
    6,
    &ISetPage4ThumbnailProvider_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    CmdThumbGen__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x60001, /* Ndr library version */
    0,
    0x801026e, /* MIDL Version 8.1.622 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _CmdThumbGen_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ISetPage4ThumbnailProviderProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IGeneProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _CmdThumbGen_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ISetPage4ThumbnailProviderStubVtbl,
    ( CInterfaceStubVtbl *) &_IGeneStubVtbl,
    0
};

PCInterfaceName const _CmdThumbGen_InterfaceNamesList[] = 
{
    "ISetPage4ThumbnailProvider",
    "IGene",
    0
};

const IID *  const _CmdThumbGen_BaseIIDList[] = 
{
    &IID_IThumbnailProvider,
    0,
    0
};


#define _CmdThumbGen_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _CmdThumbGen, pIID, n)

int __stdcall _CmdThumbGen_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _CmdThumbGen, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _CmdThumbGen, 2, *pIndex )
    
}

const ExtendedProxyFileInfo CmdThumbGen_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _CmdThumbGen_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _CmdThumbGen_StubVtblList,
    (const PCInterfaceName * ) & _CmdThumbGen_InterfaceNamesList,
    (const IID ** ) & _CmdThumbGen_BaseIIDList,
    & _CmdThumbGen_IID_Lookup, 
    2,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

