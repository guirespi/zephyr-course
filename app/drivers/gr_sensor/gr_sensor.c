#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT gr_sensor

LOG_MODULE_REGISTER(gr_sensor, LOG_LEVEL_INF);

/* The devicetree node identifier for the "user_led" alias. Modified by app.overlay */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

static bool led_state = true;

int sample_fetch_gr_sensor(const struct device *dev, enum sensor_channel chan) 
{
    LOG_INF("Sample Fetch reached, channel %d", chan);

    if (gpio_pin_set_dt(&led, 1) != 0) return 0;
    led_state = true;
    LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
    
    return 0;
}

int channel_get_gr_sensor(const struct device *dev,
				    enum sensor_channel chan,
				    struct sensor_value *val) 
{
    
    LOG_INF("Channel Get reached, channel %d", chan);
    
    if (gpio_pin_set_dt(&led, 0) != 0) return 0;
    led_state = false;
    LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
    
    return 0;
}

static DEVICE_API(sensor, api_gr_sensor) = {
    .channel_get = channel_get_gr_sensor,
    .sample_fetch = sample_fetch_gr_sensor,
};

// Init fn
static int init_gr_sensor(const struct device *dev) {
    // Check GPIO
    if (!gpio_is_ready_dt(&led)) return 0;
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    LOG_INF("GR Sensor Initialized");
    return 0;
}

DEVICE_DT_INST_DEFINE(0, init_gr_sensor, NULL, NULL, NULL, POST_KERNEL, 80, &api_gr_sensor);