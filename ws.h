
#ifndef _WS_H
#define _WS_H

/* Feed data into our module for processing. Depending on the value of
 * cmd, data is processed using a tasklet, thread or workqueue. */
int process_cmd(const char *cmd, const uint8_t *data, size_t datalen);

/* Things in ws_tasklet.c */

void ws_tasklet_exit(void);

/* ws_interface.c */
int ws_intf_init(void);
void ws_intf_exit(void);

/* ws_tasklet.c */
void ws_tasklet_exit(void);
int ws_tasklet_process(const uint8_t *data, size_t datelen);

/* ws_work_queue.c */
int ws_wq_process(const uint8_t *data, size_t datelen);
int ws_wq_init(void);
void ws_wq_exit(void);

#endif

