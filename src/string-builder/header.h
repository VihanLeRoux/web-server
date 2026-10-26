#ifndef STRING_BUILDER_HEADER_H
#define STRING_BUILDER_HEADER_H

#include <stdio.h>


typedef struct String {
    char* p_line;
    size_t len;
    struct String *next;
} String;

String* split_lines(char*, size_t);
String* split_words(String*);
String* split_first_char(String*, char);
char* read_string(String*);

void free_strings(String*);


#endif