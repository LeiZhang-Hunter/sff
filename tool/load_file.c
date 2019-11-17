//
// Created by root on 18-11-13.
//
#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"

#endif

void include_file(const char *filename, zval *return_value_ptr) {
    zend_file_handle include_file_handle;
    zend_op_array *op_array;
    zval dummy;
    bzero(return_value_ptr,sizeof(zval));
    zval result;
    ZVAL_UNDEF(&result);
    //init file handel
    zend_bool open_result = php_stream_open_for_zend_ex(filename, &include_file_handle,USE_PATH | STREAM_OPEN_FOR_INCLUDE);

    if (open_result == SUCCESS) {
        zend_try{
            op_array = zend_compile_file(&include_file_handle, ZEND_INCLUDE);
            //destroy file handel
            zend_destroy_file_handle(&include_file_handle);
            zend_string * opened_path = zend_string_init(filename, strlen(filename), 0);


            if (op_array) {
                ZVAL_NULL(&dummy);

                zend_hash_add(&EG(included_files), opened_path, &dummy);
                zend_string_release(opened_path);
            }
        }zend_end_try();

        if (EG(exception)) {
            zend_exception_error(EG(exception), E_ERROR);
        }



        if (op_array) {
            zend_try
            {
                zend_execute(op_array, &result);
                if (return_value_ptr) {
                    ZVAL_DUP(return_value_ptr, &result);
                }
                zval_ptr_dtor(&result);
            }
            zend_catch
            {
                php_error_docref(NULL, E_ERROR, "load file error");
//                        return_ret = 0;
            }
            zend_end_try();

            destroy_op_array(op_array);
            efree(op_array);
        }else{
            php_error_docref(NULL, E_ERROR, "load file error");
        }
    }else{
        php_error_docref(NULL, E_ERROR, "load file error");
    }
}

void get_file_execute_dir(char *buf) {
    char path[MAXPATHLEN];

#if HAVE_GETCWD
    VCWD_GETCWD(path, MAXPATHLEN);
#elif HAVE_GETWD
    VCWD_GETWD(path);
#endif
    strcpy(buf, path);
}

/**
 *
 * @param ht    //要存储的hashtable
 * @param file_path //文件目录
 * @param folder_level 目录等级
 * @param level 地柜到几个级别
 */
//!string is illeagle, i need change his by sff string
//遍历指定目录的目录树，然后编译php文件存放到hashtable当中
void get_file_data_to_hash_table(HashTable *ht, char *file_path) {

    DIR *config_handle;//目录文件句柄
    char file_path_cpy[MAXPATHLEN];//路径文件的复制
    char recursion_dir[MAXPATHLEN];
    struct dirent *unit_file_stream_struct;//存放文件信息的结构体
    HashTable *base_hash;
    zend_string* dir_key;
    zend_string* file_key;
    char compile_file_name[MAXPATHLEN];
    zval include_file_result;//编译php文件的返回值
    //申请一个空的数组
    zval dir_array;//目录用的空数组

    if(!file_path)
    {
        return;
    }


    //赋值文件路径
    strcpy(file_path_cpy,file_path);

    //判断路径是否是一个目录
    if(is_dir(file_path_cpy) == SFF_FALSE)
    {
        return;
    }

    config_handle = opendir(file_path);


    /**
     * 由于readdir在不通操作系统上有兼容行问题当出现兼容性问题我们使用is_dir做判断，不是文件夹就是文件
     */

    //如果说可以打开
    while((unit_file_stream_struct = readdir(config_handle)))
    {

        bzero(&recursion_dir,sizeof(recursion_dir));

        //再次进行拼接路径
        sprintf(recursion_dir,"%s/%s",file_path_cpy,unit_file_stream_struct->d_name);

        //如果是一个目录
            if(unit_file_stream_struct->d_type == DT_DIR || is_dir(recursion_dir) == SFF_TRUE)
        {

            //不能是 . 或者是 ../
            if (strcmp(unit_file_stream_struct->d_name, ".") == 0 || strcmp(unit_file_stream_struct->d_name, "..") == 0) {
                continue;
            }

            bzero(&dir_array, sizeof(dir_array));

            //初始化一个空的数组
            array_init(&dir_array);

            dir_key = zend_string_init(unit_file_stream_struct->d_name, strlen(unit_file_stream_struct->d_name), 0);

            zend_hash_add(ht,dir_key,&dir_array);

            zend_string_release(dir_key);



            //再次递归

            get_file_data_to_hash_table(Z_ARRVAL_P(&dir_array),recursion_dir);

        //如果是一个文件
        }else if(unit_file_stream_struct->d_type == DT_REG || is_file(recursion_dir) == SFF_TRUE)
        {
            bzero(&compile_file_name,sizeof(compile_file_name));
            sprintf(compile_file_name,"%s/%s",file_path,unit_file_stream_struct->d_name);

            //初始化
            bzero(&include_file_result, sizeof(include_file_result));


            //如果说存在文件
            if(access(compile_file_name,F_OK) == 0)
            {
                include_file(compile_file_name,&include_file_result);

                //如果说是一个数组
                if(Z_TYPE(include_file_result) == IS_ARRAY)
                {
                    const char* stream_file_name = get_file_name(unit_file_stream_struct->d_name);
                    file_key = zend_string_init(stream_file_name,strlen(stream_file_name),0);
                    free((char*)stream_file_name);
                    zend_hash_add(ht,file_key,&include_file_result);
                    zend_string_release(file_key);
                }
            }
        }
    }

    closedir(config_handle);
}

//获取文件名字
const char* get_file_name(char* file_name)
{
    //要返回的文件名字
    static char* return_file_name;
    char file_name_cpy[MAXPATHLEN];
    unsigned char tmp;

    //初始化这一个数组
    bzero(&file_name_cpy,sizeof(file_name_cpy));

    strcpy(file_name_cpy,file_name);

    //赋值出一块地址
    tmp = (unsigned char)(strchr(file_name_cpy,'.')-file_name_cpy);
    return_file_name = tmp > 0 ? strndup(file_name,tmp):strdup(file_name_cpy);
    return return_file_name;
}

//检查文件是否是目录
SFF_BOOL is_dir(char *path)
{
    DIR *result = opendir(path);
    if(result == NULL)
    {
        return SFF_FALSE;
    }else {
        closedir(result);
        return SFF_TRUE;
    }
}

//检查文件是否存在
SFF_BOOL is_file(char *path)
{
    int result = access(path,F_OK);
    if(result != 0)
    {
        return SFF_FALSE;
    }else {
        return SFF_TRUE;
    }
}

