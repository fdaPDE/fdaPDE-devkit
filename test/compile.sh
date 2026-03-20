#!/bin/bash

testing=$HOME/nonlinear-strpde
cpp=$HOME/fdaPDE-cpp
core=$cpp/fdaPDE/core
eigen=/usr/include/eigen3
include=../include
lbfgspp=/usr/include/LBFGSpp/include

exe=$(basename -- $1 .cpp)

if [ -f $exe ]; then rm $exe; fi
g++ -o $exe $1 -I$cpp -I$core -I$eigen -I$lbfgspp -I$include -O2 -std=c++20 -march=native -s
