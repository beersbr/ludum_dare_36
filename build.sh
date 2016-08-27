#!/bin/bash

EXE_NAME="ludum_dare_36"
SOURCE_DIRECTORY="src/"
OUTPUT_DIRECTORY="bin/"
FRAMEWORK_DIRECTORY=$HOME"/frameworks/"
BUILD_TYPE="DEBUG"

clang++ -c $SOURCE_DIRECTORY"keyboard.cpp" -o keyboard.o -D $BUILD_TYPE -D EXE_NAME="\"$EXE_NAME\"" -std=c++11
clang++ -c $SOURCE_DIRECTORY"timer.cpp" -o timer.o -D $BUILD_TYPE -D EXE_NAME="\"$EXE_NAME\"" -std=c++11
clang++ -c $SOURCE_DIRECTORY"sprite.cpp" -o sprite.o -D $BUILD_TYPE -D EXE_NAME="\"$EXE_NAME\"" -std=c++11
clang++ -c $SOURCE_DIRECTORY"game_object.cpp" -o game_object.o -D $BUILD_TYPE -D EXE_NAME="\"$EXE_NAME\"" -std=c++11

clang++ -F $FRAMEWORK_DIRECTORY -framework SDL2 -framework OpenGL -framework SDL2_Image -D $BUILD_TYPE -D EXE_NAME="\"$EXE_NAME\"" \
-o $OUTPUT_DIRECTORY$EXE_NAME -g $SOURCE_DIRECTORY"main.cpp" keyboard.o timer.o game_object.o sprite.o -lglew -std=c++11

rm *.o