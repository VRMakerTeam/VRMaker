RD /Q/S runpath
MD runpath
MD tools

copy /Y c:\Windows\SysWOW64\msvcp140.dll runpath\
copy /Y c:\Windows\SysWOW64\vcruntime140.dll runpath\

copy /Y "%VRMAKER_ROOT%\creator-library\3rd\adb\adb.exe" runpath\
copy /Y "%VRMAKER_ROOT%\creator-library\3rd\adb\AdbWinApi.dll" runpath\
copy /Y "%VRMAKER_ROOT%\creator-library\3rd\adb\AdbWinUsbApi.dll" runpath\

copy /Y "%QT_ROOT%\bin\Qt5Core.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Cored.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Gui.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Guid.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Multimedia.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Multimediad.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Network.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Networkd.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Sql.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Sqld.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Svg.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Svgd.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Widgets.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Widgetsd.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Qml.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Qmld.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Quick.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Quickd.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5QuickWidgets.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5QuickWidgetsd.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5PrintSupport.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5PrintSupportd.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Positioning.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5Positioningd.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5WebChannel.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5WebChanneld.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5WebEngineCore.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5WebEngineCored.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5WebSockets.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5WebSocketsd.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5WebEngineWidgets.dll" runpath\
copy /Y "%QT_ROOT%\bin\Qt5WebEngineWidgetsd.dll" runpath\
copy /Y "%QT_ROOT%\bin\QtWebEngineProcess.exe" runpath\
copy /Y "%QT_ROOT%\bin\QtWebEngineProcessd.exe" runpath\
copy /Y "%QT_ROOT%\bin\libEGL.dll" runpath\
copy /Y "%QT_ROOT%\bin\libGLESv2.dll" runpath\
copy /Y "%QT_ROOT%\bin\opengl32sw.dll" runpath\

copy /Y "%QT_ROOT%\resources\icudtl.dat" runpath\
copy /Y "%QT_ROOT%\resources\qtwebengine_devtools_resources.pak" runpath\
copy /Y "%QT_ROOT%\resources\qtwebengine_resources.pak" runpath\
copy /Y "%QT_ROOT%\resources\qtwebengine_resources_100p.pak" runpath\
copy /Y "%QT_ROOT%\resources\qtwebengine_resources_200p.pak" runpath\

MD "%VRMAKER_ROOT%\creator-app\bin"

copy /Y "%QT_ROOT%\bin\QtWebEngineProcess.exe" "%VRMAKER_ROOT%\creator-app\bin\"
copy /Y "%QT_ROOT%\bin\QtWebEngineProcessd.exe" "%VRMAKER_ROOT%\creator-app\bin\"
copy /Y "%QT_ROOT%\resources\icudtl.dat" "%VRMAKER_ROOT%\creator-app\bin\"
copy /Y "%QT_ROOT%\resources\qtwebengine_devtools_resources.pak" "%VRMAKER_ROOT%\creator-app\bin\"
copy /Y "%QT_ROOT%\resources\qtwebengine_resources.pak" "%VRMAKER_ROOT%\creator-app\bin\"
copy /Y "%QT_ROOT%\resources\qtwebengine_resources_100p.pak" "%VRMAKER_ROOT%\creator-app\bin\"
copy /Y "%QT_ROOT%\resources\qtwebengine_resources_200p.pak" "%VRMAKER_ROOT%\creator-app\bin\"

MD runpath\plugins
MD runpath\plugins\imageformats
MD runpath\plugins\mediaservice
MD runpath\plugins\platforms
MD runpath\plugins\sqldrivers
MD runpath\plugins\audio

copy /Y "%QT_ROOT%\plugins\imageformats\qgif.dll" runpath\plugins\imageformats\
copy /Y "%QT_ROOT%\plugins\imageformats\qgifd.dll" runpath\plugins\imageformats\
copy /Y "%QT_ROOT%\plugins\imageformats\qico.dll" runpath\plugins\imageformats\
copy /Y "%QT_ROOT%\plugins\imageformats\qicod.dll" runpath\plugins\imageformats\
copy /Y "%QT_ROOT%\plugins\imageformats\qjpeg.dll" runpath\plugins\imageformats\
copy /Y "%QT_ROOT%\plugins\imageformats\qjpegd.dll" runpath\plugins\imageformats\

copy /Y "%QT_ROOT%\plugins\mediaservice\dsengine.dll" runpath\plugins\mediaservice\
copy /Y "%QT_ROOT%\plugins\mediaservice\dsengined.dll" runpath\plugins\mediaservice\
copy /Y "%QT_ROOT%\plugins\mediaservice\qtmedia_audioengine.dll" runpath\plugins\mediaservice\
copy /Y "%QT_ROOT%\plugins\mediaservice\qtmedia_audioengined.dll" runpath\plugins\mediaservice\

copy /Y "%QT_ROOT%\plugins\platforms\qwindows.dll" runpath\plugins\platforms\
copy /Y "%QT_ROOT%\plugins\platforms\qwindowsd.dll" runpath\plugins\platforms\

copy /Y "%QT_ROOT%\plugins\sqldrivers\qsqlite.dll" runpath\plugins\sqldrivers\
copy /Y "%QT_ROOT%\plugins\sqldrivers\qsqlited.dll" runpath\plugins\sqldrivers\

copy /Y "%QT_ROOT%\plugins\audio\qtaudio_windows.dll" runpath\plugins\audio\
copy /Y "%QT_ROOT%\plugins\audio\qtaudio_windowsd.dll" runpath\plugins\audio\

MD runpath\player
MD runpath\cache

cmd /k