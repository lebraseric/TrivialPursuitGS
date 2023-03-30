/*
 *  net.h -- Network device routines
 */


#ifndef NET_H
#define NET_H

#include "sp.h"
#include <stdint.h>

int net_open_url(const uint8_t sp_net, const uint8_t mode,
                 const uint8_t trans, const char *url);

#endif /* NET_H */