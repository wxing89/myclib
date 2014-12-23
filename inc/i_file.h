/*******************************************************
 * File: 
 *      i_file.h
 * Description:
 *      file methods
 * Author: 
 *      w.x
 * History:
 *      2013.08.01 file created
 * 
 *
*******************************************************/


#ifndef _I_FILE_H
#define _I_FILE_H

#include <stdio.h>

#define BUFFER_LEN		4096
#define FILE_NAME_LEN	256 

int get_line(char *line, FILE *fs);

int get_file(char file_arr[][FILE_NAME_LEN], const char *file_dir, const char *pattern);

int cp_file(const char *file_src, const char *file_dst);
int cp_file2(const char *file_src, const char *file_dst);

int mv_file(const char *file_src, const char *file_dst);

int app_file(const char *file_src, const char *file_dst);


#endif
