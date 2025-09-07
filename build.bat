@echo off
setlocal

set "VCPKG_TOOLCHAIN=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"

if "%~1"=="" (
    echo No build preset provided.
    echo Usage: build.bat [clangdeb^|clangrel^|cldeb^|clrel]
    exit /b 1
)

if /i "%~1"=="clangdeb" (
    set "PRESET=x64-Clang-Debug"
) else if /i "%~1"=="clangrel" (
    set "PRESET=x64-Clang-Release"
) else if /i "%~1"=="cldeb" (
    set "PRESET=x64-Debug"
) else if /i "%~1"=="clrel" (
    set "PRESET=x64-Release"
) else (
    echo Invalid argument: %~1
    echo Usage: build.bat [clangdeb^|clangrel^|cldeb^|clrel]
    exit /b 1
)

if /i "%~2"=="clean" (
    echo Cleaning build directories...
    rd /s /q "build"
    if exist "out/main.exe" (
        rd /s /q "out/main.exe"
    )
    if exist "out/main_d.exe" (
        rd /s /q "out/main_d.exe"
    )
    if exist "out/main_d.pdb" (
        rd /s /q "out/main_d.pdb"
    )
    if exist "out/Debug" (
        rd /s /q "out/Debug"
    )
    if exist "out/Release" (
        rd /s /q "out/Release"
    )
    if %errorlevel% neq 0 (
        echo Failed to remove build directories.
        exit /b %errorlevel%
    ) else (
        echo Build directories cleaned.
    )
)

set "SOURCE_DIR=."

set "LOG_DIR=build\logs"
set "LOG_FILE=%LOG_DIR%\build_log.txt"

echo --- Starting CMake Configuration and Build ---
echo Using preset: %PRESET%
echo Source Directory: "%SOURCE_DIR%"
echo All CMake output will be logged to: "%LOG_FILE%"
echo.

if not exist "%LOG_DIR%" (
    mkdir "%LOG_DIR%"
    echo Created log directory: "%LOG_DIR%"
)

> "%LOG_FILE%" echo --- CMake Build Log - %DATE% %TIME% ---

echo --- Configuring CMake ---
echo --- Configuring CMake --- >> "%LOG_FILE%" 2>&1
cmake . -DCMAKE_TOOLCHAIN_FILE="%VCPKG_TOOLCHAIN%" --preset %PRESET% >> "%LOG_FILE%" 2>&1

if %errorlevel% neq 0 (
    echo.
    echo CMake configuration FAILED!
    echo Please check "%LOG_FILE%" for detailed errors.
    exit /b %errorlevel%
)

echo CMake configuration COMPLETED successfully.
echo.

echo --- Building Project ---
echo --- Building Project --- >> "%LOG_FILE%" 2>&1
cmake --build --preset %PRESET% >> "%LOG_FILE%" 2>&1

if %errorlevel% neq 0 (
    echo.
    echo Project build FAILED!
    echo Please check "%LOG_FILE%" for detailed errors.
    exit /b %errorlevel%
) else (
    echo.
    echo Project build COMPLETED successfully.
)

echo.
echo Build process finished. Full log available at "%LOG_FILE%".

endlocal