#!/bin/bash
# Build dependencies

# Clone googletest only if it does not exist
if [ ! -d "${PWD}/googletest" ] 
then
    echo "Clone google test framework from Github."
    git clone https://github.com/google/googletest
else
    echo "Google test already exists :)"
fi

# Install mysql cpp connector
sudo apt-get install libmysqlcppconn-dev

# Create build directory and change directory to it.
if [ ! -d "${PWD}/build" ] 
then
    echo "Create CMake build directory."
    mkdir build && cd build
fi

# generate cmake cache
cmake ..

# make it
make