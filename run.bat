@echo off
REM Ripa na Xulipa - script de build + run (Windows + VS2022 + vcpkg)
setlocal
set CMAKE="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
set VCPKG=E:\Projetos\vcpkg\scripts\buildsystems\vcpkg.cmake

%CMAKE% -S . -B build -DCMAKE_TOOLCHAIN_FILE=%VCPKG% -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit /b 1

%CMAKE% --build build --config Release
if errorlevel 1 exit /b 1

echo.
echo === Executando jogo (working dir = build\Release) ===
cd build\Release
.\RipaNaXulipa.exe
