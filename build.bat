@echo off

REM Compile Target C Files
set PROJECT=gbroccoli
set SOURCES=main.c tileset_may.c

REM Generate Object Files
for %%i in (%SOURCES%) do (
    C:\SDK\gbz80-gb\2-1-5\bin\lcc.exe -c %%i
)

REM Link
C:\SDK\gbz80-gb\2-1-5\bin\lcc.exe -o %PROJECT%.gb %SOURCES:.c=.o%

REM Clean Up (Delete Object Files)
del *.o

echo Build completed.
