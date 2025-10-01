@echo off
REM VulnScan Build Script for Windows with Qt 6.9.1 and MinGW 13.1.0

echo ========================================
echo VulnScan Build Script
echo ========================================

REM Set Qt and MinGW paths
set QT_DIR=C:\Qt\6.9.1\mingw_64
set MINGW_DIR=C:\Qt\Tools\mingw1310_64
set PATH=%MINGW_DIR%\bin;%QT_DIR%\bin;%PATH%

echo Qt Directory: %QT_DIR%
echo MinGW Directory: %MINGW_DIR%
echo.

REM Check if compilers are available
echo Checking for qmake...
where qmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: qmake not found in PATH
    exit /b 1
)

echo Checking for g++...
where g++ >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: g++ not found in PATH
    exit /b 1
)

echo.
echo Compilers found:
qmake --version
g++ --version
echo.

REM Create build directory
if not exist build mkdir build
cd build

REM Run qmake
echo Running qmake...
qmake .. -spec win32-g++ "CONFIG+=debug"
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: qmake failed
    cd ..
    exit /b 1
)

echo.
echo qmake completed successfully!
echo.

REM Run make
echo Running mingw32-make...
mingw32-make -j4
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Build failed
    cd ..
    exit /b 1
)

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo.
echo Binaries location:
dir /b debug\*.exe 2>nul
echo.

cd ..
