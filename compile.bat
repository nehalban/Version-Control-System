@echo off
REM Windows batch script for compiling the file system

g++ -std=c++17 -Wall -Wextra -O2 main.cpp -o filesystem.exe

if %ERRORLEVEL% EQU 0 (
    echo Compilation successful!
    echo Run with: filesystem.exe
) else (
    echo Compilation failed!
    exit /b 1
)
