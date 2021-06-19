#!/bin/bash

# Test 10000 concurrent requests.

siege_config_file_path=$HOME/.siege/siege.conf

# Check whether the siege configuration file exists.
if [ -e $siege_config_file_path ]
then 
    echo "find siege.config file"
else
    echo "siege.config doesn't exist."
    mkdir $HOME/.siege
    siege.config
fi

# Change configurations
echo "Set siege process's maximum opening fds to 10000, permission needed."
ulimit -n 10000

default_number_of_concurrent_users_config="concurrent = 25"
new_number_of_concurrent_users_config="concurrent = 10000"

default_number_of_threads="limit = 255"
new_number_of_threads="limit = 10000"

sed -i "s/$default_number_of_concurrent_users_config/$new_number_of_concurrent_users_config/" $siege_config_file_path
sed -i "s/$default_number_of_threads/$new_number_of_threads/" $siege_config_file_path
echo "Change siege's default number of concurrent users to 10000"

# Check https://www.systutorials.com/docs/linux/man/1-siege/ for siege usage
siege -c10000 -t5S -b 127.0.0.1:18135