; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

!define APP "ThumbGensPack"
!define TTL "ThumbGensPack ${__TIMESTAMP__}"
!define COM "HIRAOKA HYPERS TOOLS, Inc."

!define VER "1.2.11"

!define CLSID "{93FB1A02-084D-43B4-A69F-65d8b86f2ab3}"

!define IID_IExtractImage "{BB2E617C-0920-11d1-9A0B-00C04FC2D6C1}"
!define IID_IThumbnailProvider "{e357fccd-a995-4576-b01f-234630154e96}"
!define IID_ISetPage4ThumbnailProvider "{d8d5f848-8223-42fc-bbcb-c40056b71a17}"

; The name of the installer
Name "${APP} ${VER}"

; The file to write
OutFile "Setup_${APP}.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\${APP}"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\${COM}\${APP}" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

!define DOTNET_VERSION "2.0"

!include "DotNET.nsh"
!include LogicLib.nsh

!include "Registry.nsh"

;--------------------------------

; Pages

PageEx license
   LicenseData GPLv2.rtf
PageExEnd

Page license
Page components
Page directory
Page instfiles

LicenseData Readme.rtf

UninstPage uninstConfirm
UninstPage instfiles

SetOverwrite ifdiff

;--------------------------------

; The stuff to install
; - - - Section - - -
Section ""
  SetOutPath $INSTDIR

  !insertmacro CheckDotNET ${DOTNET_VERSION}

  ; Write the installation path into the registry
  WriteRegStr HKLM "Software\${COM}\${APP}" "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "DisplayName" "${TTL}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "NoRepair" 1
  WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

; - - - Section - - -
Section "CmdThumbGen ����"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  File ".\CmdThumbGen\Release\CmdThumbGen.dll"
  File ".\CmdThumbGen\Release\CmdThumbGen.pdb"

  ExecWait 'REGSVR32 /s "$OUTDIR\CmdThumbGen.dll"' $0
  DetailPrint "����: $0"
SectionEnd

Section /o "CmdThumbGen(x64) ����" SEC_CMDTHUMBGEN_X64
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\x64

  File ".\CmdThumbGen\x64\Release\CmdThumbGen.dll"
  File ".\CmdThumbGen\x64\Release\CmdThumbGen.pdb"

  ExecWait 'REGSVR32 /s "$OUTDIR\CmdThumbGen.dll"' $0
  DetailPrint "����: $0"
SectionEnd

!macro BackupRegValue FULLPATH SRCVALUE TARVALUE
  Push $R1
  Push $R2
  ${registry::Read} "${FULLPATH}" "${TARVALUE}" $R1 $R2
  ${If} $R2 == ''
    ${registry::MoveValue} "${FULLPATH}" "${SRCVALUE}" "${FULLPATH}" "${TARVALUE}" $R1
  ${EndIf}
  Pop $R2
  Pop $R1
!macroend

!macro SetExtAssoc EXT
    StrCpy $0 "${EXT}"

    !insertmacro BackupRegValue "HKCR\$0\ShellEx\${IID_IExtractImage}"      "" "Backup"
    !insertmacro BackupRegValue "HKCR\$0\ShellEx\${IID_IThumbnailProvider}" "" "Backup"

    WriteRegStr HKCR "$0\ShellEx\${IID_IExtractImage}" "" "${CLSID}"
    WriteRegStr HKCR "$0\ShellEx\${IID_IThumbnailProvider}" "" "${CLSID}"
    WriteRegStr HKCR "$0\ShellEx\${IID_ISetPage4ThumbnailProvider}" "" "${CLSID}"
!macroend

!macro SetProgidAssoc EXT
    StrCpy $0 "${EXT}"

    ReadRegStr $1 HKCR "$0" ""
    ${If} $1 != ''
      ReadRegStr $2 HKCR "$1\CurVer" ""
      ${If} $2 != ''
        StrCpy $1 $2
      ${EndIf}

      !insertmacro BackupRegValue "HKCR\$1\ShellEx\${IID_IExtractImage}"      "" "Backup"
      !insertmacro BackupRegValue "HKCR\$1\ShellEx\${IID_IThumbnailProvider}" "" "Backup"

      WriteRegStr HKCR "$1\ShellEx\${IID_IExtractImage}" "" "${CLSID}"
      WriteRegStr HKCR "$1\ShellEx\${IID_IThumbnailProvider}" "" "${CLSID}"
      WriteRegStr HKCR "$0\ShellEx\${IID_ISetPage4ThumbnailProvider}" "" "${CLSID}"
    ${EndIf}
!macroend

