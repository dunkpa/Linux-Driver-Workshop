
#include <linux/module.h>
#include <linux/timer.h>

#include "ws.h"

struct timer_list timer;

static void ws_timer(unsigned long data)
{
    printk(KERN_INFO "Timer fired\n");

    /* I have often seen code which queues a job on a work queue here, as
     * we can't sleep in a timer. Generally, we can use the 
     * schedule_delayed_work() work queue API instead. */
}

int ws_timer_process(const uint8_t *data, size_t datalen)
{
    /* Set the timer to fire in 10 milliseconds. */
    mod_timer(&timer, msecs_to_jiffies(10));
    return 0;
}

int ws_timer_init(void) 
{
    /* Create the timer. The value of 0 is passed to the timer func.This
     * can be done statically with DEFINE_TIMER(). */
    setup_timer(&timer, ws_timer, 0);
    return 0;
}

void ws_timer_exit(void)
{
    del_timer_sync(&timer);
}

