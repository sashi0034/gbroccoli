@echo on
set FILE=%1

if not exist output (
    mkdir output
) 
set FILE=%1

set FILE_UPPER=%1
for %%i in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do call ^
    set FILE_UPPER=%%FILE_UPPER:%%i=%%i%%
echo %FILE_UPPER%

.\img2gb\img2gb-1.1.0.exe tileset ^
    --output-c-file="..\tileset_%FILE%.c" ^
    --output-header-file="..\tileset_%FILE%.h" ^
    --output-image="output\%FILE%.gb.png" ^
    --sprite8x16 ^
    --alternative-palette ^
    --name "TILESET_%FILE_UPPER%" ^
    .\%FILE%.png
