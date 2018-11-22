@echo off

rem build
rmdir %~dp0\build /S /Q
mkdir build

rem bin
rmdir %~dp0\bin /S /Q
mkdir bin

echo.
echo Cleaned!
echo.

pause