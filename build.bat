@echo off


rem call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC/vcvarsall" x64

set EXE_NAME=ludum_dare_36

mkdir bin
pushd bin

rem -Zi -- generate debug info
rem -FC -- use full paths

cl -FC -Zi ../src/main.cpp /Iinclude/ lib /D DEBUG /D EXE_NAME=%EXE_NAME%  math.lib opengl32.lib SDL2.lib SDL2main.lib glew32.lib /out:%EXE_NAME%.exe
 
popd