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

    Line *line = (Line *)malloc(sizeof(Line));
    Line *head = line;

    for(size_t i = 0; i < len; i++){
        if (message[i] == '\0') break;
        else if (message[i] == '\r') continue;
        else if (message[i] == '\n') {
            if (i != len - 2){
                Line *new_line = (Line *)malloc(sizeof(Line));

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
    for(Line *p = line; p != NULL; p = p->next){
        Line *temp = p;
        free(temp);
    }
}


char* read_line(Line *line){
    
    char* ret = malloc((line->len + 1) * sizeof(char));

    for(size_t i = 0; i < line->len; i++){
        char next = *(line->p_line + (i * sizeof(char)));

        ret[i] = next;
    }

    ret[line->len] = '\0';

    return ret;
}

Line* get_words(Line *line){
    Line *words = (Line *)malloc(sizeof(Line));
    Line *current = words;

    for (size_t i = 0; i < line->len; i++){
        char* letter = (line->p_line + (i * sizeof(char)));

        if (*letter == ' ') {
            Line *new_word = (Line *)malloc(sizeof(Line));
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


#include <string.h>


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

    //free(str);
}

void parse_path(Line *l_path, Request *request){
    
    if (l_path->len > MAX_PATH_LENGTH){
        fprintf(stderr, "Request too long");
        return;
    }
    
    char* s_path = read_line(l_path);

    strcpy(request->path, s_path);
    request->state = VERSION;

    //free(s_path);
}

void parse_version(Line* l_version, Request *request){
    if (l_version->len > MAX_VERSION_LENGTH){
        fprintf(stderr, "Request too long");
        return;
    }

    char* s_version = read_line(l_version);

    strcpy(request->version, s_version);

    //free(s_version);
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
    
    printf("\n%d %s %s\n", request->method, request->path, request->version);

    free_lines(words);
}

void parse_headers(){

}

void parse_body(){

}


void parse_http_request(char* message, int len, Request* request){
    Line *line = get_lines(message, len);

    parse_message(line, request);

    // for(Line *p = line; p != NULL; p = p->next){
    //     char* l = read_line(p);

    //     printf("(%ld) %s\n", p->len, l);

    //     free(l);
    // }

    free_lines(line);
}