#ifndef HELPER_H
#define HELPER_H

void clear_input_buffer();

void get_string(char *&buffer);

unsigned int my_len(const char *str);

void print_whitespace(const int num);

const char* print_utf8_substr(const char* str, int max_chars);


#endif