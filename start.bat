@echo off

set PROJECT_NAME=Grimlock
set BUILD_DIR=build
set CONFIG=Debug

echo Configuring project with CMake...
cmake -S . -B %BUILD_DIR% -G "Visual Studio 17 2022"

IF ERRORLEVEL 1 (
    echo CMake configuration failed!
    exit /b 1
)

echo Building project...
cmake --build %BUILD_DIR% --config %CONFIG%

IF ERRORLEVEL 1 (
    echo Build failed
    exit /b 1
)

set EXE_PATH=%BUILD_DIR%\%CONFIG%\%PROJECT_NAME%.exe

if exist "%EXE_PATH%" (
    echo Running %PROJECT_NAME%...
    "%EXE_PATH%"
) else (
    echo Executable not found: %EXE_PATH%
    exit /b 1
)