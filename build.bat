@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"

mkdir build 
cd build
cmake -G "NMake Makefiles" ..
nmake

copy /Y src\RGB_LED_TURN_SIGNAL.uf2 ..\

cd ..\
cmd /k