/*******************************************************
 * File: 
 *      i_sys.h
 * Description:
 *      some methods
 * Author: 
 *      w.x
 * History:
 *      2013.08.01 file created
 * 
 *
*******************************************************/


#ifndef _I_LIB_H
#define _I_LIB_H

#include <stdio.h>

#define MSG_LEN		4096
#define CFG_LEN		1024

void check_run(char* lockFile);


void *allocArr(char **arr, size_t size, size_t nmemb);
void freeArr(char **arr, size_t nmemb);

void write_log(char *file, int line, const char *msg, ...);

char *get_cfg(const char *filename, char *section, char *item);

#endif
