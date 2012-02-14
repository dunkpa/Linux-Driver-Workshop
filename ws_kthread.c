
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/wait.h>

#include "ws.h"

static struct task_struct *thread = NULL;
static bool thread_exit = false;
static bool thread_wake = false;
static wait_queue_head_t thread_wake_q;

/* An example of a homebrew worker thread. */
static int ws_kthread(void *unused)
{
    int rv = 0;
    DEFINE_WAIT(wait);

    while (!thread_exit)
    {
        /* Wait for some work. This pattern is the recommended way of doing
         * things, to avoid races. */
        add_wait_queue(&thread_wake_q, &wait);
        while (!thread_wake)
        {
            prepare_to_wait(&thread_wake_q, &wait, TASK_INTERRUPTIBLE);
            /* We're not handling signals properly here; just leave if we get one. */
            if (signal_pending(current))
            {
                printk(KERN_INFO "Caught signal; exiting.\n");
                rv = -EINTR;
                goto exit; 
            }
            schedule();
        }
        finish_wait(&thread_wake_q, &wait);

        thread_wake = false;
    }

exit:
    return rv;
}

int ws_kthread_process(const uint8_t *data, size_t datalen)
{
    /* For now, we don't do anything meaningful; just wake the thread. */

    thread_wake = true;
    wake_up(&thread_wake_q);

    return 0;
}


int ws_kthread_init(void)
{
    /* Create the wake queue we use to wake up our thread. */
    init_waitqueue_head(&thread_wake_q);

    /* Create and start our thread. */
    thread = kthread_run(ws_kthread, NULL, "ws");
    if (IS_ERR(thread))
    {
        printk(KERN_ERR "Failed to create thread.\n");
        return PTR_ERR(thread);
    }

    return 0;
}

void ws_kthread_exit(void)
{
    /* TODO: This is quick'n'dirty. We should ask the thread to exit,
     *       and wait for it to do so. */
    if (thread)
        kthread_stop(thread);
}

