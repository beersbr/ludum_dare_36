@echo off

rem get myself a shell
call "D:\Program Files (x86)\Microsoft Visual Studio 11.0\VC/vcvarsall" x64

mkdir ..\build
pushd ..\build

rem -Zi -- generate debug info
rem -FC -- use full paths

cl -FC -Zi ..\code\win32_handmade.cpp User32.lib Gdi32.lib

popd