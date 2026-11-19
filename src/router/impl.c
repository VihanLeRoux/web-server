#include <stdlib.h>


#include "../string-builder/header.h"
#include "header.h"


static Route *root;

String *split_path(char *path) {
    size_t len = strlen(path);
    size_t start = 0;
    String *ret = NULL;
    String *tail = NULL;

    for (size_t i = 0; i <= len; i++) {
        if (i < len && path[i] != '/') {
            continue;
        }

        /* Segment runs from start to i; skip empty segments. */
        if (i > start) {
            String *node = malloc(sizeof *node);
            if (node == NULL) {
                /* Free any nodes already allocated before returning. */
                while (ret != NULL) {
                    String *next = ret->next;
                    free(ret);
                    ret = next;
                }
                return NULL;
            }

            node->p_line = &path[start];
            node->len = i - start;
            node->next = NULL;

            if (tail != NULL) {
                tail->next = node;
            } else {
                ret = node;
            }
            tail = node;
        }

        start = i + 1;
    }

    return ret;
}


Route* find_route_part(char* part, Route *current){
    if (current == NULL)
        return NULL;

    Route *route = NULL;

    for(size_t i = 0; i < current->size; i++){

        if (strcmp(current->children[i]->part, part)) continue;

        route = current->children[i];

        break;
    }

    return route;
}

Route* add_child(char* part, Route* current){


    Route *child = calloc(1, sizeof(Route));

    size_t part_len = strlen(part);


    child->part = malloc((part_len + 1) * sizeof(char));
    memcpy(child->part, part, part_len);
    child->part[part_len] = '\0';

    
    if (current->size <= 0){
        current->children = calloc(current->size + 1, sizeof *current->children);
    } else{
        Route **new_children = realloc(current->children, (current->size + 1) * sizeof(Route**));
        current->children = new_children;
    }
    
    current->children[current->size++] = child;

    return child;
}

void list_routes(const Route *current, char *path, size_t path_len, size_t capacity) {
    if (current == NULL) return;

    size_t old_len = path_len;
    size_t part_len = strlen(current->part);

    if (part_len > 0) {
        int written = snprintf(path + path_len, capacity - path_len,
                               "%s%.*s",
                               path_len > 0 ? "/" : "",
                               (int)part_len, current->part);
        if (written < 0 || (size_t)written >= capacity - path_len) {
            path[old_len] = '\0';
            return;
        }
        path_len += (size_t)written;
    }

    if (current->size == 0) {
        puts(path);
    } else {
        for (size_t i = 0; i < current->size; i++) {
            list_routes(current->children[i], path, path_len, capacity);
        }
    }

    path[old_len] = '\0';
}

Route* find_route(char* path){
    String *split = split_path(path);
    Route* current = root;
    
    while (split != NULL){
        char *part = read_string(split);

        Route *next = find_route_part(part, current);

        //printf("%p (%s)\n", next, part);

        if (next == NULL) return NULL;

        current = next;
        split = split->next;

        free(part);
    }

    free_strings(split);

    return current;
}


void add_route(char* path, enum Method method, handler_function function){
    String *split = split_path(path);

    if (root == NULL) {
        root = calloc(1, sizeof(Route));
        root->part = malloc(sizeof(char));
        root->part = "\0";
    }

    Route *current = root;

    while (split != NULL){
        char *part = read_string(split);


        Route *next = find_route_part(part, current);

        if (next == NULL) {
            next = add_child(part, current);
        }

        current = next;
        split = split->next;

        free(part);
    }

    current->function[method] = function;

    free_strings(split);
}

