import configparser

config = configparser.ConfigParser()

try:
    print(os.listdir())# Parse an INI file

    config.read('config.ini')

    # Access sections and keys
    print(config['Section']['Key'])  # Access specific value

except KeyError:
    print("Error: Missing key or section.")
except configparser.ParsingError as e:
    print(f"Error: Parsing failed - {e}")
except Exception as e:
    print(f"General error: {e}")
