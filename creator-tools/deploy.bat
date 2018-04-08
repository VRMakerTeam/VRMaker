@echo off

set target=%1

if "%target%"=="" (
	echo "Usage: deploy.bat <Target>"
	pause
	exit
)

set work_dir=%VRMAKER_ROOT%\creator-%target%
cd %work_dir%

%QT_ROOT%\bin\rcc --binary res\%target%.qrc -o bin\%target%.rcc

MD %VRMAKER_ROOT%\creator-library\lib
MD %VRMAKER_ROOT%\creator-library\data
MD %VRMAKER_ROOT%\creator-library\res

copy /Y bin\%target%.lib %VRMAKER_ROOT%\creator-library\lib\
copy /Y bin\%target%_d.lib %VRMAKER_ROOT%\creator-library\lib\
xcopy /E/Y inc %VRMAKER_ROOT%\creator-library\inc\plugins\
xcopy /E/Y data  %VRMAKER_ROOT%\creator-library\data\
copy /Y res\*.ts %VRMAKER_ROOT%\creator-library\res\
copy /Y res\*.ui %VRMAKER_ROOT%\creator-library\res\
copy /Y bin\*.rcc %VRMAKER_ROOT%\creator-library\res\


