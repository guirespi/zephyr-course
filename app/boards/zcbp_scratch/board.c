#include<zephyr/init.h>
#include<zephyr/kernel.h>

static int board_zcbp_scratch_init(void) {
    /* Some function to run before boot */    
    printk("Board Initialized - ZCBP from scratch\n");
    return 0;
}