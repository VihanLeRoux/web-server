#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

enum Method {
    GET,
    POST,
    PUT,
    DELETE
};

enum Code {
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    INT_SERVER_ERROR = 500
};

struct Header {
    char* name;
    char* value;
    struct Header *next;
};

typedef struct {
    enum Method method;
    char* path;
    float version;
    struct Header header;
} Request;

typedef struct {
    float version;
    enum Code code;
    char* message;
    char* body;
    struct Header header;
} Response;

typedef struct Line {
    char* p_line;
    size_t len;
    struct Line *next;
} Line;

void parse_http_request(char*, int, Request*);

#endif