SectionGroup "PDF�Ή�"
  ; - - - Section - - -
  Section "pdf2bmp2,pdfpcnt�ꎮ"
    ; Set output path to the installation directory.
    SetOutPath $INSTDIR
  
    ; Put file there
    File ".\pdf2bmp2\release\pdf2bmp2.exe"
    File ".\pdf2bmp2\release\pdf2bmp2.pdb"
    File ".\pdf2bmp2\pdftoppm.exe"

    File ".\pdfpcnt\bin\Release\pdfpcnt.exe"
    File ".\pdfpcnt\bin\Release\pdfpcnt.pdb"
    File ".\pdfpcnt\pdfinfo.exe"

    WriteRegStr HKLM "Software\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.pdf" ""    '"$INSTDIR\pdf2bmp2.exe" "%1!s!" "%2!s!" "%3!u!" "%4!u!" "%5!u!"'
    WriteRegStr HKLM "Software\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.pdf" "MP"  '"$INSTDIR\pdf2bmp2.exe" "%1!s!" "%2!s!" "%3!u!" "%4!u!" "%5!u!" "%6!u!"'
    WriteRegStr HKLM "Software\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.pdf" "GPC" '"$INSTDIR\pdfpcnt.exe" "%1!s!" "%2!s!"'
    SetRegView 64
    WriteRegStr HKLM "Software\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.pdf" ""    '"$INSTDIR\pdf2bmp2.exe" "%1!s!" "%2!s!" "%3!u!" "%4!u!" "%5!u!"'
    WriteRegStr HKLM "Software\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.pdf" "MP"  '"$INSTDIR\pdf2bmp2.exe" "%1!s!" "%2!s!" "%3!u!" "%4!u!" "%5!u!" "%6!u!"'
    WriteRegStr HKLM "Software\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.pdf" "GPC" '"$INSTDIR\pdfpcnt.exe" "%1!s!" "%2!s!"'
    SetRegView lastused

    SetOutPath "$INSTDIR\share"
    File /r /x ".svn" "share\*.*"

  SectionEnd
  ; - - - Section - - -
  Section ".PDF �g���q�� �֘A�t��"
    !insertmacro SetExtAssoc ".pdf"
  SectionEnd
  ; - - - Section - - -
  Section ".PDF �A�v���� �֘A�t��"
    !insertmacro SetProgidAssoc ".pdf"
  SectionEnd
SectionGroupEnd



SectionGroup "JNT�Ή� (Windows Journal Viewer 1.5 �Ή�)"
  ; - - - Section - - -
  Section /o "Jnt2bmp�ꎮ"
    ; Set output path to the installation directory.
    SetOutPath $INSTDIR

    ; Put file there
    File "..\Jnt2bmp\bin\x86\DEBUG\Jnt2bmp.exe"
    File "..\Jnt2bmp\bin\x86\DEBUG\Jnt2bmp.pdb"
    File "..\Jnt2bmp\bin\x86\DEBUG\AxInterop.NbDocViewerLib.dll"
    File "..\Jnt2bmp\bin\x86\DEBUG\Interop.NbDocViewerLib.dll"

    WriteRegStr HKLM "Software\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.jnt" "" '"$INSTDIR\Jnt2bmp.exe" "%1!s!" "%2!s!" "%3!u!" "%4!u!"'
    SetRegView 64
    WriteRegStr HKLM "Software\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.jnt" "" '"$INSTDIR\Jnt2bmp.exe" "%1!s!" "%2!s!" "%3!u!" "%4!u!"'
    SetRegView lastused
  SectionEnd
  ; - - - Section - - -
  Section /o ".JNT �g���q�� �֘A�t��"
    !insertmacro SetExtAssoc ".jnt"
  SectionEnd
  ; - - - Section - - -
  Section /o "�J��: Microsoft Windows Journal Viewer 1.5" SEC_JOURNAL
    ExecShell "open" "http://www.microsoft.com/downloads/ja-jp/details.aspx?FamilyID=fad44098-8b73-4e06-96d4-d1eb70eacb44"
  SectionEnd
SectionGroupEnd



SectionGroup "DXF�Ή�"
  ; - - - Section - - -
  Section /o "dxf2bmp�ꎮ"
    ; Set output path to the installation directory.
    SetOutPath $INSTDIR

    ; Put file there
    File ".\dxf2bmp\bin\Release\dxf2bmp.pdb"
    File ".\dxf2bmp\bin\Release\dxf2bmp.exe"

    WriteRegStr HKLM "Software\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.dxf" "" '"$INSTDIR\dxf2bmp.exe" "%1!s!" "%2!s!" "%3!u!" "%4!u!"'
    SetRegView 64
    WriteRegStr HKLM "Software\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\.dxf" "" '"$INSTDIR\dxf2bmp.exe" "%1!s!" "%2!s!" "%3!u!" "%4!u!"'
    SetRegView lastused
    
    IfFileExists "$WINDIR\Microsoft.NET\Framework\v3.5\*.*" Avail
      MessageBox MB_ICONEXCLAMATION "dxf2bmp�����s����ɂ�.NET Framework 3.5���K�v�ł��B$\n$\n���o����Ȃ������悤�Ȃ̂ŁA�x���������܂��B"
    Avail:

  SectionEnd
  ; - - - Section - - -
  Section /o ".DXF �g���q�� �֘A�t��"
    !insertmacro SetExtAssoc ".dxf"
  SectionEnd
  ; - - - Section - - -
  Section /o ".DXF �A�v���� �֘A�t��"
    !insertmacro SetProgidAssoc ".dxf"
  SectionEnd
