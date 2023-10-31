:: Run this as administrator.
@echo off
SET ULTRAENGINEPATH=C:\Program Files\Ultra Engine
if not exist "%ULTRAENGINEPATH%" goto Error_MissingLocation

xcopy "%~dp0C++ Game" "%ULTRAENGINEPATH%\Templates\C++ Game" /i /d /y /s
pause

:Error_MissingLocation
echo ERROR: Unable to find Ultra Engine installed on this PC!.
EXIT /B 999