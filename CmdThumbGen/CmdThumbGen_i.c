

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu May 22 12:06:26 2014
 */
/* Compiler settings for .\CmdThumbGen.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IGene,0x60E422E5,0xE50D,0x4C9C,0x91,0xF3,0xD2,0x81,0x0E,0x7F,0x2E,0x9A);


MIDL_DEFINE_GUID(IID, IID_ISetPage4ThumbnailProvider,0xd8d5f848,0x8223,0x42fc,0xbb,0xcb,0xc4,0x00,0x56,0xb7,0x1a,0x17);


MIDL_DEFINE_GUID(IID, LIBID_CmdThumbGenLib,0x7DF507D7,0x46CD,0x43FF,0xA5,0x91,0x4C,0xB7,0xE9,0xD5,0xBE,0x63);


MIDL_DEFINE_GUID(CLSID, CLSID_Gene,0x93FB1A02,0x084D,0x43B4,0xA6,0x9F,0x65,0xD8,0xB8,0x6F,0x2A,0xB3);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



