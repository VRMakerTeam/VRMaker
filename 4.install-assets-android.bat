@echo off
%SystemDrive%
set targetpath=%USERPROFILE%\AppData\LocalLow\vrmaker\vplayer\pkgs\Android

%VRMAKER_ROOT%\runpath\adb.exe devices
echo 如果有输出，则表示设备已经连接

pause

for /f %%i in ('dir "%targetpath%" /b /a-d /o-d') do ( 
  echo 拷贝 %%i
  %VRMAKER_ROOT%\runpath\adb.exe push %targetpath%\%%i /mnt/sdcard/Android/data/com.vrmaker.vplayer/files/pkgs/Android/%%i
)
echo 拷贝完成
pause

