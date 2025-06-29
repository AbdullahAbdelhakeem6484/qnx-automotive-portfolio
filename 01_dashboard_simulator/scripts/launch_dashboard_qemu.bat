@echo off
echo Starting QNX Dashboard Simulator in QEMU...

REM Set QEMU path
set QEMU_PATH=C:\Program Files\qemu

REM Check if QEMU is installed
if not exist "%QEMU_PATH%\qemu-system-x86_64.exe" (
    echo ERROR: QEMU not found at %QEMU_PATH%
    echo Please install QEMU first:
    echo   1. Run PowerShell as Administrator
    echo   2. Run: choco install qemu -y
    echo   3. Or download from: https://www.qemu.org/download/#windows
    pause
    exit /b 1
)

REM Check if QNX image exists
if not exist "qnx_image\qnx_dashboard.ifs" (
    echo ERROR: QNX image not found
    echo Please run build_qnx_image.bat first
    pause
    exit /b 1
)

echo Launching QNX Dashboard Simulator...
echo =====================================
echo QEMU Controls:
echo - Ctrl+Alt+G: Release mouse
echo - Ctrl+Alt+F: Toggle fullscreen
echo - Ctrl+C: Quit QEMU
echo =====================================

REM Launch QNX with Dashboard
"%QEMU_PATH%\qemu-system-x86_64.exe" ^
    -m 1G ^
    -smp 4 ^
    -kernel qnx_image\qnx_dashboard.ifs ^
    -append "console=ttyS0,115200" ^
    -serial stdio ^
    -vga std ^
    -netdev user,id=net0 ^
    -device e1000,netdev=net0 ^
    -name "QNX Dashboard Simulator" ^
    -display gtk,zoom-to-fit=on

echo QNX Dashboard QEMU session ended.
pause 