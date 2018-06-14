ISetPage4ThumbnailProvider interface
====================================

Adding multipage support to IThumbnailProvider.

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

Useful for:
* PDF
* TIF/TIFF

iPage of SetActivePage:
| iPage | Command type invoked |
|:-----:|----------------------|
|   0   | Invoke `(Default)` command. |
| Positive numbers <br /> 1,2,3,... | Invoke `MP` command. |
