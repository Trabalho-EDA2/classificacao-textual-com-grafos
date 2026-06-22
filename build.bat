@echo off
setlocal

cd /d "%~dp0"

if not exist bin mkdir bin

set "UCRT=C:\msys64\ucrt64\bin"
if not exist "%UCRT%\g++.exe" set "UCRT=C:\msys64\mingw64\bin"
if not exist "%UCRT%\g++.exe" (
  echo g++ nao encontrado. Instale MSYS2 UCRT64.
  exit /b 1
)

set "PATH=%UCRT%;%PATH%"
set "GPP=%UCRT%\g++.exe"

echo Compilando com: %GPP%
"%GPP%" -std=c++17 -Wall -Isrc src\main.cpp src\tokenizer.cpp src\dataset.cpp src\graph.cpp src\queue.cpp -o bin\app.exe
if errorlevel 1 (
  echo Erro de compilacao.
  exit /b 1
)

echo OK: bin\app.exe
