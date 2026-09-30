#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>

#include "http-handler.h"

typedef struct Line {
    char* p_line;
    size_t len;
    struct Line *next;
} Line;

Line* get_lines(char* message, size_t len){

    Line *line = (Line *)calloc(1, sizeof(Line));
    Line *head = line;

    for(size_t i = 0; i < len; i++){
        if (message[i] == '\0') break;
        else if (message[i] == '\r') continue;
        else if (message[i] == '\n') {
            if (i != len - 2){
                Line *new_line = (Line *)calloc(1, sizeof(Line));

                line->next = new_line;
                line = new_line;
            }
        }
        else {
            if (line->len == 0){
                line->p_line = &message[i];
                line->len = 0;
                line->next = NULL;
            }

            line->len++;
        }
    }

    line->next = NULL;

    return head;
}

void free_lines(Line *line){
    while (line != NULL) {
        Line *next = line->next;
        free(line);
        line = next;
    }
}


char* read_line(Line *line){
    
    char* ret = malloc((line->len + 1) * sizeof(char));

    for(size_t i = 0; i < line->len; i++){
        ret[i] = line->p_line[i];
    }

    ret[line->len] = '\0';

    return ret;
}

Line* get_words(Line *line){
    Line *words = (Line *)calloc(1, sizeof(Line));
    Line *current = words;

    for (size_t i = 0; i < line->len; i++){
        char* letter = (line->p_line + (i * sizeof(char)));

        if (*letter == ' ') {
            Line *new_word = (Line *)calloc(1, sizeof(Line));
            current->next = new_word;
            current = new_word;
        } 
        else {
            if (current->p_line == NULL)
                current->p_line = letter;
            current->len++;
        }
    }
    
    return words;
}

Line* split_line(Line *line, char splitter){
    Line *split = (Line *)calloc(1, sizeof(Line));
    Line *current = split;

    int splat = 0;
    for(size_t i = 0; i < line->len; i++){
        char letter = line->p_line[i];
        if (letter == splitter && splat == 0){
            Line *new_word = (Line *)calloc(1, sizeof(Line));
            current->next = new_word;
            current = new_word;

            splat = 1;
        }
        else if (current->p_line == NULL){
            if (letter == ' ') continue;

            current->p_line = &(line->p_line[i]);
            current->len++;
        }
        else{
            current->len++;
        }
    }

    return split;
}


void parse_method(Line *l_method, Request *request) {
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

void parse_path(Line *l_path, Request *request){
    
    if (l_path->len > MAX_PATH_LENGTH){
        fprintf(stderr, "Request too long");
        return;
    }
    
    char* s_path = read_line(l_path);

    strcpy(request->path, s_path);
    request->state = VERSION;

    free(s_path);
}

void parse_version(Line* l_version, Request *request){
    if (l_version->len > MAX_VERSION_LENGTH){
        fprintf(stderr, "Request too long");
        return;
    }

    char* s_version = read_line(l_version);

    strcpy(request->version, s_version);

    free(s_version);
}



void parse_message(Line *message, Request *request){
    Line *words = get_words(message);
    Line *current = words;

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

void parse_headers(Line *line, Request *request){
    struct Header *header = calloc(1, sizeof(struct Header));
    struct Header *current = header;

    while (line != NULL && line->len > 0){
        Line *split = split_line(line, ':');

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
    Line *line = get_lines(message, len);

    Request *request = calloc(1, sizeof(Request));

    parse_message(line, request);
    parse_headers(line->next, request);

    free_lines(line);
}