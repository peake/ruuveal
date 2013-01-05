#include <string.h>

#include "htc/devices.h"
#include "htc/keydata.h"
#include "htc/keymap.h"

static int get_keydata_offset(const char *device)
{
    int i;

    for(i = 0; i < sizeof(htc_devices); i++) {
        if(!strcmp(htc_devices[i].name, device)) {
            return htc_devices[i].keydata_offset;
        }
    }

    return -1;    
}

int htc_generate_aes_keys(const char *device, int keymap_offset, char *aeskey, 
                          char *aesiv)
{
    char *keymap, *keydata;
    int i, offset;
    

    if((offset = get_keydata_offset(device)) == -1) {
        return 0;
    }
    
    if((keymap_offset - 1)<<5 >= sizeof(htc_keymap)) {
        return 0;
    }

    keymap = &htc_keymap[(keymap_offset-1)<<5];
    keydata = &htc_keydata[offset * 96];
    
    for(i=0; i < 0x10; i++) {
        aeskey[i] = keydata[keymap[i]];
        aesiv[i]  = keydata[keymap[i + 0x10]];
    }

    return 1;
}