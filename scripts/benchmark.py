import os
import fileinput

# Check https://www.systutorials.com/docs/linux/man/1-siege/ for siege reference

siege_config_folder_path = os.environ['HOME'] + "/.siege/"

siege_config_file_path = siege_config_folder_path + "siege.conf"

if not os.path.exists(siege_config_file_path):
    print("create siege config file at: " + siege_config_file_path)

    # create siege config folder first
    if not os.path.exists(siege_config_folder_path):
        os.mkdir(siege_config_folder_path)

    # generate default siege config file
    os.system("siege.config")

# change default config
for line in fileinput.input([siege_config_file_path], inplace=True):
    if "gmethod = HEAD\n" in line:
        print(line.replace('gmethod = HEAD', 'gmethod = GET'), end='')
        continue
    if "verbose = false\n" in line:
        print(line.replace('verbose = false', 'verbose = true'), end='')
        continue
    if "chunked = true\n" in line:
        print(line.replace('chunked = true', 'chunked = false'), end='')
        continue
    if "concurrent = 25\n" in line:
        print(line.replace('concurrent = 25', 'concurrent = 255'), end='')
        continue
    if "benchmark = false\n" in line:
        print(line.replace('benchmark = false', 'benchmark = true'), end='')
        continue
    print(line, end='')

os.system("siege -t1M -u 127.0.0.1:40000/?q=word")