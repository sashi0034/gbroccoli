@echo on
set file=%1

if not exist output (
    mkdir output
) 

.\img2gb\img2gb-1.1.0.exe tileset ^
    --output-c-file="output\%file%.set.c" ^
    --output-header-file="output\%file%.set.h" ^
    --output-image="output\%file%.gb.png" ^
    --sprite8x16 ^
    --alternative-palette ^
    .\%file%.png

@REM .\img2gb\img2gb-1.1.0.exe tilemap ^
@REM     --output-c-file="output\%file%.map.c" ^
@REM     --output-header-file="output\%file%.map.h" ^
@REM     "output\%file%.gb.png" ^
@REM     .\%file%.png
