@echo off

set "DEST_PATH=%~dp0bin\data"
set "SRC_PATH=%~dp0data"

robocopy %SRC_PATH% %DEST_PATH% /MIR /MT:8 /NJH /NJS /NC /NS /LOG:copy_data.log

echo 'Data Copied!
echo.

pause