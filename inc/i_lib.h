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

#define MSG_LEN		4096
#define CFG_LEN		1024

void check_run(char* lockFile);

void write_log(char *file, int line, const char *msg, ...);

char *get_cfg(const char *filename, char *section, char *item);

#endif
