
#include <linux/module.h>
#include <linux/string.h>

#include "ws.h"

int process_cmd(const char *cmd, const uint8_t *data, size_t datalen)
{
    if (strcmp("tasklet", cmd) == 0)
        return ws_tasklet_process(data, datalen);
    if (strcmp("wq", cmd) == 0)
        return ws_wq_process(data, datalen);
    if (strcmp("thread", cmd) == 0)
        return ws_kthread_process(data, datalen);

    printk(KERN_ERR "Unknown cmd: %s", cmd);
    return -EINVAL;
}



