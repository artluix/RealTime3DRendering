@echo off

rem build
rmdir %~dp0\build /S /Q
mkdir build

rem bin
rmdir %~dp0\bin /S /Q
mkdir bin

rem log
del %~dp0*.log /Q

echo.
echo Cleaned!
echo.

pause