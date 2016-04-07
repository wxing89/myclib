/*******************************************************
 * File: 
 *      i_file.c
 * Description:
 *      file methods
 * Author: 
 *      w.x
 * History:
 *      2013.08.01 file created
 * 
 *
*******************************************************/


#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#include "i_string.h"
#include "i_file.h"


int get_line(char *line, FILE *fs) {
    char *tmp;
    size_t len = 0;

    int n;
    n = getline(&tmp, &len, fs);
    if (n < 0) {
        free(tmp);
        return -1;
    }

    size_t i;
    for(i = strlen(tmp) - 1; i >= 0 && !isprint(tmp[i]); --i)
        ;
    tmp[i + 1] = '\0';

    strcpy(line, tmp);
    free(tmp);

    return n;
}

static void sort_file(char **file_arr, size_t nmemb) {
    size_t i, j;
    for (i = 0; i < nmemb; ++i)
        for (j = i + 1; j < nmemb; ++j)
            if (strcmp(file_arr[i], file_arr[j]) > 0) 
                str_swap(file_arr[i], file_arr[j]);
}


int get_file(char **file_arr, const char *file_dir, const char *pattern) {
    char file_path[FILE_NAME_LEN];

    DIR *dir = NULL;
    struct dirent *dp;

    errno = 0;
    if ((dir = opendir(file_dir)) == NULL) {
        printf("OPEN DIR %s ERROR!", file_dir);
        perror(strerror(errno));
        return -1;
    }

    unsigned i = 0;
    int ismatch;
    while ((dp = readdir(dir)) != NULL) {
        struct stat st;
        snprintf(file_path, FILE_NAME_LEN, "%s/%s", file_dir, dp->d_name);

        if (reg_match(dp->d_name, pattern) == 0) {
            if (stat(file_path, &st) == 0 && S_ISREG(st.st_mode)) {
                strcpy(file_arr[i], dp->d_name);
                ++i;
            }
        }
    }

    closedir(dir);

    sort_file(file_arr, i);
    return i;
}

int cp_file(const char *file_src, const char *file_dst) {
    int src, dst;

    struct stat src_st;
    if(stat(file_src, &src_st) == -1) {
        printf("STAT SRC %s ERROR!\n", file_src);
        return -1;
    }

    if ((src = open(file_src, O_RDONLY)) == -1) {
        printf("OPEN SRC %s ERROR!\n", file_src);
        return -1;
    }

    if ((dst = open(file_dst, O_RDWR | O_CREAT, src_st.st_mode)) == -1) {
        printf("OPEN DST %s ERROR!\n", file_dst);
        return -1;
    }

    char buffer[BUFFER_LEN];
    int n;
    while ((n = read(src, buffer, BUFFER_LEN)) > 0) {
        if (write(dst, buffer, n) != n) {
            printf("WRITE ERROR!\n");
            return -1;
        }
    }

    close(src);
    close(dst);

    if (n < 0) {
        printf("READ ERROR!\n");
        return -1;
    }

    return 0;
}

int cp_file2(const char *file_src, const char *file_dst) {
    FILE *src;
    FILE *dst;

    src = fopen(file_src, "r");
    dst = fopen(file_dst, "w");
    if (!src || !dst) 
        printf("OPEN ERROR!\n");

    fseek(src, 0, SEEK_SET);
    char buffer[BUFFER_LEN];
    int bytes_in, bytes_out;
    while ((bytes_in = fread(buffer, 1, BUFFER_LEN, src)) > 0) {
        bytes_out = fwrite(buffer, 1, bytes_in, dst);
        if (bytes_in != bytes_out) {
            printf("ERROR!\n");
            return -1;
        }
    }

    fclose(src);
    fclose(dst);
}

int mv_file(const char *file_src, const char *file_dst) {
    if (rename(file_src, file_dst)) {
        if (cp_file(file_src, file_dst))
            return -1;
        if (unlink(file_src))
            return -1;
        return 0;
    }
    return 0;
}

int app_file(const char *file_src, const char *file_dst) {
    int src, dst;

    struct stat src_st;
    if(stat(file_src, &src_st) == -1) {
        printf("STAT SRC %s ERROR!\n", file_src);
        return -1;
    }

    if ((src = open(file_src, O_RDONLY)) == -1) {
        printf("OPEN SRC %s ERROR!\n", file_src);
        return -1;
    }

    if ((dst = open(file_dst, O_RDWR | O_APPEND | O_CREAT, src_st.st_mode)) == -1) {
        printf("OPEN DST %s ERROR!\n", file_dst);
        return -1;
    }

    char buffer[BUFFER_LEN];
    int n;
    while ((n = read(src, buffer, BUFFER_LEN)) > 0) {
        if (write(dst, buffer, n) != n) {
            printf("WRITE ERROR!\n");
            return -1;
        }
    }

    close(src);
    close(dst);

    if (n < 0) {
        printf("READ ERROR!\n");
        return -1;
    }

    return 0;
}




