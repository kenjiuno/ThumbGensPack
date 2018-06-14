CmdThumbGen's registry
----------------------

The thumbnail generator extracts a thumbnail image from a document like TIF/PDF.

It is done by command line converter programs.

Add the registry value in order to install it.

## Registry key path

CmdThumbGen looks up the following registry key per `(.fileExtension)`.

```
HKLM
 Software
  HIRAOKA HYPERS TOOLS, Inc.
   CmdThumbGen
    FileExts
     (.fileExtension)
```

## Registry values

Under `(.fileExtension)` registry key, you can set these values:

| Value's name | Type | Usage |
|:------------:|:----:|-------|
| `(Default)`  | Command line with parameters | A single page document (like JPG/PNG). |
| `MP`         | Command line with parameters | A multipage document (like TIF/PDF). This is optional. |
| `GPC`        | Command line with parameters | GPC means GetPageCount |
| `Suffix`     | Attribute    | (Since 1.2.14) append `Suffix` to output file path. <br/>Useful if you use generic converter like ImageMagick to specify `.png` to get better output than using `.bmp`. |

Command line parameters:
- Command line and its parameters are resolved by Win32 [FormatMessage](https://msdn.microsoft.com/ja-jp/library/windows/desktop/ms679351(v=vs.85).aspx) API.

### For `(Default)` and `MP`:

| Symbol | Usage |
|:------:|-------|
| `%1`   | - Input file path |
| `%2`   | - Output file path (is a temp file path) with no file extension. <br />- `Suffix` value is appended since 1.2.14. |
| `%3`   | - Requested width in pixel unit |
| `%4`   | - Requested height in pixel unit |
| `%5`   | - Flags of IExtractImage::GetLocation (like IEIFLAG_ORIGSIZE) |
| `%6`   | Requested page number: <br />- Always `0` for command type `""`. <br />- Positive page numbers are given for command type `MP`. |

How to deal with output contents:
- _1.2.13 and earlier_:
  - Try [LoadImage](https://msdn.microsoft.com/ja-jp/library/windows/desktop/ms648045(v=vs.85).aspx) API.
- _1.2.14 and later_:
  - Try `Windows Imaging Component` beginning with [IWICImagingFactory interface](https://msdn.microsoft.com/ja-jp/library/windows/desktop/ee690281(v=vs.85).aspx).
  - And then try [LoadImage](https://msdn.microsoft.com/ja-jp/library/windows/desktop/ms648045(v=vs.85).aspx) API.

ExitCode:
- `0` means successful
- `non-zero` means failure. CmdThumbGen returns `E_FAIL`.

### For `GPC`:

| Symbol | Usage |
|:------:|-------|
| `%1`   | - Input file path |
| `%2`   | - Output file path (is a temp file path). <br />- You should store page count in integer. <br />- CmdThumbGen tries reading it by [_ttoi](http://msdn.microsoft.com/en-us/library/yd5xkb5c.aspx). |

ExitCode:
- 0 means successful.
- `non-zero` means failure. CmdThumbGen returns `E_FAIL`.

### Registry example for .pdf
```
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.pdf]
@="\"C:\\Program Files (x86)\\ThumbGensPack\\pdf2bmp2.exe\" \"%1!s!\" \"%2!s!\" \"%3!u!\" \"%4!u!\" \"%5!u!\""
"MP"="\"C:\\Program Files (x86)\\ThumbGensPack\\pdf2bmp2.exe\" \"%1!s!\" \"%2!s!\" \"%3!u!\" \"%4!u!\" \"%5!u!\" \"%6!u!\""
"GPC"="\"C:\\Program Files (x86)\\ThumbGensPack\\pdfpcnt.exe\" \"%1!s!\" \"%2!s!\""
```
