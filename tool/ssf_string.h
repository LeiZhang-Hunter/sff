//
// Created by root on 18-11-18.
//

#ifndef SFF_SFF_STRING_H
#define SFF_SFF_STRING_H

#endif //SFF_SFF_STRING_H

typedef struct _sff_string {
    char *content;
    int length;
} sff_string;

//malloc
sff_string *init_sff_string(char *content);

//stack
void init_sff_string_by_stack(sff_string *ptr, char *content);

void debug_sff_string(sff_string *string_ptr);

void sub_str_string(sff_string *dst_string, sff_string *string_ptr, int begin_postion, int end_position);

void sff_string_free(sff_string *string_ptr);

void replace_string(char *string,char search, char purpose);