#include "header.h"

String* split_lines(char* message, size_t len){

    String *string = (String *)calloc(1, sizeof(String));
    String *head = string;

    for(size_t i = 0; i < len; i++){
        if (message[i] == '\0') break;
        else if (message[i] == '\r') continue;
        else if (message[i] == '\n') {
            if (i != len - 2){
                String *new_line = (String *)calloc(1, sizeof(String));

                string->next = new_line;
                string = new_line;
            }
        }
        else {
            if (string->len == 0){
                string->p_line = &message[i];
                string->len = 0;
                string->next = NULL;
            }

            string->len++;
        }
    }

    string->next = NULL;

    return head;
}


String* split_words(String *string){
    String *words = (String *)calloc(1, sizeof(String));
    String *current = words;

    for (size_t i = 0; i < string->len; i++){
        char* letter = (string->p_line + (i * sizeof(char)));

        if (*letter == ' ') {
            String *new_word = (String *)calloc(1, sizeof(String));
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

String* split_first_char(String *string, char splitter){
    String *split = (String *)calloc(1, sizeof(String));
    String *current = split;

    int splat = 0;
    for(size_t i = 0; i < string->len; i++){
        char letter = string->p_line[i];
        if (letter == splitter && splat == 0){
            String *new_word = (String *)calloc(1, sizeof(String));
            current->next = new_word;
            current = new_word;

            splat = 1;
        }
        else if (current->p_line == NULL){
            if (letter == ' ') continue;

            current->p_line = &(string->p_line[i]);
            current->len++;
        }
        else{
            current->len++;
        }
    }

    return split;
}

void free_strings(String *string){
    while (string != NULL) {
        String *next = string->next;
        free(string);
        string = next;
    }
}