@echo off

set "SOURCE_FOLDER=%~dp0\src"
set "BUILD_FOLDER=%~dp0\build"

setlocal enabledelayedexpansion
set "GENERATOR=Visual Studio 15 2017"
set TARGET=APPLICATION

if /i "%1" == "tools" (
    set TARGET=TOOLS

    if /i "%2" == "Win64" (
        set "GENERATOR=!GENERATOR! Win64"
    )
) else if /i "%1" == "Win64" (
    set "GENERATOR=!GENERATOR! Win64"

    if /i "%2" == "tools" (
        set TARGET=TOOLS
    )
)

endlocal & set "GENERATOR=%GENERATOR%" & set "TARGET=%TARGET%"

if not exist %BUILD_FOLDER% (
    mkdir %BUILD_FOLDER%
)

pushd %BUILD_FOLDER%
cmake -G "%GENERATOR%" %SOURCE_FOLDER% -D%TARGET%=TRUE -T v141
popd

echo.
echo %TARGET% solution generated!
echo.

pause