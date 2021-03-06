
/* The external interface with the ws driver. Currently, this uses debugfs
 * to get data into and out of the driver. To use this, echo commands into
 * /sys/kernel/debug/ws/ctrl e.g.
 *
 *  echo "tasklet" > /sys/kernel/debug/ws/ctrl
 */

#include <linux/string.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "ws.h"

static struct dentry *root = NULL;
static struct dentry *ws_ctrl = NULL;

static ssize_t ws_ctrl_write(struct file *file, const char __user *buf,
        size_t count, loff_t *ppos)
{
    char *ctrl;
    char *ctrl_arg;
    char *ctrl_sep = " ";
    size_t ctrl_arg_len;
    ssize_t rv = 0;
    ssize_t err = 0;

    ctrl = kmalloc(count, GFP_KERNEL);
    if (ctrl == NULL)
        return -ENOMEM;
    rv = count;

    err = copy_from_user(ctrl, buf, count);
    if (err != 0)
    {
        rv = -ENOSPC;
        goto exit;
    }

    /* We expect a command string consisting of a command followed by a space,
     * and then command arguments. There may be no arguments, and hence no 
     * space. */
    ctrl_arg = strstr(ctrl, ctrl_sep);
    if (ctrl_arg != NULL)
    {
        ctrl_arg_len = count - (ctrl_arg - ctrl);
        *ctrl_arg = '\0';
        ctrl_arg++;
    }
    else
    {
        ctrl_arg_len = 0;
        /* Clip the trailing newline. */
        if (strlen(ctrl))
            ctrl[strlen(ctrl)-1] = '\0';
    }

    err = process_cmd(ctrl, ctrl_arg, ctrl_arg_len);
    if (err < 0)
    {
        rv = err;
        goto exit;
    }

exit:
    kfree(ctrl);
    return rv;
}

static struct file_operations fops =
{
    .write = ws_ctrl_write,
};

int ws_intf_init(void)
{
    root = debugfs_create_dir("ws", NULL);
    if (root == NULL)
    {
        printk(KERN_ERR "Failed to create debugfs entry.\n");
        return -EINVAL;
    }

    ws_ctrl = debugfs_create_file("ctrl", 0666, root, NULL, &fops);
    if (ws_ctrl == NULL)
    {
        printk(KERN_ERR "Failed to create debugfs entry.\n");
        return -EINVAL;
    }

    return 0;
}

void ws_intf_exit(void)
{
    if (ws_ctrl)
        debugfs_remove(ws_ctrl);
    if (root)
        debugfs_remove(root);
}

