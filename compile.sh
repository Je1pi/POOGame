#!/bin/bash

g++ scr/*.cpp scr/Fases/*.cpp scr/ASCII_Engine/*.cpp scr/ASCII_Engine/Core/*.cpp scr/ASCII_Engine/utils/*.cpp  -o game -lncurses

if [ $? -eq 0 ]; then
    ./game
else
    echo "Erro na compilação."
fi