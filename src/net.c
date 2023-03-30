/*
 *  net.c -- Network device routines
 */


segment "fujinet";

#include "net.h"
#include "sp.h"
#include <stdint.h>
#include <string.h>
#include <window.h>

int net_open_url(uint8_t sp_net, uint8_t mode, uint8_t trans, const char *url)
{
  unsigned idx = 0;
  size_t s;

  s = 1 + 1 + strlen(url);
  sp_payload[idx++] = s & 0xFF;
  sp_payload[idx++] = s >> 8;
  sp_payload[idx++] = mode;
  sp_payload[idx++] = trans;
  strcpy(&sp_payload[idx++], url);
  return sp_control(sp_net, 'O');
}

int net_close_url(uint8_t sp_net)
{
  unsigned idx = 0;

  sp_payload[idx++] = 0;
  sp_payload[idx++] = 0;
  return sp_control(sp_net, 'C');
}

int net_set_json(uint8_t sp_net)
{
  unsigned idx = 0;

  sp_payload[idx++] = 0x01; // length of packet.
  sp_payload[idx++] = 0x00;
  sp_payload[idx++] = 0x01; // Set to JSON mode
  return sp_control(sp_net, 0xFC); // Do it.
}