@echo off
for %%i in (*) do ( 
if %%i equ replace_user_single_file.bat (
echo NO_file
pause
exit
)
for /d %%a in (*) do if %%a NEQ user (del "%~dp0%%a\user\%%i" /q)
for /d %%a in (*) do if %%a NEQ user (copy "%~dp0\%%i" "%~dp0%%a\user\%%i" /y)
del /q .\%%i
del /q .\replace_user_single_file.bat
exit
)
 
