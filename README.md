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

iPage of SetActivePage:
- Set 0 to call (Default) generator.
- Set non zero such as 1,2 or 3 to call "MP" generator.

Thumbnail generators
--------------------

The thumbnail generator extracts a thumbnail image from a document like TIF/PDF.

It is done by command line converter programs.

Add the registry value in order to install it.

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
Suffix
```
Note: `Suffix` has been added since 1.2.14.

#### `(Default)` and `MP`
Specify a command line thumbnail generator.

It extracts a single page thumbnail from your document.

Description: 
* The registry value __(Default)__ is for **single** page document like JPG/PNG.
* The registry value __MP__ is for **multi** page document like TIF/PDF, if it is supported.


Example: 
* "C:\Program Files (x86)\ThumbGensPack\pdf2bmp2.exe" "%1!s!" "%2!s!" "%3!u!" "%4!u!" "%5!u!"


%1
* input file path


%2
* output file path (is a temp file path)
* `Suffix` value is appended.

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
* GPC tool leaves page count. Gene reads it by [_ttoi](http://msdn.microsoft.com/en-us/library/yd5xkb5c.aspx).


ExitCode:
* 0 means ok
* non-0 means err. returns E_FAIL.


### Registry example
```
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.pdf]
@="\"C:\\Program Files (x86)\\ThumbGensPack\\pdf2bmp2.exe\" \"%1!s!\" \"%2!s!\" \"%3!u!\" \"%4!u!\" \"%5!u!\""
"MP"="\"C:\\Program Files (x86)\\ThumbGensPack\\pdf2bmp2.exe\" \"%1!s!\" \"%2!s!\" \"%3!u!\" \"%4!u!\" \"%5!u!\" \"%6!u!\""
"GPC"="\"C:\\Program Files (x86)\\ThumbGensPack\\pdfpcnt.exe\" \"%1!s!\" \"%2!s!\""
```
