@echo off

set "DEST_PATH=%~dp0bin\data"
set "SRC_PATH=%~dp0data"
set "TOOLS_PATH=%~dp0tools"

set TEXCONV_TOOL="%~dp0tools\bin\texconv\x64\texconv.exe"
set FXC_TOOL="C:\Program Files (x86)\Windows Kits\10\bin\10.0.17763.0\x64\fxc.exe"

setlocal enabledelayedexpansion

echo Start copy data:

set STEP=0

set /a STEP+=1
echo.
echo %STEP%. Compile Effects

set EFFECTS_DEST_PATH=%DEST_PATH%\effects
if not exist %EFFECTS_DEST_PATH% (
    mkdir %EFFECTS_DEST_PATH%
)

for %%f in (%SRC_PATH%\effects\*.fx) do (
    call :build_effect %%f %%~nf
)

:: export textures
set /a STEP+=1
echo.
echo %STEP%. Export Textures

set TEXTURES_DEST_PATH=%DEST_PATH%\textures
if not exist %TEXTURES_DEST_PATH% (
    mkdir %TEXTURES_DEST_PATH%
)

for %%f in (%SRC_PATH%\textures\*.*) do (
    call :export_texture %%f %%~nf %%~xf
)

:: copy fonts
set /a STEP+=1
echo.
echo %STEP%. Copy Fonts
xcopy %SRC_PATH%\fonts %DEST_PATH%\fonts /i /f /d

:: copy models
set /a STEP+=1
echo.
echo %STEP%. Copy models
xcopy %SRC_PATH%\models %DEST_PATH%\models /i /f /d

endlocal

:: finish
echo.
echo Data Copied!
echo.

pause
goto :eof


:build_effect
setlocal
set FX_PATH=%1
set FX_NAME=%2

set FX_DEST_PATH=%EFFECTS_DEST_PATH%\%FX_NAME%.fx
set FX_HASH_PATH=%FX_DEST_PATH%.hash

set FX_COMPILED_PATH=%EFFECTS_DEST_PATH%\%FX_NAME%.fxc
set FX_COMPILED_PATH_DEBUG=%EFFECTS_DEST_PATH%\%FX_NAME%_d.fxc

for /f %%i in ('certutil -hashfile %FX_PATH% ^| findstr /V ":"') do set FX_HASH=%%i
set HASH_CHANGED=True

if exist %FX_HASH_PATH% (
    set /p OLD_FX_HASH=<%FX_HASH_PATH%
    if "%FX_HASH%"=="!OLD_FX_HASH!" (
        set HASH_CHANGED=False
    )
)

set NEED_COPY_FX=%HASH_CHANGED%
set NEED_COMPILE_FX=%HASH_CHANGED%
set NEED_COMPILE_FX_DEBUG=%HASH_CHANGED%

if not exist %FX_DEST_PATH% set NEED_COPY_FX=True
if not exist %FX_COMPILED_PATH% set NEED_COMPILE_FX=True
if not exist %FX_COMPILED_PATH_DEBUG% set NEED_COMPILE_FX_DEBUG=True

if "%NEED_COPY_FX%"=="False" (
    if "%NEED_COMPILE_FX%"=="False" (
        if "%NEED_COMPILE_FX_DEBUG%"=="False" (
            echo Skipped %FX_PATH%
            exit /b 0
        )
    )
)

:: copy shader
if "%NEED_COPY_FX%"=="True" (
    copy %FX_PATH% %FX_DEST_PATH%
)

:: write shader hash
if "%HASH_CHANGED%"=="True" (
    echo %FX_HASH%>%FX_HASH_PATH%
)

:: debug shader
if "%NEED_COMPILE_FX_DEBUG%"=="True" (
    %FXC_TOOL% %FX_PATH% /T fx_5_0 /Zi /Od /Fo %EFFECTS_DEST_PATH%\%FX_NAME%_d.fxc /nologo
)

:: release shader
if "%NEED_COMPILE_FX%"=="True" (
    %FXC_TOOL% %FX_PATH% /T fx_5_0 /Fo %EFFECTS_DEST_PATH%\%FX_NAME%.fxc /O3 /nologo
)

exit /b 0


:export_texture
setlocal
set TEX_PATH=%1
set TEX_NAME=%2
set TEX_EXT=%3

set TEX_DEST_PATH=%TEXTURES_DEST_PATH%\%TEX_NAME%.dds
set TEX_HASH_PATH=%TEXTURES_DEST_PATH%\%TEX_NAME%%TEX_EXT%.hash

for /f %%i in ('certutil -hashfile %TEX_PATH% ^| findstr /V ":"') do set TEX_HASH=%%i
set HASH_CHANGED=True

if exist %TEX_HASH_PATH% (
    set /p OLD_TEX_HASH=<%TEX_HASH_PATH%
    if "%TEX_HASH%"=="!OLD_TEX_HASH!" (
        set HASH_CHANGED=False
    )
)

set NEED_EXPORT_TEX=%HASH_CHANGED%
if not exist %TEX_DEST_PATH% set NEED_EXPORT_TEX=True

if "%NEED_EXPORT_TEX%"=="False" (
    echo Skipped %TEX_PATH%
    exit /b 0
)

:: write texture hash
if "%HASH_CHANGED%"=="True" (
    echo %TEX_HASH%>%TEX_HASH_PATH%
)

if "%NEED_EXPORT_TEX%"=="True" (
    if /i "%TEX_EXT%" == ".dds" (
        copy %TEX_PATH% %TEX_DEST_PATH%
    ) else (
        echo Export texture %TEX_PATH%
        %TEXCONV_TOOL% -m 1 -ft dds %TEX_PATH% -o %TEXTURES_DEST_PATH% -nologo
        echo.
    )
)

exit /b 0


