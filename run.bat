@echo off
REM Ripa na Xulipa - build + run (Windows, sem vcpkg obrigatorio).
REM Usa o CMake do VS2022 Build Tools se existir, senao o do PATH.
REM Deps SDL: se houver vcpkg (VCPKG_ROOT), usa; senao baixa os
REM pacotes -devel oficiais automaticamente (ver cmake/Dependencies.cmake).
setlocal
set CMAKE=cmake
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (
  set CMAKE="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
)

set TOOLCHAIN=
if defined VCPKG_ROOT (
  if exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
    echo Usando vcpkg em %VCPKG_ROOT%
    set TOOLCHAIN=-DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows
  )
)

%CMAKE% -S . -B build %TOOLCHAIN% -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit /b 1

%CMAKE% --build build --config Release
if errorlevel 1 exit /b 1

if "%1"=="--no-run" exit /b 0

echo.
echo === Executando jogo (working dir = build\Release) ===
cd build\Release
.\RipaNaXulipa.exe
