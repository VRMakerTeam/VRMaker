@echo off
set targetpath=%USERPROFILE%\AppData\LocalLow\vrmaker\vplayer

md _package

xcopy /E/Y runpath _package\application\
del /Q _package\application\Qt5Cored.dll
del /Q _package\application\Qt5Guid.dll
del /Q _package\application\Qt5Multimediad.dll
del /Q _package\application\Qt5Networkd.dll
del /Q _package\application\Qt5Positioningd.dll
del /Q _package\application\Qt5PrintSupportd.dll
del /Q _package\application\Qt5Qmld.dll
del /Q _package\application\Qt5Quickd.dll
del /Q _package\application\Qt5QuickWidgetsd.dll
del /Q _package\application\Qt5Sqld.dll
del /Q _package\application\Qt5Svgd.dll
del /Q _package\application\Qt5WebChanneld.dll
del /Q _package\application\Qt5WebEngineWidgetsd.dll
del /Q _package\application\Qt5WebEngineCored.dll
del /Q _package\application\Qt5WebSocketsd.dll
del /Q _package\application\Qt5Widgetsd.dll
del /Q _package\application\QtWebEngineProcessd.exe

move /Y _package\application\app.exe _package\application\vrmaker.exe



xcopy /E/Y tools _package\tools\

md _package\installer
xcopy /E/Y player-app\svm _package\installer\svm\
move /Y _package\application\player\reviewer_cardboard.apk _package\installer\vplayer.apk

echo ..\tools\adb.exe install -r vplayer.apk > _package\installer\1.install-vplayer.bat
echo pause >> _package\installer\1.install-vplayer.bat

copy /Y player-app\install-svm.bat _package\installer\2.install-svm.bat
copy /Y player-app\push-svm.bat _package\installer\3.push-svm.bat
copy /Y pkg-maker\push-assets.bat _package\installer\4.push-assets.bat

pause