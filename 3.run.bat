xcopy /E/Y %VRMAKER_ROOT%\creator-library\res runpath\res\
%QT_ROOT%\bin\rcc --binary res\app.qrc -o runpath\res\app.rcc
%QT_ROOT%\bin\rcc --binary res\fonts.qrc -o runpath\res\fonts.rcc
xcopy /E/Y runtime\* runpath\

cd runpath
..\creator-app\bin\app.exe