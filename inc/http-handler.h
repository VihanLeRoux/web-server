#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#define MAX_PATH_LENGTH 2048

#define MAX_VERSION_LENGTH 512

#define MAX_HEADER_KEY_LENGTH 256
#define MAX_HEADER_VALUE_LENGTH 512

#define MAX_BODY_LENGTH 4086

#define METHOD_COUNT 4

#include "../src/hash-table/header.h"

enum Method {
    GET = 0,
    POST,
    PUT,
    DELETE
};

enum Code {
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    TOO_LONG_REQUEST = 414,
    INT_SERVER_ERROR = 500
};

enum ParseState {
    METHOD,
    PATH,
    VERSION,
};

typedef struct {
    enum ParseState state;
    enum Method method;
    char path[MAX_PATH_LENGTH];
    char version[MAX_VERSION_LENGTH];
    char* body;
    HashTable *header;
} Request;

typedef struct {
    float version;
    enum Code code;
    char* message;
    char* body;
    HashTable *header;
} Response;

Request* parse_http_request(char*, int);
void close_request(Request*);

#endif