//
// Created by root on 18-11-13.
//

#ifndef SFF_LOAD_FILE_H
#define SFF_LOAD_FILE_H

#endif //SFF_LOAD_FILE_H



void include_file(const char* filename,zval *return_value_ptr);

void get_file_execute_dir(char *buf);

void get_file_data_to_hash_table(HashTable *ht, char *file_path);

//获取文件名字
const char* get_file_name(char* file_name);

SFF_BOOL is_dir(char *path);
SFF_BOOL is_file(char *path);

