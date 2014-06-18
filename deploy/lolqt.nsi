!define VERSION "1.0.1"
!define APP "lolQt"
!define PUBLISHER "c't"

Name "${APP} ${VERSION}"
OutFile "${APP}-${VERSION}-setup.exe"
InstallDir $PROGRAMFILES\${APP}
InstallDirRegKey HKLM "Software\${PUBLISHER}\${APP}" "Install_Dir"
RequestExecutionLevel admin
SetCompressor lzma
ShowInstDetails show

Page license

  LicenseData ..\LICENSE

Page directory

Page instfiles


;Section "Visual Studio 2012 edistributables"
;  File D:\Developer\Qt-5.3\vcredist\vcredist_sp1_x86.exe
;SectionEnd

Section "lolQt"

;  ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\11.0\VC\VCRedist\x86" "Installed"
;  StrCmp $1 1 vcredist_installed
;  DetailPrint "Installing Visual Studio 2010 Redistributable (x86) ..."
;  ExecWait 'vcredist_sp1_x86.exe'
;vcredist_installed:

  SetOutPath $INSTDIR
  CreateDirectory $INSTDIR\plugins
  CreateDirectory $INSTDIR\plugins\imageformats
  CreateDirectory $INSTDIR\platforms
  CreateDirectory $INSTDIR\mediaservice
  CreateDirectory $INSTDIR\sampledata
  File v${VERSION}\lolQt.exe
  File v${VERSION}\lolQt.exe.embed.manifest
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
  File /oname=platforms\qminimal.dll D:\Developer\Qt-5.3\5.3\msvc2012_opengl\plugins\platforms\qminimal.dll
  File /oname=platforms\qwindows.dll D:\Developer\Qt-5.3\5.3\msvc2012_opengl\plugins\platforms\qwindows.dll
  File /oname=mediaservice\dsengine.dll D:\Developer\Qt-5.3\5.3\msvc2012_opengl\plugins\mediaservice\dsengine.dll
  File /oname=mediaservice\qtmedia_audioengine.dll D:\Developer\Qt-5.3\5.3\msvc2012_opengl\plugins\mediaservice\qtmedia_audioengine.dll
  File /oname=mediaservice\wmfengine.dll D:\Developer\Qt-5.3\5.3\msvc2012_opengl\plugins\mediaservice\wmfengine.dll
  WriteUninstaller $INSTDIR\uninstall.exe
SectionEnd


Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\${APP}"
  CreateShortCut "$SMPROGRAMS\${APP}\${APP} ${VERSION}.lnk" "$INSTDIR\${APP}.exe"
  CreateShortcut "$SMPROGRAMS\${APP}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
SectionEnd


Section "Uninstall"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}"
  DeleteRegKey HKLM SOFTWARE\${APP}

  Delete $INSTDIR\lolQt.exe
  Delete $INSTDIR\lolQt.exe.embed.manifest
  Delete $INSTDIR\LICENSE
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\icudt52.dll
  Delete $INSTDIR\icuin52.dll
  Delete $INSTDIR\icuuc52.dll
  Delete $INSTDIR\Qt5Concurrent.dll
  Delete $INSTDIR\Qt5Core.dll
  Delete $INSTDIR\Qt5Gui.dll
  Delete $INSTDIR\Qt5Multimedia.dll
  Delete $INSTDIR\Qt5Network.dll
  Delete $INSTDIR\Qt5Widgets.dll

  Delete $INSTDIR\mediaservice\dsengine.dll
  Delete $INSTDIR\mediaservice\qtmedia_audioengine.dll
  Delete $INSTDIR\mediaservice\wmfengine.dll
  RMDir $INSTDIR\mediaservice

  Delete $INSTDIR\platforms\qminimal.dll
  Delete $INSTDIR\platforms\qwindows.dll
  RMDir $INSTDIR\platforms

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
