#include "config.h"

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

int parseConfig(Config* conf, char* path) {
    FILE* f = fopen(path, "r");
    if (!f) {
        logMessage(LOG_WARNING, "Config", "Failed to open '%s'(%s). Skipping configuration", path, strerror(errno));
        return -1;
    }

    char buf[MAX_LINE_LENGTH];
    int lineNumber = 0;

    while (fgets(buf, sizeof(buf), f)) {
        lineNumber++;
        int i = 0;

        // Trim leading spaces
        while (isblank(buf[i])) i++;
        // skip empty or commented lines
        if (buf[i] == '\0' || buf[i] == '\n' || buf[i] == '#' || buf[i] == ';') continue;

        if (buf[i] == '[') {
            i++;
            int start = i;
            while (buf[i] != ']' && buf[i] != '\0' && buf[i] != '\n' && !isblank(buf[i])) i++;
            if (isblank(buf[i])) {
                logMessage(LOG_WARNING, "Config", "Line %d: Space characters not allowed within section name\n", lineNumber);
                continue;
            }
            if (buf[i] != ']') {
                logMessage(LOG_WARNING, "Config", "Line %d: Missing closing bracket for section declaration\n", lineNumber);
                continue;
            }
            
			Section* section = (Section*) malloc(sizeof(Section));
			section->next = *conf;
			section->title = malloc(sizeof(char)*(i-start+1));
			strncpy(section->title, buf+start, i-start);

			section->head = NULL;
			*conf = section;
        } else {
            if (*conf == NULL) {
                logMessage(LOG_WARNING, "Config", "Line %d: Key-value pair outside of any section\n", lineNumber);
                continue;
            }
            
            int spaceCounter = 0;
            int quoteFlag = 0;

            int key_start = i;
            
            while (buf[i] != '=' && buf[i]!='\n' && buf[i] != '\0' && buf[i]!=';' && buf[i]!='#') {
                if(buf[i] == '\'' || buf[i] == '"') {
                    quoteFlag = -1;
					break;
				}
				if(buf[i] != ' ' && spaceCounter != 0) {
                    spaceCounter = -1;
					break;
				}
				else if(isblank(buf[i])) spaceCounter++;
                else spaceCounter = 0;
				i++;
			}
            if(quoteFlag == -1) {
                logMessage(LOG_WARNING, "Config", "Line %d: Unexpected quote symbol within key string\n", lineNumber);
                continue;
            }
            if(spaceCounter == -1) {
                logMessage(LOG_WARNING, "Config", "Line %d: Unexpected spacing characters within key string\n", lineNumber);
                continue;
            }
            if (buf[i] != '=') {
                logMessage(LOG_WARNING, "Config", "Line %d: No value specified\n", lineNumber);
                continue;
            }
            int key_end = i - spaceCounter;

            i++; // Skip the separator

            while (isblank(buf[i])) i++; // Trim leading spaces
            ValueType type = INTEGER;

            if(buf[i] == '\'' || buf[i]=='"') {
                type = STRING;
                i++; // skip the quote and parse the string
            }

            
            int floatingPointFlag = 0;
            int val_start = i;
            while (buf[i] != '\0' && buf[i] != '\n' && buf[i] != ';' && buf[i] != '#') {
                if(type == STRING && buf[i] == buf[val_start-1]) break;
                if(buf[i] == '.'){
                    if(type != STRING && floatingPointFlag) {
                        floatingPointFlag = -1; // multiple dot detected
                        break;
                    }
                    floatingPointFlag = 1;
                }
                i++;
            };
            if(floatingPointFlag == -1) {
                logMessage(LOG_WARNING, "Config", "Line %d: Invalid floating point expression\n", lineNumber);
                continue;
            }
            int val_end = i;

            Value val;

            if(type == STRING) { // check for non blank characters after enclosing quote
                i++; // skip the enclosing quote detected
                int nonBlankFlag = 0;
                while(buf[i] != '\0' && buf[i] != '\n' && buf[i] != ';' && buf[i] != '#') {
                    if(!isblank(buf[i])) {
                        nonBlankFlag = 1;
                        break;
                    }
                    i++;
                }
                if(nonBlankFlag) {
                    logMessage(LOG_WARNING, "Config", "Line %d: Unexpected character after string value\n", lineNumber);
                    continue;
                }

                val.s_value = malloc(sizeof(char) * (val_end-val_start+1));
                strncpy(val.s_value, buf+val_start, val_end-val_start);
            }
            else { // If not string value, remove the spaces at the end
                while (val_end > val_start && isblank(buf[val_end - 1])) val_end--;
                buf[val_end]='\0';
                
                char* endptr;
                int failed = 0;
                if(floatingPointFlag) {
                    type = DOUBLE;
                    val.d_value = strtod(buf + val_start, &endptr);
                    if(*endptr!='\0' || buf[val_start]=='\0') failed = 1;
                }
                else {
                    val.i_value = strtol(buf + val_start, &endptr, 10);
                    if(*endptr!='\0' || buf[val_start]=='\0') failed = 1;
                }

                if(failed) {
                    logMessage(LOG_WARNING, "Config", "Line %d: Value parsing failed\n", lineNumber);
                    continue;
                }
            }
            
            Data* data = (Data*) malloc(sizeof(Data));
			data->key = malloc(sizeof(char) * (key_end-key_start+1));
			strncpy(data->key, buf+key_start, key_end-key_start);
            data->val = val;
            data->type = type;
			data->next = (*conf)->head;
			(*conf)->head = data;
		}
    }

    if (ferror(f)) {
        logMessage(LOG_WARNING, "Config", "Error while reading the file.\n");
        fclose(f);
        return -1;
    }

    fclose(f);
    return 0;
}

