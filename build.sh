#!/bin/bash

install_package_if_not_exists()
{
    if dpkg -s $1 | grep -q "Status: install ok installed";
    then
        echo "$1: Already Installed."
    else
        echo "$1: Not Installed, Install It Now."
        sudo apt install $1
    fi
}

install_package_if_not_exists cmake
install_package_if_not_exists sqlite3
install_package_if_not_exists libsqlite3-dev
install_package_if_not_exists libz-dev
install_package_if_not_exists redis-server

if [ ! -d "${PWD}/googletest" ] 
then
    echo "Clone googletest framework."
    git clone --depth=1 https://github.com/google/googletest 
else 
    echo "Googletest Installed."
fi 

if [ ! -d "${PWD}/build" ] 
then
    mkdir build && cd build
    cmake ..
    make
fi

if [ ! -d "${PWD}/logs" ] 
then
    mkdir logs
fi

echo "DONE!"