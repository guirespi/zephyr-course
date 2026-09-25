#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#ifndef CONFIG_GR_SENSOR
/* The devicetree node identifier for the "user_led" alias. Modified by app.overlay */
#define LED_NODE DT_ALIAS(app_led)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
#endif

#ifdef CONFIG_GR_SENSOR
static const struct device * gr_sensor = DEVICE_DT_GET(DT_NODELABEL(gr_sensor0));
#endif

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
#ifndef CONFIG_GR_SENSOR
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
#else
    bool state = true;
    while (1) {
        struct sensor_value val;
        if(state) {
            auto ret = sensor_channel_get(gr_sensor, SENSOR_CHAN_AMBIENT_TEMP, &val);
        } else {
            auto ret = sensor_sample_fetch(gr_sensor);
        }
        state = !state;
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
#endif
    return 0;
}
