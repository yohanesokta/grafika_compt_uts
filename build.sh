#!/usr/bin/env bash

set -e 

if [ "$1" = "setup" ]; then
    echo "Running setup..."
    mkdir -p build
    cd build
    cmake ..
    cd ..
else
    echo "Running build..."
    cd build
    cmake --build .
    cd ..
    if [ -n "$1" ]; then
        echo "Running Application"
        ./bin/$1
    fi
fi

echo "Done."