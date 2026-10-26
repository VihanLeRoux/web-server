#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "http-handler.h"
#include "../src/hash-table/header.h"


void test_parse_http_request(){
    printf("Testing HTTP request parsing...\n");

    char* message = "POST /api/users HTTP/1.1\r\nHost: api.example.com\r\nContent-Type: application/json\r\nContent-Length: 25\r\n\r\n{\"name\":\"John\",\"age\":30}\r\n";

    int len = strlen(message);

    Request* request = parse_http_request(message, len);

    assert(request->method == POST);
    assert(strcmp(request->path, "/api/users") == 0);
    assert(strcmp(request->version, "HTTP/1.1") == 0);

    HashTable *table = request->header;

    Node* node = lookup(table, "Host");
    assert(strcmp(node->value, "api.example.com") == 0);

    node = lookup(table, "Content-Type");
    assert(strcmp(node->value, "application/json") == 0);

    node = lookup(table, "Content-Length");
    assert(strcmp(node->value, "25") == 0);

    printf("✓ All assertions passed\n");
}

int main(void){
    test_parse_http_request();

    return 0;
}