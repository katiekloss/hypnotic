#include "discover.h"
#include "record.h"
#include "schedule.h"

#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        if(!strncmp(argv[1], "discover", 8))
        {
            printf("Dumping channel list into database\n");
            discover();
        }
        else if(!strncmp(argv[1], "record", 6))
        {
            if(argc == 7)
            {
                int channel, program;
                time_t end;
                
                sscanf(argv[3], "%d", &channel);
                sscanf(argv[4], "%d", &program);
                sscanf(argv[5], "%d", &end);
                
                printf("Starting recording\n");
                RecordBackend(argv[2], channel, program, end, argv[6]);
            } else {
                printf("Usage: hypnotic record <channel> <program number> <end epoch> <filename>\n");
                exit(0);
            }
        }
        else if(!strncmp(argv[1], "schedule_once", 13))
        {
            process_schedules();
        }
    }
    return 0;
}