SectionGroupEnd

; - - - Section - - -
Section /o "�\�[�X�R�[�h����"
  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\src"
  File /r /x ".svn" "src\*.*"
SectionEnd

Function .onInit
;  ReadRegStr $0 HKCR "CLSID\{03A421AD-E8EE-4c47-9A03-FB386747186B}\InprocServer32" ""
;  IfFileExists $0 Skip
;    SectionSetFlags ${SEC_JOURNAL} ${SF_SELECTED}
;  Skip:
  
  ${If} $PROGRAMFILES32 != $PROGRAMFILES64
    SectionSetFlags ${SEC_CMDTHUMBGEN_X64} ${SF_SELECTED}
  ${EndIf}
FunctionEnd

;--------------------------------

; Uninstaller

!macro RestoreAssoc EXT
  StrCpy $0 "${EXT}"
  DetailPrint "$0 �֘A�t������"

  DeleteRegValue HKCR "$0\ShellEx\${IID_IExtractImage}" ""
  DeleteRegValue HKCR "$0\ShellEx\${IID_IThumbnailProvider}" ""

  ${registry::MoveValue} "HKCR\$0\ShellEx\${IID_IExtractImage}"      "Backup" "HKCR\$0\ShellEx\${IID_IExtractImage}"      "" $2
  ${registry::MoveValue} "HKCR\$0\ShellEx\${IID_IThumbnailProvider}" "Backup" "HKCR\$0\ShellEx\${IID_IThumbnailProvider}" "" $2

  DeleteRegKey HKLM "Software\HIRAOKA HYPERS TOOLS, Inc.\CmdThumbGen\FileExts\$0"

  ReadRegStr $1 HKCR "$0" ""
  ${If} $1 != ''
    ReadRegStr $2 HKCR "$1\CurVer" ""
    ${If} $2 != ''
      StrCpy $1 $2
    ${EndIf}

    DeleteRegValue HKCR "$1\ShellEx\${IID_IExtractImage}" ""
    DeleteRegValue HKCR "$1\ShellEx\${IID_IThumbnailProvider}" ""

    ${registry::MoveValue} "HKCR\$1\ShellEx\${IID_IExtractImage}"      "Backup" "HKCR\$1\ShellEx\${IID_IExtractImage}"      "" $2
    ${registry::MoveValue} "HKCR\$1\ShellEx\${IID_IThumbnailProvider}" "Backup" "HKCR\$1\ShellEx\${IID_IThumbnailProvider}" "" $2
  ${EndIf}
!macroend

Section "Uninstall"
  
  ; Remove files and uninstaller
  Delete "$INSTDIR\pdf2bmp.exe"
  Delete "$INSTDIR\pdf2bmp.pdb"
  Delete "$INSTDIR\pdftoppm.exe"

  Delete "$INSTDIR\pdf2bmp2.exe"
  Delete "$INSTDIR\pdf2bmp2.pdb"

  Delete "$INSTDIR\pdfpcnt.exe"
  Delete "$INSTDIR\pdfpcnt.pdb"
  Delete "$INSTDIR\pdfinfo.exe"

  Delete "$INSTDIR\dxf2bmp.exe"
  Delete "$INSTDIR\dxf2bmp.pdb"

  Delete "$INSTDIR\Jnt2bmp.exe"
  Delete "$INSTDIR\Jnt2bmp.pdb"
  Delete "$INSTDIR\AxInterop.NbDocViewerLib.dll"
  Delete "$INSTDIR\Interop.NbDocViewerLib.dll"

  ExecWait 'REGSVR32 /s /u "$INSTDIR\CmdThumbGen.dll"' $0
  DetailPrint "����: $0"

  Delete "$INSTDIR\CmdThumbGen.dll"
  Delete "$INSTDIR\CmdThumbGen.pdb"

  ExecWait 'REGSVR32 /s /u "$INSTDIR\x64\CmdThumbGen.dll"' $0
  DetailPrint "����: $0"

  Delete "$INSTDIR\x64\CmdThumbGen.dll"
  Delete "$INSTDIR\x64\CmdThumbGen.pdb"

  RMDir  "$INSTDIR\x64"

  ; Remove .pdf assoc ->
  !insertmacro RestoreAssoc ".pdf"
  ; <- Remove .pdf assoc

  ; Remove .dxf assoc ->
  !insertmacro RestoreAssoc ".dxf"
  ; <- Remove .dxf assoc

  ; Remove .jnt assoc ->
  !insertmacro RestoreAssoc ".jnt"
  ; <- Remove .jnt assoc

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}"
  DeleteRegKey HKLM "Software\${COM}\${APP}"

!include "Dlist.txt"
  RMDir "$INSTDIR\share"

  Delete "$INSTDIR\uninstall.exe"

  ; Remove directories used
  RMDir "$INSTDIR"

SectionEnd