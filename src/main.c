#include "cdcacm.h"
#include "platform.h"

#include "version.h"

typedef struct {
    uint32_t namesz;
    uint32_t descsz;
    uint32_t type;
    uint8_t data[];
} note_section_t;

/* Defined in linker script. */
extern const note_section_t _build_id;

static char bin2hex(uint8_t b)
{
    return b < 10 ? '0' + b : 'A' + (b - 10);
}

static void hexencode(char *str, const uint8_t *bin, uint8_t len)
{
    assert(bin);
    assert(str);
    uint8_t b;
    // Encode bytes.
    for (int i = 0; i < len; i++)
    {
        // Encode first char.
        b = (bin[i] >> 4) & 0x0F;
        *str++ = bin2hex(b);
        // Encode second char.
        b = bin[i] & 0x0F;
        *str++ = bin2hex(b);
    }
}

static const char *get_build_id(int *len)
{
    static char build_id[40];
    assert(_build_id.descsz * 2 <= sizeof(build_id));
    const uint8_t *build_id_data = &_build_id.data[_build_id.namesz];
    hexencode(build_id, build_id_data, _build_id.descsz);
    *len = _build_id.descsz * 2;
    return build_id;
}

static const char *get_firmware_version(int *len)
{
    *len = strlen(FIRMWARE_VERSION);
    return FIRMWARE_VERSION;
}

static bool heartbeat(int delay_ms)
{
    static uint32_t last_tick_ms = 0;
    const uint32_t now_ms = platform_get_time();
    const int enough = now_ms - last_tick_ms - delay_ms;
    if (enough >= 0)
    {
        last_tick_ms = now_ms;
        platform_led_toggle();
        return true;
    }
    return false;
}

int main(void)
{
    platform_init();
    cdcacm_init();
    int build_id_len;
    int fw_version_len;
    const char *build_id = get_build_id(&build_id_len);
    const char *fw_version = get_firmware_version(&fw_version_len);
    while (1)
    {
        if (!cdcacm_get_configuration())
            heartbeat(50);
        else
        {
            if (heartbeat(1000))
            {
                cdcacm_write_now("beat fw=", 8);
                cdcacm_write_now(fw_version, fw_version_len);
                cdcacm_write_now(" build=", 7);
                cdcacm_write_now(build_id, build_id_len);
                cdcacm_write_now("\n", 1);
            }
        }
    }
    return 0;
}