void printConfig(Config config) {
    Section* section = config;
    while (section != NULL) {
        printf("[%s]\n", section->title);
        Data* data = section->head;
        while (data != NULL) {
            printf("  %s = ",  data->key);

            if(data->type == STRING) printf("'%s'", data->val.s_value);
            else if(data->type == INTEGER) printf("%d", data->val.i_value);
            else printf("%g", data->val.d_value);

            printf("\n");
            data = data->next;
        }
        section = section->next;
    }
}

void freeConfig(Config config) {
    Section* section = config;
    while (section) {
        Section* nextSec = section->next;
        free(section->title);
        Data* data = section->head;
        while (data) {
            Data* nextData = data->next;
            free(data->key);
            if(data->type == STRING) free(data->val.s_value);
            free(data);
            data = nextData;
        }
        free(section);
        section = nextSec;
    }
}

int keyExists(Config config, char* section, char* key) {
    Section* current = config;

    while(current != NULL && strcmp(current->title, section) != 0) current = current->next;

    // section not found
    if(current != NULL) {
        Data* temp = current->head;
        while(temp != NULL && strcmp(temp->key, key) != 0) temp = temp->next; 
        if(temp != NULL) return 1;
    }

    return 0;
}


char* getString(Config config, char* section, char* key) {
    Section* current = config;

    while(current != NULL && strcmp(current->title, section) != 0) current = current->next;

    if(current != NULL) {
        Data* temp = current->head;
        while(temp != NULL && strcmp(temp->key, key) != 0) temp = temp->next; 
        if(temp != NULL && temp->type == STRING) return temp->val.s_value;
    }

    return NULL;
}

int getInt(Config config, char* section, char* key) {
    Section* current = config;

    while(current != NULL && strcmp(current->title, section) != 0) current = current->next;

    if(current != NULL) {
        Data* temp = current->head;
        while(temp != NULL && strcmp(temp->key, key) != 0) temp = temp->next; 
        if(temp != NULL && temp->type == INTEGER) return temp->val.i_value;
    }

    return 0;
}

double getDouble(Config config, char* section, char* key) {
    Section* current = config;

    while(current != NULL && strcmp(current->title, section) != 0) current = current->next;

    if(current != NULL) {
        Data* temp = current->head;
        while(temp != NULL && strcmp(temp->key, key) != 0) temp = temp->next; 
        if(temp != NULL && temp->type == DOUBLE) return temp->val.d_value;
    }

    return 0;
}



// int main() {
//     Config config = NULL;
//     parseConfig(&config, "config.ini");

//     printConfig(config);

//     return 0;
// }