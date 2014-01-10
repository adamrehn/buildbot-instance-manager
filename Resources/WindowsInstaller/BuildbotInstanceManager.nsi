;; Buildbot Instance Manager Windows installer script
;;
;; -------
;;
;; Note that the file list contained in this script reflects
;; a MinGW-based build. The list of DLLs must be changed if
;; attempting to package an MSVC-based build. 
;;
;; -------
;;
;; Copy this file to the build directory, along with the
;; necessary Qt DLL files, and the following files from the
;; source tree:
;;
;;     - COPYING
;;     - Resources/icon.ico
;;     - Resources/WindowsInstaller/header.bmp

; Include the Modern UI 2 Header File
!include "MUI2.nsh"

; The name of the installer
Name "Buildbot Instance Manager"
BrandingText "Buildbot Instance Manager"

; The filename of the installer
OutFile "InstallBuildbotInstanceManager.exe"

; The default installation directory
InstallDir $PROGRAMFILES\BuildbotInstanceManager

; Request application privileges for Windows Vista and above
RequestExecutionLevel admin

; Default Settings
ShowInstDetails show
ShowUninstDetails show

;--------------------------------

; Installer Pages
!define MUI_ICON "icon.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "header.bmp"
!define MUI_HEADERIMAGE_RIGHT
!insertmacro MUI_PAGE_LICENSE "COPYING"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

; Uninstaller Pages
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Languages
!insertmacro MUI_LANGUAGE "English"

; Version Settings
!define APP_VERSION "1.0.0.0"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "Buildbot Instance Manager Installer"
VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "Buildbot Instance Manager Installer"
VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Adam Rehn"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright © 2014, Adam Rehn"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "Buildbot Instance Manager Installer"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" ${APP_VERSION}
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductVersion" ${APP_VERSION}
VIProductVersion ${APP_VERSION}

;--------------------------------

; Uninstaller instructions
Section "Uninstall"

  ; No reboot required
  SetRebootFlag false
  
  ; Delete the executable and dlls
  Delete $INSTDIR\BuildbotInstanceManager.exe
  Delete $INSTDIR\Qt5Core.dll
  Delete $INSTDIR\Qt5Gui.dll
  Delete $INSTDIR\Qt5Widgets.dll
  Delete $INSTDIR\icudt51.dll
  Delete $INSTDIR\icuin51.dll
  Delete $INSTDIR\icuuc51.dll
  Delete $INSTDIR\libgcc_s_dw2-1.dll
  Delete $INSTDIR\libstdc++-6.dll
  Delete $INSTDIR\libwinpthread-1.dll
  Delete $INSTDIR\platforms\qwindows.dll
  RMDir $INSTDIR\platforms
  
  ; Delete the uninstaller itself
  Delete $INSTDIR\uninstall.exe
  
  ; Delete the config files and directory
  Delete $APPDATA\BuildbotInstanceManager\BuildbotInstanceManager.ini
  
  ; Delete the installation directory
  RMDir $INSTDIR

SectionEnd

; Installer instructions
Section ""

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Install the executable and dlls
  File BuildbotInstanceManager.exe
  File Qt5Core.dll
  File Qt5Gui.dll
  File Qt5Widgets.dll
  File icudt51.dll
  File icuin51.dll
  File icuuc51.dll
  File libgcc_s_dw2-1.dll
  File libstdc++-6.dll
  File libwinpthread-1.dll
  File /r platforms
  
  ; Write the uninstaller
  WriteUninstaller $INSTDIR\uninstall.exe
  
SectionEnd
