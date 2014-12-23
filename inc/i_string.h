/*******************************************************
 * File: 
 *      i_string.h
 * Description:
 *      string methods
 * Author: 
 *      w.x
 * History:
 *      2013.08.01 file created
 * 
 *
*******************************************************/


#ifndef _I_STRING_H
#define _I_STRING_H

#include <stdlib.h>

char *trim(char *str);
char *trim_left(char *str);
char *trim_right(char *str);

char *str_tolower(char *str);
char *str_toupper(char *str);

char *substr(char *sub, const char *str, size_t index, size_t len);

size_t str_split(char **arr, const char *str, char delim);
size_t str_split2(char **arr, const char *str, char *delim);

char *str_field(char *field, const char *str, char delim, size_t index);
char *str_field2(char *field, const char *str, char *delim, size_t index);

void str_swap(char *str1, char *str2);

char *str_reverse(char *str);

char *str_replace(char *str2, const char *str1, const char *src, const char *dst);
char *str_replaceall(char *str2, const char *str1, const char *src, const char *dst);

void str_sort(char **strarr, size_t nmemb);
void str_qsort(char **strarr, size_t nmemb);

int reg_match(const char *str, const char *pattern);


#endif
