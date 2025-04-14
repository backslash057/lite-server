#include "request.h"


void handleRequest(HttpRequest request, HttpResponse* response) {
	response->httpVersion = request.httpVersion;
	
	if(request.badRequestFlag) {
		response->code = 400;
		response->body = calloc(1, sizeof(char)); // empty string
		return;
	}

	/*

	recuperer le path
	verifier si c'est un fichier qui est demandé
	si oui
		lire le fichier et l'ecrire dans le socket
	si non
		lire le template et ecrire


	*/

	response->code = 200;

	response->body = calloc(100, sizeof(char));
	sprintf(response->body, "You requested %s", request.uri.path);
}

int parseRequestLine(HttpRequest* request, char* line) {
    request->method = strtok(line, " ");
    if (!request->method) return -1;

    char* uriString = strtok(NULL, " ");
    if (!uriString) return -1;
    int ret = parseRequestURI(&request->uri, uriString);
	if(ret < 0) return ret;

    char* version = strtok(NULL, "\r\n");
    if (!version) return -1;
    request->httpVersion = version;

    return 0;
}

int parseRequestURI(RequestURI* uri, char* uriString) {
    if (!uriString) return -1;

    char* queryStart = strchr(uriString, '?');
    if (queryStart) {
        *queryStart = '\0';
        queryStart++;
    }

    uri->path = uriString;
    if (!uri->path || strlen(uri->path) == 0) return -1;

    // TODO: verify if the url is 'url_safe'
	
	// TODO: parse request in url arguments using queryStart pointer

    return 0;
}

int parseRequest(HttpRequest* request, char* buf) {
	int count=0;
	
	char* requestLine = strtok(buf, "\r\n");
	logMessage(LOG_DEBUG, "Request", "Request line: '%s'\n", requestLine);
	int ret = parseRequestLine(request, requestLine);
	if(ret < 0) return ret;

	return 0;
}


void sendResponse(int clientFd, HttpResponse response) {
	// TODO: change to send an actual resposne

	char* fmt =
	"%s %d %s\r\n"
	"Server: Live\r\n"
	"Content-Type: text/html\r\n"
	"\r\n"
	"%s";

	dprintf(clientFd, fmt, response.httpVersion,
		response.code, getReasonPhrase(response.code), response.body
	);

	dprintf(1, fmt, response.httpVersion,
		response.code, getReasonPhrase(response.code), response.body
	);
	printf("\n");
}


char* getReasonPhrase(int responseCode) {
	switch(responseCode) {
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";
		case 103: return "Early Hints";
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi-Status (WebDAV)";
		case 208: return "Already Reported (WebDAV)";
		case 226: return "IM Used (HTTP Delta Encoding)";
		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 306: return "Unused";
		case 307: return "Temporary Redirect";
		case 308: return "Permanent Redirect";
		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Payload Too Large";
		case 414: return "URI Too Large";
		case 415: return "Unsupported Media Type";
		case 416: return "Range Not Satisfiable";
		case 417: return "Expectation Failed";
		case 418: return "I'm a teapot";
		case 421: return "Misdirected Request";
		case 422: return "Unprocessable Entity (WebDAV)";
		case 423: return "Locked (WebDAV)";
		case 424: return "Failed Dependency (WebDAV)";
		case 425: return "Too Early";
		case 426: return "Upgrade Required";
		case 428: return "Precondition Required";
		case 429: return "Too Many Requests";
		case 431: return "Request Header Fields Too Large";
		case 451: return "Unavailable for Legal Reasons";
		case 499: return "Client Closed Request";
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		case 507: return "Insufficient Storage (WebDAV)";
		case 508: return "Loop Detected (WebDAV)";
		case 510: return "Not Extended";
		case 511: return "Network Authentication Required";
		case 599: return "Network Connect Timeout Error";
		default: return "Unknown Status Code";
	}
}