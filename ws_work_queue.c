
/* Work queue management. */

#include <linux/module.h>
#include <linux/workqueue.h>

#include "ws.h"

static struct work_struct ws_data_wq_handler;

/* Our work queue handler function. */
static void ws_data_wq_func(struct work_struct *unused)
{
    printk(KERN_INFO "In wq handler.\n");
}

int ws_wq_process(const uint8_t *data, size_t datelen)
{
    /* For the moment, we're not doing anything with the data we're passed;
     * we're just running the work handler. */

    /* Schedule ws_data_wq_func() to run on the system work queue.
     *
     * NOTE: This function can be scheduled to run multiple times.
     */
    schedule_work(&ws_data_wq_handler);

    return 0;
}

int ws_wq_init(void) 
{
    /* Create a work queue handler at runtime.
     *
     * It's also possible to statically declare a work queue at compile time.
     * The 3rd parameter, '0', is the parameter passed to ws_data_wq_func(). 
     */
    INIT_WORK(&ws_data_wq_handler, ws_data_wq_func);

    return 0;
}

void ws_wq_exit(void)
{
}

