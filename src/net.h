/*
 *  net.h -- Network device routines
 */


#ifndef NET_H
#define NET_H

#include "sp.h"
#include <stdint.h>

int net_open_url(uint8_t sp_net, uint8_t mode, uint8_t trans, const char *url);
int net_close_url(uint8_t sp_net);
int net_set_json(uint8_t sp_net);

#endif /* NET_H */