#include <stdio.h>

#define MAX_LINE_LENGTH 256
#define MAX_KEY_LENGTH 100
#define MAX_VAL_LENGTH 150

typedef enum {
	INTEGER, STRING
} ValueType;

typedef struct node {
	char* key;

	ValueType type;
	union {
		char* s_value;
		int i_value;
	} value;

	struct node *next;
} Node;

typedef struct section {
	char* title;
	Node* head;

	struct section *next;
} Section;

typedef struct {
	Section* sections;
} Config;

void initConfig(Config* config) {
	config->sections = NULL;
}

void parseConfig(Config* conf) {

}

void printConfig(Config config) {
	if(config.sections == NULL) return;

	// Section* section = config.sections;
	// Node* current;

	// while(section != NULL) {
	// 	printf("[%s]", section);

	// 	current = section->head;

	// 	while(current != NULL) {
	// 		printf("\t%s=%s")

	// 		current = current->next;
	// 	}

	// 	section = section->next;
	// }
}


int main() {
	Config config;

	initConfig(&config);

	parseConfig(&config);

	printConfig(config);
}