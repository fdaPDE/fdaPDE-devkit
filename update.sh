#!/bin/sh

# the very first time
# git submodule add https://github.com/fdaPDE/fdaPDE-cpp.git
# git submodule update --init --recursive

## update fdaPDE-cpp submodule and its core submodule
git submodule update --recursive --remote --force

