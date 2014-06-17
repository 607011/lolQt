!define VERSION "1.0.0"
!define APP "lolqt"
!define PUBLISHER "c't"

!include x64.nsh

Name "${APP} ${VERSION}"
OutFile "${APP}-${VERSION}-setup.exe"
InstallDir $PROGRAMFILES\${APP}-${VERSION}
InstallDirRegKey HKLM "Software\${PUBLISHER}\${APP}-${VERSION}" "Install_Dir"
RequestExecutionLevel admin
SetCompressor lzma
ShowInstDetails show

Page license

  LicenseData ..\LICENSE

Page directory

Page instfiles


Section "Visual Studio 2012 edistributables"
  File D:\Developer\Qt-5.3\vcredist\vcredist_sp1_x86.exe
SectionEnd

Section "lolqt"

;  ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\11.0\VC\VCRedist\x86" "Installed"
;  StrCmp $1 1 vcredist_installed
;  DetailPrint "Installing Visual Studio 2010 Redistributable (x86) ..."
;  ExecWait 'vcredist_sp1_x86.exe'
vcredist_installed:

  SetOutPath $INSTDIR
  CreateDirectory $INSTDIR\plugins
  CreateDirectory $INSTDIR\plugins\imageformats
  CreateDirectory $INSTDIR\sampledata
  File v${VERSION}\lolqt.exe
  File v${VERSION}\lolqt.exe.embed.manifest
  File D:\Developer\Qt-5.3\5.3\msvc2012_opengl\bin\icudt52.dll
  File D:\Developer\Qt-5.3\5.3\msvc2012_opengl\bin\icuin52.dll
  File D:\Developer\Qt-5.3\5.3\msvc2012_opengl\bin\icuuc52.dll
  File D:\Developer\Qt-5.3\5.3\msvc2012_opengl\bin\Qt5Concurrent.dll
  File D:\Developer\Qt-5.3\5.3\msvc2012_opengl\bin\Qt5Core.dll
  File D:\Developer\Qt-5.3\5.3\msvc2012_opengl\bin\Qt5Gui.dll
  File D:\Developer\Qt-5.3\5.3\msvc2012_opengl\bin\Qt5Multimedia.dll
  File D:\Developer\Qt-5.3\5.3\msvc2012_opengl\bin\Qt5Network.dll
  File D:\Developer\Qt-5.3\5.3\msvc2012_opengl\bin\Qt5Widgets.dll
  File ..\LICENSE
  File /oname=sampledata\cat.gif sampledata\thirsty-cat.gif
  File /oname=plugins\imageformats\qgif.dll D:\Developer\Qt-5.3\5.3\msvc2012_opengl\plugins\imageformats\qgif.dll
  WriteUninstaller $INSTDIR\uninstall.exe
SectionEnd


Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\${APP}"
  CreateShortCut "$SMPROGRAMS\${APP}\${APP} ${VERSION}.lnk" "$INSTDIR\lolqt.exe"
  CreateShortcut "$SMPROGRAMS\${APP}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
SectionEnd


Section "Uninstall"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\lolqt"
  DeleteRegKey HKLM SOFTWARE\lolqt
  Delete $INSTDIR\lolqt.exe
  Delete $INSTDIR\LICENSE
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\lolqt.exe.embed.manifest
  Delete $INSTDIR\icudt52.dll
  Delete $INSTDIR\icuin52.dll
  Delete $INSTDIR\icuuc52.dll
  Delete $INSTDIR\Qt5Concurrent.dll
  Delete $INSTDIR\Qt5Core.dll
  Delete $INSTDIR\Qt5Gui.dll
  Delete $INSTDIR\Qt5Multimedia.dll
  Delete $INSTDIR\Qt5Network.dll
  Delete $INSTDIR\Qt5Widgets.dll
  Delete $INSTDIR\plugins\imageformats\qgif.dll
  RMDir $INSTDIR\plugins\imageformats
  RMDir $INSTDIR\plugins
  Delete $INSTDIR\sampledata\thirsty-cat.gif
  RMDir $INSTDIR\sampledata
  RMDir $INSTDIR
  Delete "$SMPROGRAMS\lolqt\*.*"
  RMDir "$SMPROGRAMS\lolqt"
  RMDir "$INSTDIR"
SectionEnd
