@echo off

set "SOURCE_FOLDER=%~dp0\src"
set "BUILD_FOLDER=%~dp0\build"
set "GENERATOR=Visual Studio 15 2017"

pushd %BUILD_FOLDER%
cmake -G "%GENERATOR%" %SOURCE_FOLDER%
popd

echo.
echo Generated!
echo.

pause