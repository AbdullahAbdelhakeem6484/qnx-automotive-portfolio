@echo off
cls
echo ====================================================================
echo                QNX DASHBOARD SIMULATOR - QUICK DEMO
echo ====================================================================
echo.

cd /d "%~dp0"

echo [1/5] Building Dashboard Simulator...
make clean
make all

if not exist "console_dashboard.exe" if not exist "console_dashboard" (
    echo ERROR: Build failed! Check QNX environment.
    pause
    exit /b 1
)

echo.
echo [2/5] Dashboard built successfully!
echo.
echo Files created:
dir /b console_dashboard* gui_dashboard*

echo.
echo [3/5] Starting Console Dashboard Demo...
echo Press Ctrl+C to stop and continue to GUI demo
echo.
pause

echo Starting console dashboard...
console_dashboard.exe

echo.
echo [4/5] Starting GUI Dashboard Demo...
echo Press Ctrl+C to stop
echo.
pause

echo Starting GUI dashboard...
gui_dashboard.exe

echo.
echo [5/5] Demo Complete!
echo.
echo Project Structure:
tree /f /a

echo.
echo ====================================================================
echo          DEMO FINISHED - Ready for recording!
echo ====================================================================
pause 