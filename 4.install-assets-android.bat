@echo off
%SystemDrive%
set targetpath=%USERPROFILE%\AppData\LocalLow\vrmaker\vplayer\pkgs\Android

%VRMAKER_ROOT%\runpath\adb.exe devices
echo �������������ʾ�豸�Ѿ�����

pause

for /f %%i in ('dir "%targetpath%" /b /a-d /o-d') do ( 
  echo ���� %%i
  %VRMAKER_ROOT%\runpath\adb.exe push %targetpath%\%%i /mnt/sdcard/Android/data/com.vrmaker.vplayer/files/pkgs/Android/%%i
)
echo �������
pause

