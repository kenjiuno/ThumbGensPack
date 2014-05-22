ThumbGensPack
=============

Thumbnail Generator pack for Windows 7 Explorer

CmdThumbGen
-----------

CmdThumbGen is a thumbnail provider implementation.

Gene Class exposes:
* __IExtractImage__ interface http://msdn.microsoft.com/en-us/library/windows/desktop/bb761848(v=vs.85).aspx
* __IExtractImage2__ interface http://msdn.microsoft.com/en-us/library/windows/desktop/bb761842(v=vs.85).aspx
* __IInitializeWithFile__ interface http://msdn.microsoft.com/en-us/library/windows/desktop/bb761818(v=vs.85).aspx
* __IInitializeWithItem__ interface http://msdn.microsoft.com/en-us/library/windows/desktop/bb761814(v=vs.85).aspx
* __IPersist__ interface
* __IPersistFile__ interface
* __IThumbnailProvider__ interface http://msdn.microsoft.com/en-us/library/windows/desktop/bb774614(v=vs.85).aspx
* __ISetPage4ThumbnailProvider__ interface
* __IUnknown__ interface

ISetPage4ThumbnailProvider interface
------------------------------------

It adds multipage extraction support for IThumbnailProvider.

Useful for:
* PDF
* TIF/TIFF

idl definition:
```
	[
	  odl,
	  uuid(D8D5F848-8223-42FC-BBCB-C40056B71A17),
	  helpstring("ISetPage4ThumbnailProvider if")
	]
	interface ISetPage4ThumbnailProvider : IThumbnailProvider {
		HRESULT _stdcall GetPageCount([out, retval] unsigned long* pcPages);
		HRESULT _stdcall SetActivePage([in] unsigned long iPage);
	};
```

Thumbnail generators
--------------------

thumbnail generation is done by command line programs.

### Registry key path
```
HKLM
 Software
  HIRAOKA HYPERS TOOLS, Inc.
   CmdThumbGen
    FileExts
     .pdf
```

### Registry values
```
(Default)
MP
GPC
```

#### (Default) and MP
Description: 
* __(Default)__ is for single page extraction
* __MP__ is for multi page extraction


Example: 
* "C:\Program Files (x86)\ThumbGensPack\pdf2bmp2.exe" "%1!s!" "%2!s!" "%3!u!" "%4!u!" "%5!u!"


%1
* input file path


%2
* output file path (is a temp file path)


%3
* width in pixels


%4
* height in pixels


%5
* flags from IExtractImage::GetLocation (like IEIFLAG_ORIGSIZE)


%6
* iCurPage
 * if iCurPage == 0, output first page with verb "".
 * if iCurPage == 1, output first page with verb "MP".
 * if iCurPage == 2, output second page with verb "MP".
 * if iCurPage == 3, output third page with verb "MP".

ExitCode:
* 0 means ok
* non-0 means err. returns E_FAIL.


#### GPC
Description:
* GPC means GetPageCount


Example:
* "C:\Program Files (x86)\ThumbGensPack\pdfpcnt.exe" "%1!s!" "%2!s!"


%1
* input file path


%2
* output file path (is a temp file path)


ExitCode:
* 0 means ok
* non-0 means err. returns E_FAIL.
