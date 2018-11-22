@echo off

set "BUILD_FOLDER=%~dp0\build"

set CONFIG=RELEASE
if /i "%1" == "Debug" (
    set CONFIG=DEBUG
)

cmake --build build --target install --config %CONFIG%

pause