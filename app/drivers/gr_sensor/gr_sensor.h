#ifndef GR_SENSOR_H
#define GR_SENSOR_H

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

void gr_sensor_set_amp(const struct device *dev, uint32_t amp);

#ifdef __cplusplus
}
#endif


#endif