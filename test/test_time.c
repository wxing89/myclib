#include "../inc/i_time.h"
            
#include <string.h>                                                                
#include <stdio.h>
                
int main()  
{               
    char s[24];
    char t[24];
        
    get_datefmt(s, "%F %T");                                                       
    printf("current date        - %s\n", s);

    modi_time(t, s, "%F %T", -48, DATE_TYPE_HOUR);                                 
    printf("48 hours before     - %s\n", t);

    modi_time(t, s, "%F %T", -180, DATE_TYPE_MIN);                                 
    printf("180 minutes before  - %s\n", t);

    modi_time(t, s, "%F %T", -180, DATE_TYPE_SEC);                                 
    printf("180 seconds before  - %s\n", t);

}
