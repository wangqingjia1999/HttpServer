#!/bin/bash
# Build dependencies

# Clone googletest only if it does not exist
if [ ! -d "${PWD}/googletest" ] 
then
    echo "Google test does not exist."
    echo "Clone google test framework from Github."
    git clone --depth=1 https://github.com/google/googletest 
else 
    echo "Google test already exists :)"
fi 

# Install mysql and cpp connector
sudo apt update
sudo apt install cmake

# Create build directory and change directory to it.
if [ -d "${PWD}/build" ] 
then
    echo "Delete build directory"
    rm -rf build
else
    echo "Create CMake build directory."
fi

mkdir build && cd build

# generate cmake cache
cmake ..

# make it
make
