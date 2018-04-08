@echo off

set target=%1

if "%target%"=="" (
	echo "Usage: compile-release.bat <Target>"
	pause
	exit
)

 
set work_dir=%VRMAKER_ROOT%\creator-%target%
cd %work_dir%
cd build\VS2015\
"c:\Program Files (x86)\MSBuild\14.0\Bin\amd64\msbuild" %target%.sln /p:Platform=x86;Configuration=Release

