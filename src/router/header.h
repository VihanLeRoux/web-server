#ifndef ROUTER_HEADER_H
#define ROUTER_HEADER_H

#include <string.h>

#include "http-handler.h"


typedef void (*handler_function) (Request *request, Response *response);

typedef struct Route {
    handler_function function[METHOD_COUNT];
    char* part;
    struct Route **children;
    size_t size;
} Route;

Route* find_route(char* path);
void add_route(char* path, enum Method method, handler_function function);


#endif