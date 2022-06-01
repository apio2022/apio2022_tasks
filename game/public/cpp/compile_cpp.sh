#!/bin/bash

TASK=game

g++ -std=gnu++17 -Wall -O2 -static -o ${TASK} grader.cpp ${TASK}.cpp
