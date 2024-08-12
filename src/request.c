#include "request.h"


int parseRequestURI(RequestURI* uri, char* s) {
	int waitingValue = 0;

	uri->path = s;

	for(int i=0; s[i] != '\0'; i++) {
		if(s[i] == '?') {
			s[i] = '\0';
			break;
		}
	}
	
	// parse request arguments if necessary

	return 0;
}

int parseRequestLine(HttpRequest* request, char* line) {
	int i=0, count=0;
	
	request->method = line;

	for(int i=0; line[i]!='\0'; i++) {
		if(line[i] == ' ') {
			line[i] = '\0';

			if(count==0) {
				if(parseRequestURI(&request->uri, line+i+1) < 0) return -1;
			}
			else if(count==1) request->httpVersion = line+i+1;

			count++;
		}
	}

	return 0;
}

void parseRequestHead(HttpRequest* request, char* head) {
	printf("%s\n", head);
}


void parseRequestBody(HttpRequest* request, char* body) {
	printf("%s\n", body);
}
