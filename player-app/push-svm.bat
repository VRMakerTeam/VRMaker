@echo off
setlocal enabledelayedexpansion

set targetpath=%USERPROFILE%\AppData\LocalLow\vrmaker\vplayer\svm
..\tools\adb.exe devices
pause

for /f %%i in ('dir /B/S "%targetpath%"\*.out') do ( 
  set "var=%%i"
  set "var=!var:%targetpath%\=!"
  set "var=!var:\=/!"
  echo copy %%i to /mnt/sdcard/Android/data/com.vrmaker.vplayer/files/svm/!var!
  ..\tools\adb.exe push %%i /mnt/sdcard/Android/data/com.vrmaker.vplayer/files/svm/!var!
)
echo finish
pause


