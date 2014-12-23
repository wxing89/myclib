/*******************************************************
 * File: 
 *      i_sys.c
 * Description:
 *      some methods
 * Author: 
 *      w.x
 * History:
 *      2013.08.01 file created
 * 
 *
*******************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "i_lib.h"
#include "i_file.h"
#include "i_time.h"
#include "i_string.h"

#define write_lock(fd, offset, whence, len) lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)

static int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    return (fcntl(fd,cmd, &lock));
}

void check_run(char* lockFile)
{
    int fd, val;
    char buf[10];
    strcat(lockFile,".lock");

    if((fd = open(lockFile, O_WRONLY | O_CREAT, 0644)) < 0 ) {
        exit(EXIT_FAILURE);
    }

    /* try and set a write lock on the entire file */
    if(write_lock(fd, 0, SEEK_SET, 0) <0 ) {
        puts(strcat(lockFile, " is already running."));
        if(errno == EACCES || errno == EAGAIN) {
            exit(EXIT_SUCCESS);            /* gracefully exit, daemon is already running */
        }else {
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    /* truncate to zero length, now that we have the lock */
    if(ftruncate(fd, 0) < 0) {
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* write our process id */
    sprintf(buf, "%d\\n", getpid());
    if(write(fd, buf, strlen(buf)) != strlen(buf)) {
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* set close-on-exec flag for descriptor */
    if((val = fcntl(fd, F_GETFD, 0) < 0 )) {
        close(fd);
        exit(1);
    }
    val |= FD_CLOEXEC;
    if(fcntl(fd, F_SETFD, val) < 0 ) {
        close(fd);
        exit(EXIT_FAILURE);
    }
    /* leave file open until we terminate: lock will be held */
}

void write_log(char *file, int line, const char *msg, ...) {
    FILE *fs;
    char writemsg[MSG_LEN];
    char filename[FILE_NAME_LEN];
    char datemsg[TIME_LEN];
    char timemsg[TIME_LEN];

    va_list ap;
    char tmpmsg[MSG_LEN];

    memset(datemsg, 0, sizeof(datemsg));
    memset(timemsg, 0, sizeof(timemsg));

    get_date(datemsg);
    get_datetime(timemsg);

    sprintf(filename, "./log/%s.log", datemsg);

    va_start(ap, msg);
    memset(tmpmsg, 0, MSG_LEN);
    vsprintf(tmpmsg, msg, ap);

    size_t i;
    sprintf(writemsg, "[%s][FILE=%s, LINE=%d]%s", timemsg, file, line, tmpmsg);
    for(i = strlen(writemsg) - 1; i > 0 && !isprint(writemsg[i]); --i)
        ;
    writemsg[i + 1] = '\0';
    strcat(writemsg, "\n");

    fs = fopen(filename, "a");
    if (fs) {
        fputs(writemsg, fs);
        fclose(fs);
    }

    fputs(writemsg, stdout);
    fflush(stdout);
}
/*
char *read_config(const char *filename, char *section, char *item) {
    FILE *fp;
    char *locate;
    char *pmove;
    char *
}
*/
char *get_cfg(const char *filename, char *section, char *item)
{
    FILE *fp;
    char *locate = NULL;
    char *pmove = NULL;
    char confLine[CFG_LEN],locsection[CFG_LEN],contextmsg[CFG_LEN];
    int result = 1;
    char *pline;
    int itl;
    int iSectionFind;


    if((fp = fopen(filename, "r")) == NULL)
    {
        write_log(__FILE__,__LINE__,"Open file : %s failed!!", filename);
        return NULL;
    }
    memset(contextmsg,0,sizeof(contextmsg));
    iSectionFind = 0;
    while(fgets(confLine, CFG_LEN, fp) != NULL)
    {
        pline = confLine;
        if(*pline == '#')
        {
            memset(confLine,0,sizeof(confLine));
            continue;
        }
        while(isspace(*pline) != 0) pline++;
        trim(pline);
        if(pline[strlen(pline) -1]=='\n')   pline[strlen(pline) - 1]='\0';

        if(!iSectionFind)/*直到找到section所在的行*/
        {
            memset(locsection,0,sizeof(locsection));
            sprintf(locsection,"[%s]",section);
            result = strcasecmp(pline,locsection);
            if(result == 0) iSectionFind = 1;
            continue;
        }

        if(*pline == 91) break;/*在该section内没有找到对应的item*/

        locate = strchr(pline, '=');
        if(locate == NULL)
        {
            memset(confLine, 0, sizeof(confLine));
            continue;
        }
        pmove = locate;
        pmove--;
        while(isspace(*pmove) != 0)pmove--;
        itl = pmove - pline + 1;
        if(itl == strlen(item))
        {
            result = strncasecmp(pline, item, itl);
            if(result == 0)
            {
                locate++;
                while(isspace(*locate) != 0)locate++;
                pmove = locate;
                while((isspace(*pmove) == 0 || *pmove==32) && (*pmove>0)) pmove++;
                if(pmove-locate+1>0)
                {
                    strncpy(contextmsg, locate, pmove-locate+1);
                    break;
                }
                else
                {
                    return NULL;
                }
            }
            else
            {
                memset(confLine, 0, sizeof(confLine));
                continue;
            }
        }
        else
        {
            memset(confLine, 0, sizeof(confLine));
            continue;
        }
    }
    fclose(fp);

    return trim(contextmsg);
}
