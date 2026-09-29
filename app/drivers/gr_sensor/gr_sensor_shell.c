#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

static const struct device * gr_sensor = DEVICE_DT_GET(DT_NODELABEL(gr_sensor0));

static int cmd_demo_fetch(const struct shell * sh, size_t argc, char **argv) {
    int ret = sensor_sample_fetch(gr_sensor);
    shell_print(sh, "GR sensor command fetch returned: %d", ret);
    return 0;
}

static int cmd_demo_read(const struct shell * sh, size_t argc, char **argv) {
    struct sensor_value val;
    int ret = sensor_channel_get(gr_sensor, SENSOR_CHAN_AMBIENT_TEMP, &val);
    shell_print(sh, "GR sensor command returned %d and read: %d", ret, val.val1);
    return 0;
}

static int cmd_demo_info(const struct shell * sh, size_t argc, char **argv) {
    shell_print(sh, "Name: %s, State: %d, Initialized: %s", gr_sensor->name, gr_sensor->state->init_res, gr_sensor->state->initialized?"Yes":"No");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "Sensor sample fetch.", cmd_demo_fetch),
    SHELL_CMD(read, NULL, "Sensor channel get.", cmd_demo_read),
    SHELL_CMD(info, NULL, "Device name and state.", cmd_demo_info),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "GR Sensor shell commands", NULL);