
#ifndef _WS_H
#define _WS_H

/* Feed data into our module for processing. Depending on the value of
 * type, data is processed using a tasklet, thread or workqueue. */
int push_data(const char *type, const uint8_t *data, size_t datalen);

/* Things in ws_tasklet.c */

void ws_tasklet_exit(void);

/* ws_interface.c */
int ws_intf_init(void);
void ws_intf_exit(void);

#endif

