
/* Tasklet management. */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/interrupt.h>

#include "ws.h"

/* Declarations. */
static void ws_data_tasklet_func(unsigned long data);


/* Statically declare and initialise a tasklet. Initialisation can also be done
 * at runtime using tasklet_init(). The following is equivalent to saying
 *
 *      struct tasklet_struct ws_data_tasklet = {
 *              NULL, 0, ATOMIC_INIT(0),
 *              ws_data_tasklet_func, NULL};
 *
 * The final parameter, which we set to NULL, is the value of the 'data' 
 * parameter of ws_data_tasklet_func().
 */
DECLARE_TASKLET(ws_data_tasklet, ws_data_tasklet_func, 0);

static void ws_data_tasklet_func(unsigned long data)
{
    printk(KERN_INFO "In tasklet");
}

int ws_tasklet_process(const uint8_t *data, size_t datelen) 
{

    /* For the moment, we're not pushing the data anywhere. We're just running
     * the tasklet... */

    /* Add ws_data_tasklet to the tasklet run queue. If it is already on this
     * queue, this call does nothing. */
    tasklet_schedule(&ws_data_tasklet);

    return 0;
}

void ws_tasklet_exit(void)
{
    /* Remove ws_data_tasklet from the tasklet run queue. If it is executing,
     * wait for it to finish. */
    tasklet_kill(&ws_data_tasklet);
}

