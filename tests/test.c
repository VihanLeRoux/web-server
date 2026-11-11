#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "http-handler.h"
#include "../src/hash-table/header.h"


void test_parse_http_request(){
    printf("Testing HTTP request parsing...\n");

    char* message = "POST /api/users HTTP/1.1\r\nHost: api.example.com\r\nUser-Agent: Mozilla/5.0\r\nAccept: application/json\r\nAccept-Language: en-US\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\nAuthorization: Bearer your-token-here\r\nX-Request-ID: 1a2b3cde-4fgh-5ijk-6lmn-7opqr8s9t0uv\r\nContent-Type: application/json\r\nContent-Length: 25\r\n\r\n{\"name\":\"John\",\"age\":30}\r\n";

    int len = strlen(message);

    Request* request = parse_http_request(message, len);

    //Message
    assert(request->method == POST);
    assert(strcmp(request->path, "/api/users") == 0);
    assert(strcmp(request->version, "HTTP/1.1") == 0);


    //Headers
    HashTable *table = request->header;

    Node* node = lookup(table, "Host");
    assert(strcmp(node->value, "api.example.com") == 0);

    node = lookup(table, "Content-Type");
    assert(strcmp(node->value, "application/json") == 0);

    node = lookup(table, "Content-Length");
    assert(strcmp(node->value, "25") == 0);

    node = lookup(table, "Accept");
    assert(strcmp(node->value, "application/json") == 0);

    node = lookup(table, "Accept-Language");
    assert(strcmp(node->value, "en-US") == 0);

    node = lookup(table, "Accept-Encoding");
    assert(strcmp(node->value, "gzip, deflate") == 0);

    node = lookup(table, "Connection");
    assert(strcmp(node->value, "keep-alive") == 0);

    node = lookup(table, "User-Agent");
    assert(strcmp(node->value, "Mozilla/5.0") == 0);

    node = lookup(table, "Authorization");
    assert(strcmp(node->value, "Bearer your-token-here") == 0);

    node = lookup(table, "X-Request-ID");
    assert(strcmp(node->value, "1a2b3cde-4fgh-5ijk-6lmn-7opqr8s9t0uv") == 0);

    
    //Body
    assert(strcmp(request->body, "{\"name\":\"John\",\"age\":30}") == 0);


    //Success
    printf("✓ All assertions passed\n");
}

int main(void){
    test_parse_http_request();

    return 0;
}