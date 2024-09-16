#!/bin/bash

g++ src/*.cpp src/Fases/*.cpp src/ASCII_Engine/*.cpp src/ASCII_Engine/Core/*.cpp src/ASCII_Engine/utils/*.cpp  -o game -lncurses

if [ $? -eq 0 ]; then
    ./game
else
    echo "Erro na compilação."
fi