#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#define MAX_PATH_LENGTH 2048
#define MAX_VERSION_LENGTH 512

enum Method {
    GET = 1,
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

struct Header {
    char* name;
    char* value;
    struct Header *next;
};

typedef struct {
    enum ParseState state;
    enum Method method;
    char path[MAX_PATH_LENGTH];
    char version[MAX_VERSION_LENGTH];
    struct Header header;
} Request;

typedef struct {
    float version;
    enum Code code;
    char* message;
    char* body;
    struct Header header;
} Response;

void parse_http_request(char*, int, Request*);

#endif