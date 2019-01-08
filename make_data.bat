@echo off

set "DEST_PATH=%~dp0bin\data"
set "SRC_PATH=%~dp0data"

set TEXCONV_TOOL="%~dp0tools\bin\texconv\x64\texconv.exe"
set FXC_TOOL="C:\Program Files (x86)\Windows Kits\10\bin\10.0.17763.0\x64\fxc.exe"

setlocal enabledelayedexpansion

echo Start copy data:

set STEP=0
:: clean old data
if exist %DEST_PATH% (
    set /a STEP+=1
    echo.
    echo !STEP!. Clean old data
    rmdir /S /Q %DEST_PATH%
)

:: export shaders
set /a STEP+=1
echo.
echo %STEP%. Compile Effects

set EFFECTS_DEST_PATH=%DEST_PATH%\effects
mkdir %EFFECTS_DEST_PATH%

:: copy shaders sources
xcopy %SRC_PATH%\effects\*.fx %EFFECTS_DEST_PATH% /f

for %%f in (%SRC_PATH%\effects\*.fx) do (
    :: debug shader
    %FXC_TOOL% %%f /T fx_5_0 /Zi /Od /Fo %EFFECTS_DEST_PATH%\%%~nf_d.fxc /nologo

    :: release shader
    %FXC_TOOL% %%f /T fx_5_0 /Fo %EFFECTS_DEST_PATH%\%%~nf.fxc /O3 /nologo
)

:: export textures
set /a STEP+=1
echo.
echo %STEP%. Export Textures

set TEXTURES_DEST_PATH=%DEST_PATH%\textures
mkdir %TEXTURES_DEST_PATH%

echo.
echo %STEP%.1 Copy DDS textures
xcopy %SRC_PATH%\textures\*.dds %TEXTURES_DEST_PATH% /f

echo.
echo %STEP%.2 Export other textures to DDS
for %%f in (%SRC_PATH%\textures\*.*) do (
    if /i "%%~xf" neq ".dds" (
        echo Export texture %%f
        %TEXCONV_TOOL% -ft dds %%f -o %TEXTURES_DEST_PATH% -nologo
        echo.
    )
)

:: copy fonts
set /a STEP+=1
echo.
echo %STEP%. Copy Fonts
xcopy %SRC_PATH%\fonts %DEST_PATH%\fonts /i /f

:: copy models
set /a STEP+=1
echo.
echo %STEP%. Copy models
xcopy %SRC_PATH%\models %DEST_PATH%\models /i /f

endlocal

:: finish
echo.
echo Data Copied!
echo.

pause