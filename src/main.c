#include "cdcacm.h"
#include "platform.h"

#include "version.h"

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
    while (1)
    {
        if (!cdcacm_get_configuration())
            heartbeat(50);
        else
        {
            if (heartbeat(1000))
            {
                cdcacm_write_now("beat ", 6);
                cdcacm_write_now(FIRMWARE_VERSION, sizeof(FIRMWARE_VERSION) - 1);
                cdcacm_write_now("\n", 1);
            }
        }
    }
    return 0;
}
