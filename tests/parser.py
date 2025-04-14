import configparser
import sys

config = configparser.ConfigParser()

try:
    config.read(sys.argv[1])

    for section in config:
        print(f"[{section}]")
        for key in config[section]:
            print("  " + key, config[section][key])
except: pass