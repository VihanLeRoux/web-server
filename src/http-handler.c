#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>

#include "http-handler.h"
#include "string-builder/header.h"



void parse_method(String *l_method, Request *request) {
    char* str = read_line(l_method);

    enum Method e_method = -1;

    if (strcmp(str, "GET") == 0) {
        e_method = GET;
    } else if (strcmp(str, "POST") == 0) {
        e_method = POST;
    } else if (strcmp(str, "PUT") == 0) {
        e_method = PUT;
    } else if (strcmp(str, "DELETE") == 0) {
        e_method = DELETE;
    }

    if (e_method == -1){
        fprintf(stderr, "No valid method");
    }

    request->method = e_method;
    request->state = PATH;

    free(str);
}

void parse_path(String *l_path, Request *request){
    
    if (l_path->len > MAX_PATH_LENGTH){
        fprintf(stderr, "Request too long");
        return;
    }
    
    char* s_path = read_line(l_path);

    strcpy(request->path, s_path);
    request->state = VERSION;

    free(s_path);
}

void parse_version(String* l_version, Request *request){
    if (l_version->len > MAX_VERSION_LENGTH){
        fprintf(stderr, "Request too long");
        return;
    }

    char* s_version = read_line(l_version);

    strcpy(request->version, s_version);

    free(s_version);
}



void parse_message(String *message, Request *request){
    String *words = get_words(message);
    String *current = words;

    request->state = METHOD;

    while (current != NULL){

        switch(request->state){
            case METHOD:
                parse_method(current, request);
                break;
            case PATH:
                parse_path(current, request);
                break;
            case VERSION:
                parse_version(current, request);
                break;
            default:
                break;
        }
        
        current = current->next;
    }

    free_lines(words);
}

void parse_headers(String *line, Request *request){
    struct Header *header = calloc(1, sizeof(struct Header));
    struct Header *current = header;

    while (line != NULL && line->len > 0){
        String *split = split_line(line, ':');

        if (split == NULL) {
            char* err = read_line(line);
            fprintf(stderr, "Unable to split \"%s\"\n", err);
            free(err);

            return;
        }

        char* key = read_line(split);
        char* value = split->next == NULL ? NULL : read_line(split->next);

        strcpy(current->key, key);
        strcpy(current->value, value);


        free(key);
        if (value != NULL) free(value);

        line = line->next;

        if (line->len > 0){
            struct Header *new_header = calloc(1, sizeof(struct Header));
            current->next = new_header;
            current = new_header;
        }
    }

    request->header = header;
}

void parse_body(){

}


void parse_http_request(char* message, int len){
    String *line = get_lines(message, len);

    Request *request = calloc(1, sizeof(Request));

    parse_message(line, request);
    parse_headers(line->next, request);

    free_lines(line);
}