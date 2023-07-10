@echo off

call "./build.bat"

set EMU_DIR="C:\dev\VisualStudioRepos\GBEmu\GBEmu\App"
set EMU_FILE=".\GBEmu(debug).exe"

REM Execute emulator
cd %EMU_DIR%
call %EMU_FILE%
