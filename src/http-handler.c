#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../inc/http-handler.h"

Line* get_lines(char* message, size_t len){

    Line *line = (Line *)malloc(sizeof(Line));
    Line *head = line;

    for(size_t i = 0; i < len; i++){
        if (message[i] == '\0') break;
        else if (message[i] == '\r') continue;
        else if (message[i] == '\n') {
            if (i != len - 1){
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



void parse_http_request(char* message, int len, Request* request){

    printf("Parsing...\n");

    Line *line = get_lines(message, len);

    for(Line *p = line; p != NULL; p = p->next){
        char* l = read_line(p);

        printf("(%ld) %s\n", p->len, l);

        free(l);
    }

    free_lines(line);
}