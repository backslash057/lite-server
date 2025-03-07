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

void cprintf(char *s) {
	for(int i=0;;i++) {
		if(s[i] == '\0') {
			printf("\\0");
			break;
		}
		else if(s[i] == '\n') printf("\\n");
		else printf("%c", s[i]);
	}
	printf("\n");
}


void initConfig(Config* config) {
	config == NULL;
}

int parseConfig(Config* conf, char* path) {
	FILE* f = fopen(path, "r");
	char buf[1024];
	int lineNumber = 0;

	int start;
	int i;
	int spaceCounter;
	int size;
	
	while(fgets(buf, sizeof(buf), f) != NULL) {
		lineNumber++;

		printf("\n");
		printf("%d. ", lineNumber);
		cprintf(buf);

		i=0;

		Data *data;
		
		// eliminate leading spaces and tabs
		while(buf[i] == ' ' || buf[i] == '\t') i++;

		// case when the rest of the string is empty or commented
		if(buf[i] == '\n' || buf[i] == '\0' || buf[i] == '#' || buf[i]==';') continue;
		

		if(buf[i] == '[') { // Detects if the line is the start of a section
			printf("section detected. Parsing later")
		}
		else { // Detects if the line is a key-value pair
			// parse the data key from the string
			start = i;
			spaceCounter = 0;
			int quoteFlag = 0;

			while (buf[i] != '=' && buf[i]!='\n' && buf[i] != '\0' && buf[i]!=';' && buf[i]!='#') {
				if(buf[i] == '\'' || buf[i] == '"') {
					quoteFlag = -1;
					break;
				}
				if(buf[i] != ' ' && spaceCounter != 0) {
					spaceCounter = -1; // a non-space character was found after a space
					break;
				}
				else if(spaceCounter==' ') spaceCounter++;
				i++;
			}

			if(buf[i] == '\0' || buf[i]=='\n') {
				printf("Line %d: Unexpected end of line\n", lineNumber);
				continue;
			}
			
			if(buf[i]==';' || buf[i]=='#') {
				printf("Line %d: Unexpected comment. No value specified\n", lineNumber);
				continue;
			}
			
			if(quoteFlag == -1) {
				printf("Line %d: Unexpected quote within key string\n", lineNumber);
				continue;
			}

			if(spaceCounter == -1) {
				printf("Line %d: Unexpected space within key string\n", lineNumber);
				continue;
			}
			

			size = i - spaceCounter - start; //spaceCounter counted the number of spaces at end
			printf("Key found on line %d, size: %d characters\n", lineNumber, size);

			// skip the character '=' sign found
			i++;

			// parse the value
			// --> skip the leading spaces
			while(buf[i] == ' ' || buf[i] == '\t') i++;
			
			// case when the rest of the string is empty or commented
			if(buf[i] == '\n' || buf[i] == '\0' || buf[i] == '#' || buf[i]==';') {
				printf("Line %d: Unexpected comment. No value specified\n", lineNumber);
				continue;
			}

			start = i;
			spaceCounter = 0;
			quoteFlag = 0;
			// parse the value
			while(buf[i] != '\0' && buf[i]!='\n' && buf[i]!=';') {
				if((buf[i]=='"' || buf[i]=='\'') && quoteFlag==0) {
					quoteFlag++;
				}

				if(buf[i] == ' ') spaceCounter++;
				else spaceCounter = 0;
				i++;
			}

			size = i - spaceCounter - start; //spaceCounter counted the number of spaces at end
			printf("Value found on line %d, size: %d characters\n", lineNumber, size);
		}
	}

	if(!feof(f)) {
		printf("Error while reading line %d", lineNumber);
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


	parseConfig(&config, "temp.ini");


	printConfig(config);
}



/* Possible cases */
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

// remove all leading spacing characters
// loop on the string until equal on colon is reach
// once th