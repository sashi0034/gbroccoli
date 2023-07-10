@echo on
set FILE=%1

if not exist ".\%FILE%.png" (
    exit /b 1
)

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
    --deduplicate ^
    --name "TILESET_%FILE_UPPER%" ^
    .\%FILE%.png

.\img2gb\img2gb-1.1.0.exe tilemap ^
    --output-c-file="..\tilemap_%FILE%.c" ^
    --output-header-file="..\tilemap_%FILE%.h" ^
    .\output\%FILE%.gb.png ^
    --name "TILEMAP_%FILE_UPPER%" ^
    .\%FILE%.png