@echo off
set OPENOCD_DIR=..\openocd_picoprobe
cd %OPENOCD_DIR%\
call openocd.exe -f tcl\interface\picoprobe.cfg -f tcl\target\rp2040.cfg -s tcl