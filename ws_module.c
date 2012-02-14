
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "ws.h"

static int ws_init(void)
{
    int rv = 0;

    printk(KERN_INFO "ws loading...\n");

    rv = ws_intf_init();
    if (rv < 0)
        goto exit;
    rv = ws_kthread_init();
    if (rv < 0)
        goto exit;
    rv = ws_tasklet_init();
    if (rv < 0)
        goto exit;
    rv = ws_wq_init();
    if (rv < 0)
        goto exit;


exit:
    if (rv != 0)
        printk(KERN_ERR "ws failed to load.\n");
    else
        printk(KERN_INFO "ws loaded.\n");
    return rv;
}

static void ws_exit(void)
{
    printk(KERN_INFO "ws exiting...\n");

    ws_kthread_exit();
    ws_wq_exit();
    ws_tasklet_exit();
    ws_intf_exit();

    printk(KERN_INFO "ws exited.\n");
}

module_init(ws_init);
module_exit(ws_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Duncan Palmer");
MODULE_DESCRIPTION("Demonstrate APIs for driver development.");

