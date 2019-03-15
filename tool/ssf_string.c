//
// Created by root on 18-11-18.
//
#include "../sff_common.h"
//#include "sff_string.h"
//init sff string
//#include <stdio.h>
//#include <stdlib.h>
//#include <strings.h>
//#include <string.h>




//int main()
//{
//    return 0;
//}
sff_string *init_sff_string(char *content)
{
    sff_string* sff_string_struct = malloc(sizeof(sff_string));
    bzero(sff_string_struct,sizeof(sff_string_struct));
    sff_string_struct->content = content;
    sff_string_struct->length = strlen(content);
    return sff_string_struct;
}

void init_sff_string_by_stack(sff_string* ptr,char* content)
{
    ptr->content = content;
    ptr->length = strlen(content);
}

//substr sff string
void sub_str_string(sff_string *dst_string,sff_string *string_ptr,int begin_postion,int end_position)
{
    int num;
    num = begin_postion;
    int arg = end_position-begin_postion;
    char buf[arg];
    int count = 0;
    dst_string->content = calloc(sizeof(dst_string->content), sizeof(char));

    for(num;num<end_position;num++)
    {
        buf[count] = string_ptr->content[num];
        count++;
    }
    buf[count] = '\0';

    dst_string->length = strlen(buf);

    strcpy(dst_string->content,buf);
}

//free
void sff_string_free(sff_string *string_ptr)
{
    free(string_ptr);
}

//print
void debug_sff_string(sff_string* string_ptr)
{
    php_printf("ssf_string content:%s;ssf_string length:%d\n",string_ptr->content,string_ptr->length);
}

