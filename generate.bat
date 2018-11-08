@echo off

set "SOURCE_FOLDER=%~dp0\src"
set "BUILD_FOLDER=%~dp0\build"
set "GENERATOR=Visual Studio 15 2017"

set TARGET=APPLICATION
if "%1" == "tools" (
    set TARGET=TOOLS
)

if NOT exist %BUILD_FOLDER% (
    mkdir %BUILD_FOLDER%
)

pushd %BUILD_FOLDER%
cmake -G "%GENERATOR%" %SOURCE_FOLDER% -D%TARGET%=TRUE -T v141
popd

echo.
echo %TARGET% solution generated!
echo.

pause