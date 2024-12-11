#include <stdio.h>

#define MAX_LINE_LENGTH 256
#define MAX_KEY_LENGTH 100
#define MAX_VAL_LENGTH 150

typedef enum {
	INTEGER, STRING, 
} ValueType;

typedef struct data {
	char* key;

	ValueType type;
	union {
		char* s_value;
		int i_value;
	} value;

	struct data *next;
} Data;

typedef struct section {
	char* title;
	Data* head;

	struct section *next;
} Section;


typedef Section* Config;


void initConfig(Config* config) {
	config == NULL;
}

int parseConfig(Config* conf, char* path) {
	FILE* f = fopen(path, "r");
	char buffer[1024];
	int lineNumber = 1;

	while(fgets(buffer, sizeof(buffer), f) != NULL) {
		int i=0;
		int spacingFlag = 0;

		Data* data;
		line++;
		
		// eliminate leading spaces and tabs
		while(s[i] == ' ' || s[i] == '\t') i++;

		// case when the rest of the string is empty
		if(s[i] == '\0') continue;

		// parse the data key from the string
		char* start = buffer[i];
		while(s[i] != '=' && s[i] != ':') {
			if(s[i] != ' ' || s[i] == '\t') spacingFlag = 1;
			else if (s[i] == ';' || s[i] == '#') {
				printf("[Line %d] Unexpected end of line", lineNumber);
			}


			if(s[i] == '')
			// // check end of line('\n') or comment start('#' or ';')
			// if(s[i] == '\n' || s[i] == '#' || s[i] == ';') break;

			i++;
		}

		int keyWidth = buffer + i - start;
		printf("[Line %d] key width: %d\n", lineNumber, keyWidth);

		// // skip the character '=' or ':' found
		// i++;

		// // parse the value
		// // --> skip the leading spaces
		// while(s[i] == ' ' || s[i] == '\t') i++;

		// spacingFlag = 0;
		// while(s[i] != '\0') {
		// 	i++;
		// }
	}

	if(!feof(f)) {
		printf("Error while reading line %d", line);
		return -1;
	}


	return 0;
}

void printConfig(Config config) {
	if(config == NULL) return;

	Section* section = config;
	Data* current;

	while(section != NULL) {
		printf("[%s]", section->title);

		current = section->head;

		while(current != NULL) {
			printf("\t%s", current->key);

			current = current->next;
		}

		section = section->next;
	}
}


int main() {
	Config config;
	initConfig(&config);


	parseConfig(&config, "config.ini");


	printConfig(config);
}




// quand on a une ligne, comment est ce qu-on la parcourt


/*
' \t'
key=Value
    key=value
key = val
key 2 = value
key   =     val
key2 == valllllll
key3 = 13B
*/

remove all leading spacing characters
loop on the string until equal on colon is reach
once th