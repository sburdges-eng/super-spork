@echo off
REM JUCE Audio Player - Build Script for Windows
REM This script automates the build process

echo ================================================
echo   JUCE Audio Player - Automated Build Script
echo ================================================
echo.

REM Check if CMake is installed
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: CMake is not installed!
    echo.
    echo Please install CMake from: https://cmake.org/download/
    echo Make sure to add CMake to your PATH during installation.
    echo.
    pause
    exit /b 1
)

echo CMake found!
echo.

REM Create build directory if it doesn't exist
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

REM Run CMake configuration
echo Configuring project with CMake...
echo.

cmake ..

if %errorlevel% neq 0 (
    echo.
    echo ERROR: CMake configuration failed!
    echo Check the error messages above.
    echo.
    pause
    exit /b 1
)

echo.
echo Configuration complete!
echo.

REM Build the project
echo Building project (this may take 5-10 minutes on first build)...
echo.

cmake --build . --config Release

if %errorlevel% neq 0 (
    echo.
    echo ERROR: Build failed!
    echo Check the error messages above.
    echo.
    pause
    exit /b 1
)

echo.
echo ================================================
echo   BUILD SUCCESSFUL!
echo ================================================
echo.

echo The app is located at: build\Release\AudioPlayer.exe
echo.

REM Ask if user wants to run the app
set /p run="Would you like to run the app now? (y/n) "
if /i "%run%"=="y" (
    start Release\AudioPlayer.exe
)

pause
