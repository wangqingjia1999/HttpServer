import os

# Check https://www.systutorials.com/docs/linux/man/1-siege/ for siege reference

siege_config_folder_path = os.environ['HOME'] + "/.siege/"

siege_config_file_path = siege_config_folder_path + "siege.conf"

siege_config_string = "limit = 255\ngmethod = HEAD\nprotocol = HTTP/1.1\nconcurrent = 1000\nbenchmark = true\nconnection = close\n"

if not os.path.exists(siege_config_file_path):
    print("create siege config file at: " + siege_config_file_path)

    # create siege config folder first
    if not os.path.exists(siege_config_folder_path):
        os.mkdir(siege_config_folder_path)

    # write siege config file
    with open(siege_config_file_path, "w") as siege_config:
        siege_config.write(siege_config_string)

os.system("siege -c1000 -t1M 127.0.0.1:80/?q=word")