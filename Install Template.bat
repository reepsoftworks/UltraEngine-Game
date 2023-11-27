:: Run this as administrator.
@echo off
if not exist "%ULTRAENGINE%" (
	echo ERROR: Unable to find Ultra Engine installed on this PC!. Try running the editor first!
	pause
	EXIT /B 999
)

xcopy "%~dp0C++ Game" "%ULTRAENGINE%\Templates\C++ Game" /i /d /y /s
pause
