xcopy /E/Y %VRMAKER_ROOT%\creator-library\res runpath\res\
xcopy /E/Y runtime\* runpath\

cd runpath
copy /Y ..\creator-app\bin\app.exe .\
app.exe