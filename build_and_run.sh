#!/bin/bash
clear && cmake -S . -B ./build && cmake --build ./build
./build/src/dersbiander/dersbiander