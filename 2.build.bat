cd "%VRMAKER_ROOT%\creator-library"
del .\lib\plugin*
RD /Q/S res

cd "%VRMAKER_ROOT%\creator-pluginUnity"
start /WAIT .\compile-debug.bat
start /WAIT .\compile-release.bat
start /WAIT .\deploy.bat

cd "%VRMAKER_ROOT%\creator-pluginBucket"
start /WAIT .\compile-debug.bat
start /WAIT .\compile-release.bat
start /WAIT .\deploy.bat

cd "%VRMAKER_ROOT%\creator-pluginStore"
start /WAIT .\compile-debug.bat
start /WAIT .\compile-release.bat
start /WAIT .\deploy.bat

cd "%VRMAKER_ROOT%\creator-pluginBean"
start /WAIT .\compile-debug.bat
start /WAIT .\compile-release.bat
start /WAIT .\deploy.bat

cd "%VRMAKER_ROOT%\creator-pluginDiagram"
start /WAIT .\compile-debug.bat
start /WAIT .\compile-release.bat
start /WAIT .\deploy.bat

cd "%VRMAKER_ROOT%\creator-pluginScreenplay"
start /WAIT .\compile-debug.bat
start /WAIT .\compile-release.bat
start /WAIT .\deploy.bat

cd "%VRMAKER_ROOT%\creator-app"
start /WAIT .\compile-debug.bat
start /WAIT .\compile-release.bat
start /WAIT .\deploy.bat

cmd /k