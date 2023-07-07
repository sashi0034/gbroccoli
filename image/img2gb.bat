@echo on
set file=%1

img2gb/img2gb-1.1.0.exe tileset ^
    --output-c-file=%file%.c ^
    --output-header-file=%file%.h ^
    --output-image=%file%.gb.png ^
    --deduplicate ^
    .\%file%.png
