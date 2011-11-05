#include "discover.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        if(!strncmp(argv[1], "discover", 8))
        {
            printf("Dumping channel list into database\n");
            discover();
        }
    }
    return 0;
}
