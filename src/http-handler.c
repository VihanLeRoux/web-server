#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../inc/http-handler.h"



size_t get_line_count(char* message, int* count){
    size_t len = 0;
    *count = 0;

    char current = message[len];

    while (current != '\0') {
        
        if (current == '\n'){
            (*count)++;
        }

        len++;
        current = message[len];
    }

    if (message[len-1] == '\n') (*count)--;

    return len;
}


void get_lines(char* message, Line *lines[], size_t len){
    size_t j = 0;
    char* current;

    Line *line = (Line *)malloc(sizeof(Line));

    for(size_t i = 0; i < len; i++){
        current = message[i];

        if (current == '\0') break;
        else if (current == '\r') continue;
        else if (current == '\n') {
            lines[j] = line;
            line = (Line *)malloc(sizeof(Line));
            j++;
        }
        else {
            if (line->len == 0){
                line->p_line = &message[i];
                line->len = 0;
            }

            line->len++;
        }
    }

    //free(line);
}

void parse_http_request(char* req, Request* request){

    printf("Parsing...\n");

    int count = 0;
    
    size_t len = get_line_count(req, &count);

    Line *lines[count];

    get_lines(req, lines, len);

    for (int i = 0; i < count; i++){
        Line *line = lines[i];

        for(int j = 0; j < line->len; j++){
            char next = *(line->p_line + (j * sizeof(char)));

            printf("%c", next);
        }

        printf("\n");
    }
}