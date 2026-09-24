#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

enum Request_Method {
    GET,
    POST,
    PUT,
    DELETE
};

enum Response_Code {
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    INT_SERVER_ERROR = 500
};

typedef struct {
    enum Request_Method method;
    char* path;
    float version;
} Request_Header;



typedef struct {
    float version;
    enum Response_Code code;
    char* message;
} Response_Header;

#endif