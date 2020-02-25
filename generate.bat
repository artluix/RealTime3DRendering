@echo off

set "SOURCE_FOLDER=%~dp0\src"
set "BUILD_FOLDER=%~dp0\build"

set "GENERATOR=Visual Studio 16 2019"

set TARGET=APPLICATION
set PLATFORM=

if /i "%1" == "tools" (
    set TARGET=TOOLS

    if /i "%2" == "x64" (
        set "PLATFORM= Win64"
    )
) else if /i "%1" == "x64" (
    set "PLATFORM= Win64"

    if /i "%2" == "tools" (
        set TARGET=TOOLS
    )
)

if not exist %BUILD_FOLDER% (
    mkdir %BUILD_FOLDER%
)

pushd %BUILD_FOLDER%
cmake -G "%GENERATOR%%PLATFORM%" %SOURCE_FOLDER% -D%TARGET%=TRUE
popd

echo.
echo %TARGET% solution generated!
echo.

pause