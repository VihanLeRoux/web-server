#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <errno.h>

#include "http-handler.h"
#include "string-builder/header.h"
#include "hash-table/header.h"



void parse_method(String *l_method, Request *request) {
    char* str = read_string(l_method);

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

    // if (e_method == -1){
    //     fprintf(stderr, "No valid method");
    // }

    request->method = e_method;
    request->state = PATH;

    free(str);
}

void parse_path(String *l_path, Request *request){
    
    if (l_path->len > MAX_PATH_LENGTH){
        fprintf(stderr, "Request too long");
        return;
    }
    
    char* s_path = read_string(l_path);

    strcpy(request->path, s_path);
    request->state = VERSION;

    free(s_path);
}

void parse_version(String* l_version, Request *request){
    if (l_version->len > MAX_VERSION_LENGTH){
        fprintf(stderr, "Request too long");
        return;
    }

    char* s_version = read_string(l_version);

    strcpy(request->version, s_version);

    free(s_version);
}



void parse_message(String *message, Request *request){
    String *words = split_words(message);
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

    free_strings(words);
}

String* parse_headers_hash(String *line, Request *request){
    HashTable *table = create_hash_table(10);

    seed = 5;
    //SET_SEED(5);

    while (line != NULL && line->len > 0){
        String *split = split_first_char(line, ':');

        if (split == NULL) {
            char* err = read_string(line);
            fprintf(stderr, "Unable to split \"%s\"\n", err);
            free(err);

            return NULL;
        }

        char* key = read_string(split);
        char* value = split->next == NULL ? "NULL" : read_string(split->next);

        insert_hash_table(table, key, value);

        free(key);
        if (value != NULL) free(value);

        line = line->next;
    }

    request->header = table;

    if (line == NULL) return NULL;

    return line->next;
}

void parse_body(String *line, Request *request){
    unsigned long body_len = 0;
    errno = 0;

    Node *node = lookup(request->header, "Content-Length");
    if (node != NULL){
        char* endptr = NULL;
        body_len = strtoul(node->value, &endptr, 10);

        if (errno == ERANGE) {
            printf("Error: Value out of range (overflow)\n");
            return;
        } else if (endptr == node->value) {
            printf("Error: No valid conversion\n");
            return;
        } else if (*endptr != '\0') {
            printf("Warning: Partial conversion. Stopped at: %s\n", endptr);
            return;
        }
    } else {
        body_len = MAX_BODY_LENGTH;
    }



    request->body = malloc(body_len * sizeof(char));
    if (request->body == NULL) {
        printf("Error: Memory allocation failed\n");
        return;
    }

    size_t size = 0;

    // char* word = read_string(line);
    // printf("%s\n", word);
    // free(word);


    while (line != NULL){
        if (line->len <= 0) {
            line = line->next;
            continue;
        }

        if (line->len + size > body_len)
        {
            break;
        }


        memcpy(&request->body[size], line->p_line, line->len);
        size += line->len;

        line = line->next;

        if (line == NULL || line->len <= 0) continue;

        if (size + 2 >= body_len) break;

        memcpy(&request->body[size], "\r\n", 2);
        size += 2;
    }

    request->body[size] = '\0';
}


Request* parse_http_request(char* message, int len){
    String *line = split_lines(message, len);
    String *head = line->next;

    Request *request = calloc(1, sizeof(Request));

    parse_message(line, request);
    String *body = parse_headers_hash(head, request);

    parse_body(body, request);

    free_strings(line);


    return request;
}