@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"

mkdir build 
cd build
cmake -G "NMake Makefiles" ..
nmake

copy /Y src\hello_world.uf2 ..\

cd ..\