#!/bin/bash

EXE_NAME="ludum_dare_36"
SOURCE_DIRECTORY="src/"
OUTPUT_DIRECTORY="bin/"
FRAMEWORK_DIRECTORY=$HOME"/frameworks/"
BUILD_TYPE="DEBUG"

clang++ -c $SOURCE_DIRECTORY"keyboard.cpp" -o keyboard.o
clang++ -c $SOURCE_DIRECTORY"timer.cpp" -o timer.o

clang++ -F $FRAMEWORK_DIRECTORY -framework SDL2 -framework OpenGL -framework SDL2_Image -D $BUILD_TYPE -D EXE_NAME="\"$EXE_NAME\"" \
-o $OUTPUT_DIRECTORY$EXE_NAME -g $SOURCE_DIRECTORY"main.cpp" keyboard.o timer.o -lglew -std=c++11

rm *.o