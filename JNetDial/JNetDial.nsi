;NSIS Modern User Interface version 1.69
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;Configuration

  ;General
  Name "JNetDial"
  OutFile "Release\JNetDial-0.2.0.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\JNetDial"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\JNetDial" ""

Var STARTMENU_FOLDER
Var MUI_TEMP

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\JNetDial"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "JNetDial"

  !insertmacro MUI_PAGE_STARTMENU JNetDial $STARTMENU_FOLDER
  
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section -"Required"
  SetOutPath "$INSTDIR"

  ;Store installation folder
  WriteRegStr HKCU "Software\Modern UI Test" "" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN JNetDial

    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section "Client" SecClient
  SetOutPath "$INSTDIR"

  File "Release\JNetDialClient.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN JNetDial
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Client.lnk" "$INSTDIR\JNetDialClient.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section "Server" SecServer
  SetOutPath "$INSTDIR"

  File "Release\JNetDialServer.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN JNetDial
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Server.lnk" "$INSTDIR\JNetDialServer.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecClient ${LANG_ENGLISH} "Install the JNetDial client."
  LangString DESC_SecServer ${LANG_ENGLISH} "Install the JNetDial server."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecClient} $(DESC_SecClient)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecServer} $(DESC_SecServer)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN STUFF HERE!

  Delete "$INSTDIR\JNetDialClient.exe"
  Delete "$INSTDIR\JNetDialServer.exe"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER JNetDial $MUI_TEMP

  Delete "$SMPROGRAMS\$MUI_TEMP\Client.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Server.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall.lnk"

  ;Delete empty start menu parent diretories
  StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"

  startMenuDeleteLoop:
    RMDir $MUI_TEMP
    GetFullPathName $MUI_TEMP "$MUI_TEMP\.."

    IfErrors startMenuDeleteLoopDone

    StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
  startMenuDeleteLoopDone:
  
  DeleteRegKey /ifempty HKCU "Software\JNetDial"

SectionEnd
