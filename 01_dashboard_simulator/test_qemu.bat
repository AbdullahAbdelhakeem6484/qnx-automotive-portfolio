@echo off
cls
echo ===============================================================================
echo           QNX Dashboard Simulator - QEMU Testing
echo ===============================================================================

cd /d "%~dp0"

echo [1/3] Testing Dashboard Applications...
echo.

echo Testing Console Dashboard (5 seconds)...
start "Console Dashboard Test" cmd /c "echo Testing Console Dashboard && .\console_dashboard"
timeout /t 5 /nobreak >nul

echo.
echo Testing GUI Dashboard (5 seconds)...
start "GUI Dashboard Test" cmd /c "echo Testing GUI Dashboard && .\gui_dashboard"
timeout /t 5 /nobreak >nul

echo.
echo [2/3] Creating QNX Boot Script...
echo. > dashboard_boot.sh
echo #!/bin/sh >> dashboard_boot.sh
echo echo "QNX Dashboard Simulator Boot" >> dashboard_boot.sh
echo echo "Starting Dashboard Applications..." >> dashboard_boot.sh
echo ./console_dashboard ^& >> dashboard_boot.sh
echo sleep 2 >> dashboard_boot.sh
echo ./gui_dashboard >> dashboard_boot.sh

echo [OK] Boot script created

echo.
echo [3/3] Launching QEMU with Dashboard...
echo.
echo QEMU Configuration:
echo - Memory: 256MB
echo - Display: Console + VGA
echo - Boot: Custom dashboard script
echo.

echo [INFO] Starting QEMU Virtual Machine...
echo [INFO] You will see QNX booting with dashboard applications
echo.

qemu-system-x86_64 ^
    -m 256 ^
    -nographic ^
    -serial mon:stdio ^
    -kernel dashboard_boot.sh ^
    -append "console=ttyS0" ^
    -name "QNX Dashboard Test"

echo.
echo ===============================================================================
echo                    QEMU Testing Complete!
echo ===============================================================================
pause 