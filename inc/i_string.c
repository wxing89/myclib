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


#include <string.h>
#include <regex.h>

#include "i_string.h"

char *trim(char *str) {
    return trim_right(trim_left(str));
}

char *trim_left(char *str) {
    register char *temp = str;
    if (temp) {
        while (isspace(*temp++)) {
            ;
        }
        strcpy(str, temp - 1);
    }
    
    return str;
}

char *trim_right(char *str) {
    register int tail;
    if (str) {
        for(tail = strlen(str) - 1; tail > 0 && (isspace(str[tail]) || !isprint(str[tail])); --tail)
            ;

        str[tail + 1] = '\0';
    }

    return str;
}

char *str_tolower(char *str) {
    size_t i;
    for (i = 0; str[i] != '\0'; ++i) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 'a' - 'A';
        }
    }
    
    return str;
}

char *str_toupper(char *str) {
    size_t i;
    for (i = 0; str[i] != '\0'; ++i) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] += 'A' - 'a';
        }
    }
    
    return str;
}

char *substr(char *sub, const char *str, size_t index, size_t len) {
    strncpy(sub, str + index, len);
    *(sub + len) = '\0';
    return sub;
}


size_t str_split(char **arr, const char *str, char delim) {
    size_t i, j, n;
    
    for (i = j = n = 0; (arr[i][j] = str[n]) != '\0'; ++j, ++n) {
        if (str[n] == delim) {
            arr[i][j] = '\0';
            j = -1;
            ++i;
        }
    }
    arr[i][j] = '\0';
    return i + 1;
}

size_t str_split2(char **arr, const char *str, char *delim) {
    char *p, *p1;
    size_t i, len;
    
    p1 = (char *)str;
    
    i = 0;
    while ((p = strstr(p1, delim)) != NULL) {
        len = p - p1;
        memcpy(arr[i], p1, len);
        arr[i++][len] = '\0';
        p1 = p + strlen(delim);
    }
    strcpy(arr[i], p1);

    return i + 1;
}

char *str_field(char *field, const char *str, char delim, size_t index) {
    if (index == 0) {
        strcpy(field, str);
        return field;
    }
    
    size_t i, j, k;
    for (i = j = k = 0; str[j] != '\0' && i < index; ++j) {
        if (i == index - 1 && str[j] != delim)
            field[k++] = str[j];
        
        if (str[j] == delim)
            ++i;
    }
    field[k] = '\0';
    
    return field;
}

char *str_field2(char *field, const char *str, char *delim, size_t index) {
    if (index == 0) {
        strcpy(field, str);
        return field;
    }
    
    size_t i, j;
    char *p, *p1;
    
    p1 = (char *)str;
    for (i = j = 0; (p = strstr(p1, delim)) != NULL && i < index; ++i) {
        if (i == index - 1) {
            j = p - p1;
            memcpy(field, p1, j);
        }    
        p1 = p + strlen(delim);
    }
    field[j] = 0;
    
    if (i == index - 1)
        strcpy(field, p1);
    
    return field;
}


void str_swap(char *str1, char *str2) {
    char *temp = (char *)malloc((strlen(str1) + 1) * sizeof(char));
    strcpy(temp, str1);
    strcpy(str1, str2);
    strcpy(str2, temp);
     free(temp);
}

char *str_reverse(char *str) {
    char *start = str;
    char *end = str;
    
    while(*end) {
        ++end;
    }
    --end;
    
    for(; end > start; --end, ++start) {
        *end ^= *start;
        *start ^= *end;
        *end ^= *start;
    }
    
    return str;
}

char *str_replace(char *str2, const char *str1, const char *src, const char *dst) {
    if (!str1 || !src || !dst) 
        return NULL;

    char *p;
    if (!(p = strstr(str1, src))) {
        strcpy(str2, str1);
        return str2;
    }

    strncpy(str2, str1, p - str1);
    str2[p - str1] = 0;
    strcat(str2, dst);
    strcat(str2, p + strlen(src));
    return str2;
}

char *str_replaceall(char *str2, const char *str1, const char *src, const char *dst) {
    if (!str1 || !src || !dst) 
        return NULL;

    char *p, *p1, *p2;
    size_t srclen, dstlen, len;

    p1 = (char *)str1;
    p2 = str2;
    srclen = strlen(src);
    dstlen = strlen(dst);

    while (p = strstr(p1, src)) {
        len = p - p1;
        memcpy(p2, p1, len);
        memcpy(p2 + len, dst, dstlen);
        p1 = p + srclen;
        p2 = p2 + len + dstlen;
    }

    strcpy(p2, p1);

    return str2;
}

void str_sort(char **strarr, size_t nmemb) {
    size_t i, j;
    char *tmp;
    for (i = 0; i < nmemb; ++i) {
        for (j = i + 1; j < nmemb; ++j) {
            if (strcmp(strarr[i], strarr[j]) > 0) {
                tmp = strarr[i];
                strarr[i] = strarr[j];
                strarr[j] = tmp;
            }
        }
    }
}

static int cmpstrp(const void *p1, const void *p2) {
    return strcmp(* (char * const *) p1, * (char * const *) p2);
}


void str_qsort(char **strarr, size_t nmemb) {
    qsort(strarr, nmemb, sizeof(char *), cmpstrp);
}

int reg_match(const char *str, const char *pattern) {
    regmatch_t pmatch[1];
    size_t nmatch = 1;
    regex_t reg;
    size_t err;
    
    char *regstr;
    regstr = malloc((strlen(pattern) + 3) * sizeof(char));
    strcpy(regstr, "^");
    strcat(regstr, pattern);
    strcat(regstr, "$");

    
    err = regcomp(&reg, regstr, REG_EXTENDED);
    free(regstr);

    /* if function regcomp failed, i.e. regular expression is invalid */
    if (err) 
        return -1;
    
    err = regexec(&reg, str, nmatch, pmatch, 0);
    regfree(&reg);

    return err;
}
