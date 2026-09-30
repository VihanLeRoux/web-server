#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "http-handler.h"


void test_parse_http_request(){
    printf("Testing HTTP request parsing...\n");

    char* message = "POST /api/users HTTP/1.1\r\nHost: api.example.com\r\nContent-Type: application/json\r\nContent-Length: 25\r\n\r\n{\"name\":\"John\",\"age\":30}\r\n";

    int len = strlen(message);

    Request request;

    parse_http_request(message, len, &request);

    assert(request.method == POST);
    assert(strcmp(request.path, "/api/users") == 0);
    assert(strcmp(request.version, "HTTP/1.1") == 0);

    printf("✓ All assertions passed\n");
}

int main(void){
    test_parse_http_request();

    return 0;
}