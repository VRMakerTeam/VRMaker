@echo off
set targetpath="%USERPROFILE%\AppData\LocalLow\vrmaker\vplayer\svm\"
MD %targetpath%
xcopy /E/Y svm %targetpath%
pause
