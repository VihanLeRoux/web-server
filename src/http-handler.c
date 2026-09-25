#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../inc/http-handler.h"


size_t get_line_count(char* message, size_t len){
    size_t count = 0;

    for (size_t i = 0; i < len; i++){
        if (message[i] == '\n'){
            count++;
        }
    }

    if (message[len-1] == '\n') count--;

    return count;
}


void get_lines(char* message, Line *lines[], size_t len){
    size_t j = 0;

    Line *line = (Line *)malloc(sizeof(Line));

    for(size_t i = 0; i < len; i++){
        if (message[i] == '\0') break;
        else if (message[i] == '\r') continue;
        else if (message[i] == '\n') {
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

void free_lines(Line *line){

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



void parse_http_request(char* req, Request* request){

    printf("Parsing...\n");

    size_t len = strlen(req);
    size_t count = get_line_count(req, len);

    if (count <= 0) return;

    Line *lines[count];

    get_lines(req, lines, len);

    // for(Line *p = lines[0]; p != NULL; p = p->next){
    //     char* l = read_line(p);

    //     printf("%s\n", l);

    //     free(l);
    // }
    
    for (size_t i = 0; i < count; i++){
        Line *line = lines[i];

        char* l = read_line(line);

        printf("%s\n", l);

        free(l);
    }

    //free_lines(lines);
}