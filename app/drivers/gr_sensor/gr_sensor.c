#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT gr_sensor

LOG_MODULE_REGISTER(gr_sensor, LOG_LEVEL_INF);

int sample_fetch_gr_sensor(const struct device *dev, enum sensor_channel chan) 
{
    LOG_INF("Sample Fetch reached, channel %d", chan);
    return 0;
}

int channel_get_gr_sensor(const struct device *dev,
				    enum sensor_channel chan,
				    struct sensor_value *val) 
{
    LOG_INF("Channel Get reached, channel %d", chan);
    return 0;
}

static DEVICE_API(sensor, api_gr_sensor) = {
    .channel_get = channel_get_gr_sensor,
    .sample_fetch = sample_fetch_gr_sensor,
};

// Init fn
static int init_gr_sensor(const struct device *dev) {
    LOG_INF("GR Sensor Initialized");
    return 0;
}

DEVICE_DT_INST_DEFINE(0, init_gr_sensor, NULL, NULL, NULL, POST_KERNEL, 80, &api_gr_sensor);