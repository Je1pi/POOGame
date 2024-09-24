#!/bin/bash

g++ src/*.cpp src/Core/*.cpp src/Fases/*.cpp src/ASCII_Engine/*.cpp src/ASCII_Engine/Core/*.cpp src/ASCII_Engine/utils/*.cpp  -o game && ./game || echo "Erro na compilação."