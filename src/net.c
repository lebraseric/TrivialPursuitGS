/*
 *  net.c -- Network device routines
 */


segment "fujinet";

#include "net.h"
#include "sp.h"
#include <stdint.h>
#include <string.h>
#include <window.h>

int net_open_url(const uint8_t sp_net, const uint8_t mode,
                 const uint8_t trans, const char *url)
{
  unsigned idx = 0;
  size_t s;

  // to do - copy strings into payload and figure out length
  s = 1 + 1 + strlen(url);
  sp_payload[idx++] = s & 0xFF;
  sp_payload[idx++] = s >> 8;
  sp_payload[idx++] = mode;
  sp_payload[idx++] = trans;

  strcpy(&sp_payload[idx++], url);

  return sp_control(sp_net, 'O');
}
