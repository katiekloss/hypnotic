#include <time.h>
#include "libhdhomerun/hdhomerun_os_posix.h"
#include "libhdhomerun/hdhomerun_sock.h"
#include "libhdhomerun/hdhomerun_pkt.h"
#include "libhdhomerun/hdhomerun_debug.h"
#include "libhdhomerun/hdhomerun_video.h"

void RecordBackend(const char *device_id, int channel, int program, time_t end, const char *filename)
{
    struct hdhomerun_device_t *device;
    char *channelspec;
    char *programstr;
    size_t read_size;

    device = (struct hdhomerun_device_t *) hdhomerun_device_create_from_str(device_id, NULL);
    if(hdhomerun_device_set_tuner(device, 0) <= 0)
    {
        printf("Failed to initialize tuner\n");
        exit(1);
    }

    FILE *file = fopen(filename, "wb");
    if(!file)
    {
        printf("Failed to open save.mpeg2 for writing\n");
        exit(1);
    }

    channelspec = malloc(8);
    sprintf(channelspec, "auto:%d", channel);
    if(hdhomerun_device_set_tuner_channel(device, channelspec) <= 0)
    {
        printf("Failed to set channel with channelspec \"%s\"\n", channelspec);
        exit(1);
    }

    programstr = malloc(3);
    sprintf(programstr, "%d", program);
    if(hdhomerun_device_set_tuner_program(device, programstr) <= 0)
    {
        printf("Failed to set program to %s\n", programstr);
        exit(1);
    }

    if(hdhomerun_device_stream_start(device) <= 0)
    {
        printf("Failed to start stream\n");
        exit(1);
    }

    while(1)
    {
        if(time(NULL) >= end)
        {
            break;
        }

        void *buffer = hdhomerun_device_stream_recv(device, VIDEO_DATA_BUFFER_SIZE_1S, &read_size);
        if(!buffer)
        {
            msleep_approx(64);
            continue;
        }

        if(fwrite(buffer, 1, read_size, file) != read_size)
        {
            printf("Failed to write stream\n");
            exit(1);
        }

        msleep_approx(64);
    }

    fclose(file);
    free(channelspec);
    free(programstr);
    hdhomerun_device_stream_stop(device);
}
