#include <string.h>
#include <stdint.h>
#include <sqlite3.h>
#include <stdio.h>
#include "libhdhomerun/hdhomerun_os_posix.h"
#include "libhdhomerun/hdhomerun_pkt.h"
#include "libhdhomerun/hdhomerun_discover.h"

void discover()
{
    int i, j;
    sqlite3 *connection;
    sqlite3_stmt *stmt;
    
    int error = sqlite3_open("database.sqlite3", &connection);
    if(error)
    {
        printf("Failed to open database.\n");
        exit(1);
    }

    struct hdhomerun_discover_device_t *devices = (struct hdhomerun_discover_device_t *) malloc(sizeof(struct hdhomerun_discover_device_t) * 10);
    hdhomerun_discover_find_devices_custom(0, HDHOMERUN_DEVICE_TYPE_TUNER, HDHOMERUN_DEVICE_ID_WILDCARD, devices, 10);
    for(i = 0; i < 10; i++)
    {
        if(devices[i].device_id == 0) break;
        struct hdhomerun_device_t *device = (struct hdhomerun_device_t *) hdhomerun_device_create(devices[i].device_id, devices[i].ip_addr, 0, 0);
        for(j = 2; j < 50; j++)
        {
            char channelspec[8];
            sprintf(channelspec, "auto:%i", j);
            if(hdhomerun_device_set_tuner_channel(device, channelspec) == 0)
            {
                continue;
            } else {
                char *info = malloc(100);
                char *token;
                sleep(2);
                hdhomerun_device_get_tuner_streaminfo(device, &info);
                
                int init = 1;
                int program_number;
                char *program_name = malloc(30);
                while (1)
                {
                    if(init)
                    {
                        token = strtok(info, "\n");
                        if(!strncmp(token, "none", 4)) break;
                        init = 0;
                    } else {
                        token = strtok(NULL, "\n");
                    }
                    
                    if(token == NULL) break;
                    if(!strncmp(token, "tsid", 4)) break;
                    
                    sscanf(token, "%d: %[^]", &program_number, program_name);
                    
                    // Trim trailing whitespace
                    char *end = program_name + strlen(program_name) - 1;
                    while(end > program_name && isspace(*end)) end--;
                    *(end + 1) = 0;

                    char sql[100];
                    sprintf(sql, "SELECT * FROM Channels WHERE tunerid=\"%X\" AND channel=%d AND program=%d", devices[i].device_id, j, program_number);
                    error = sqlite3_prepare_v2(connection, sql, 100, &stmt, NULL);
                    if(error != SQLITE_OK)
                    {
                        printf("Failed to compile SQL statement!\n");
                        exit(1);
                    }
                    if(sqlite3_step(stmt) != SQLITE_ROW)
                    {
                        char sql2[500];
                        sprintf(sql2, "INSERT INTO Channels VALUES (\"%X\", %d, %d, \"%s\")", devices[i].device_id, j, program_number, program_name);
                        error = sqlite3_exec(connection, sql2, NULL, NULL, NULL);
                        if(error != SQLITE_OK)
                        {
                            printf("SQL execution failed\n");
                            exit(1);
                        }
                        printf("Discovered new channel %d:%d (\"%s\")\n", j, program_number, program_name);
                    }
                }
                free(program_name);
            }
        }
    }
    free(devices);
}
