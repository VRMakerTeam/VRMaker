@echo off
set targetpath="%USERPROFILE%\AppData\LocalLow\vrmaker\vplayer\pkgs\Android"

cd ..\tools
set toolspath="%cd%"

%toolspath%\adb.exe devices

%SystemDrive%

pause

for /f %%i in ('dir %targetpath% /b /a-d /o-d') do ( 
  echo copy %%i
  %toolspath%\adb.exe push %targetpath%\%%i /mnt/sdcard/Android/data/com.vrmaker.vplayer/files/pkgs/Android/%%i
)
echo finish
pause

