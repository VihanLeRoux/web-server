#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "http-handler.h"
#include "../src/hash-table/header.h"
#include "../src/router/header.h"

void functionse(Request* request, Response *response) {

    printf("%s", request->body);
}

void test_add_route(){
    printf("Testing HTTP route add...\n");

    add_route("/api/users", POST, functionse);
}


Request* test_parse_http_request(){
    printf("Testing HTTP request parsing...\t\t");

    char* message = "POST /api/users HTTP/1.1\r\nHost: api.example.com\r\nUser-Agent: Mozilla/5.0\r\nAccept: application/json\r\nAccept: text\r\nAccept-Language: en-US\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\nAuthorization: Bearer your-token-here\r\nX-Request-ID: 1a2b3cde-4fgh-5ijk-6lmn-7opqr8s9t0uv\r\nContent-Type: application/json\r\nContent-Length: 35\r\n\r\n{\r\n\t\"name\":\"John\",\r\n\t\"age\":30\r\n}\r\n";

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
    assert(strcmp(node->value, "35") == 0);

    node = lookup(table, "Accept");
    assert(strcmp(node->value, "application/json") == 0);
    assert(strcmp(node->next->value, "text") == 0);

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
    assert(strcmp(request->body, "{\r\n\t\"name\":\"John\",\r\n\t\"age\":30\r\n}\r\n") == 0);

    //Success
    printf("✓ All assertions passed\n");

    return request;
}



int main(void){
    test_add_route();
    
    
    Request *request = test_parse_http_request();

    Route *route = find_route(request->path);

    route->function[request->method](request, NULL);
    
    close_request(request);

    return 0;
}