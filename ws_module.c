
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "ws.h"

static int ws_init(void)
{
    printk(KERN_INFO "WS Loading...\n");

    push_data("tasklet", NULL, 0);

    printk(KERN_INFO "WS Loaded\n");

    return 0;
}

static void ws_exit(void)
{
    printk(KERN_INFO "WS exiting...\n");

    ws_tasklet_exit();

    printk(KERN_INFO "WS Exited\n");
}

module_init(ws_init);
module_exit(ws_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Duncan Palmer");
MODULE_DESCRIPTION("Demonstrate APIs for driver development.");

