#include <string.h>
#include <stdint.h>
#include "libhdhomerun/hdhomerun_os_posix.h"
#include "libhdhomerun/hdhomerun_pkt.h"
#include "libhdhomerun/hdhomerun_discover.h"

void discover_device()
{
    struct hdhomerun_discover_device_t *device = (struct hdhomerun_discover_device_t*) malloc(sizeof(struct hdhomerun_discover_device_t));

    hdhomerun_discover_find_devices_custom(0, HDHOMERUN_DEVICE_TYPE_TUNER, HDHOMERUN_DEVICE_ID_WILDCARD, device, 1);
    printf("Device ID: %X\n", device->device_id);
}
