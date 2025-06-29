@echo off
echo Creating QNX QEMU image...

REM Set QNX environment
set QNX_HOST=C:\QNX\qnx800\host\win64\x86_64
set QNX_TARGET=C:\QNX\qnx800\target\qnx
set PATH=%QNX_HOST%\usr\bin;%PATH%

REM Create image directory
mkdir qnx_image 2>nul
cd qnx_image

REM Create buildfile
echo [virtual=x86_64,bios +compress] qnx_dashboard.ifs > qnx_dashboard.build
echo. >> qnx_dashboard.build
echo [+script] startup-script = { >> qnx_dashboard.build
echo     export SYSNAME=nto >> qnx_dashboard.build
echo     export TERM=qansi >> qnx_dashboard.build
echo     export PATH=/proc/boot:/bin:/usr/bin >> qnx_dashboard.build
echo     export LD_LIBRARY_PATH=/proc/boot:/lib:/usr/lib >> qnx_dashboard.build
echo     procnto-smp-instr -v >> qnx_dashboard.build
echo     devc-con -e -n4 ^& >> qnx_dashboard.build
echo     devc-pty ^& >> qnx_dashboard.build
echo     Photon ^& >> qnx_dashboard.build
echo     sleep 2 >> qnx_dashboard.build
echo     pwm ^& >> qnx_dashboard.build
echo     /bin/dashboard --config /etc/dashboard.conf ^& >> qnx_dashboard.build
echo     [+session] sh ^& >> qnx_dashboard.build
echo } >> qnx_dashboard.build
echo. >> qnx_dashboard.build
echo [data=copy] >> qnx_dashboard.build
echo %QNX_TARGET%\x86_64\boot\sys\procnto-smp-instr >> qnx_dashboard.build
echo %QNX_TARGET%\x86_64\boot\sys\startup-bios >> qnx_dashboard.build
echo %QNX_TARGET%\x86_64\boot\sys\devc-con >> qnx_dashboard.build
echo %QNX_TARGET%\x86_64\boot\sys\devc-pty >> qnx_dashboard.build
echo %QNX_TARGET%\x86_64\usr\photon\bin\Photon >> qnx_dashboard.build
echo %QNX_TARGET%\x86_64\usr\photon\bin\pwm >> qnx_dashboard.build
echo %QNX_TARGET%\x86_64\bin\sh >> qnx_dashboard.build
echo %QNX_TARGET%\x86_64\lib\libc.so >> qnx_dashboard.build
echo %QNX_TARGET%\x86_64\lib\libm.so >> qnx_dashboard.build
echo %QNX_TARGET%\x86_64\usr\lib\libph.so >> qnx_dashboard.build
echo ..\01_dashboard_simulator\build\bin\qnx-dashboard-simulator-release-x86_64=/bin/dashboard >> qnx_dashboard.build
echo ..\01_dashboard_simulator\resources\config\dashboard.conf=/etc/dashboard.conf >> qnx_dashboard.build

REM Build the image
mkifs qnx_dashboard.build qnx_dashboard.ifs

echo QNX Dashboard image created: qnx_dashboard.ifs
echo Ready to launch with QEMU!
pause 