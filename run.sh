#!/bin/bash

FILEPATH="coords.txt"
RADIUS=9

gcc -o main main.c -lm
./main  ${FILEPATH} ${RADIUS}