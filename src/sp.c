/**
 * FujiNet CONFIG for #Apple2
 *
 * SmartPort MLI Routines
 */

segment "fujinet";

#include "sp.h"
#include "initdesk.h"

#include <memory.h>
#include <window.h>
#include <misctool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <texttool.h>

#define SP_CMD_STATUS 0
#define SP_CMD_CONTROL 4
#define SP_CMD_OPEN 6
#define SP_CMD_CLOSE 7
#define SP_CMD_READ 8
#define SP_STATUS_PARAM_COUNT 3
#define SP_CONTROL_PARAM_COUNT 3
#define SP_OPEN_PARAM_COUNT 3
#define SP_CLOSE_PARAM_COUNT 3
#define SP_READ_PARAM_COUNT 4

// extern globals:
uint8_t *sp_payload;
uint16_t sp_count;
uint8_t sp_dest;
uint8_t sp_net;
uint16_t sp_dispatch;
uint8_t sp_error;

static Handle sp_handle;
static uint8_t *sp_cmdlist;
static uint8_t *sp_instr;
static FWRec fwRec;
static uint8_t sp_cmdlist_low, sp_cmdlist_high;
static uint8_t sp_dispatch_low, sp_dispatch_high;

int8_t sp_status(uint8_t dest, uint8_t statcode)
{
  sp_error = 0;
  // build the command list
  sp_cmdlist[0] = SP_STATUS_PARAM_COUNT;
  sp_cmdlist[1] = dest; // set before calling sp_status();
  sp_cmdlist[2] = (uint8_t)((uint16_t)sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = statcode;

  sp_cmdlist_low = (uint8_t)((uint16_t)sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)sp_cmdlist >> 8) & 0xFF;
  sp_dispatch_low = (uint8_t)((uint16_t)sp_dispatch & 0x00FF);
  sp_dispatch_high = (uint8_t)((uint16_t)sp_dispatch >> 8) & 0xFF;

  // Make firmware call
  sp_instr[0] = 0x20;	// JSR
  sp_instr[1] = sp_dispatch_low;
  sp_instr[2] = sp_dispatch_high;
  sp_instr[3] = SP_CMD_STATUS;
  sp_instr[4] = sp_cmdlist_low;
  sp_instr[5] = sp_cmdlist_high;
  sp_instr[6] = 0x60;	// RTS

  // Call in emulation mode with FWEntry
  fwRec = FWEntry(0, 0, 0, (Word)sp_instr);
  sp_count = (fwRec.yRegExit << 8) | fwRec.xRegExit;
  sp_error = fwRec.aRegExit;
  return sp_error;
}

int8_t sp_control(uint8_t dest, uint8_t ctrlcode)
{
  sp_error = 0;
  // build the command list
  sp_cmdlist[0] = SP_CONTROL_PARAM_COUNT;
  sp_cmdlist[1] = dest; // set before calling sp_status();
  sp_cmdlist[2] = (uint8_t)((uint16_t)sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = ctrlcode;

  sp_cmdlist_low = (uint8_t)((uint16_t)sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)sp_cmdlist >> 8) & 0xFF;
  sp_dispatch_low = (uint8_t)((uint16_t)sp_dispatch & 0x00FF);
  sp_dispatch_high = (uint8_t)((uint16_t)sp_dispatch >> 8) & 0xFF;

  // Make firmware call
  sp_instr[0] = 0x20;	// JSR
  sp_instr[1] = sp_dispatch_low;
  sp_instr[2] = sp_dispatch_high;
  sp_instr[3] = SP_CMD_CONTROL;
  sp_instr[4] = sp_cmdlist_low;
  sp_instr[5] = sp_cmdlist_high;
  sp_instr[6] = 0x60;	// RTS

  // Call in emulation mode with FWEntry
  fwRec = FWEntry(0, 0, 0, (Word)sp_instr);
  //sp_count = (fwRec.yRegExit << 8) | fwRec.xRegExit;
  sp_error = fwRec.aRegExit; 
  return sp_error;                         
}

int8_t sp_open(uint8_t dest)
{
  sp_error = 0;
  // build the command list
  sp_cmdlist[0] = SP_OPEN_PARAM_COUNT;
  sp_cmdlist[1] = dest; // set before calling sp_open();

  sp_cmdlist_low = (uint8_t)((uint16_t)sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)sp_cmdlist >> 8) & 0xFF;
  sp_dispatch_low = (uint8_t)((uint16_t)sp_dispatch & 0x00FF);
  sp_dispatch_high = (uint8_t)((uint16_t)sp_dispatch >> 8) & 0xFF;

  // Make firmware call
  sp_instr[0] = 0x20;	// JSR
  sp_instr[1] = sp_dispatch_low;
  sp_instr[2] = sp_dispatch_high;
  sp_instr[3] = SP_CMD_OPEN;
  sp_instr[4] = sp_cmdlist_low;
  sp_instr[5] = sp_cmdlist_high;
  sp_instr[6] = 0x60;	// RTS

  // Call in emulation mode with FWEntry
  fwRec = FWEntry(0, 0, 0, (Word)sp_instr);
  //sp_count = (fwRec.yRegExit << 8) | fwRec.xRegExit;
  sp_error = fwRec.aRegExit;
  return sp_error;
}

int8_t sp_close(uint8_t dest)
{
  sp_error = 0;
  // build the command list
  sp_cmdlist[0] = SP_CLOSE_PARAM_COUNT;
  sp_cmdlist[1] = dest; // set before calling sp_close();

  sp_cmdlist_low = (uint8_t)((uint16_t)sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)sp_cmdlist >> 8) & 0xFF;
  sp_dispatch_low = (uint8_t)((uint16_t)sp_dispatch & 0x00FF);
  sp_dispatch_high = (uint8_t)((uint16_t)sp_dispatch >> 8) & 0xFF;

  // Make firmware call
  sp_instr[0] = 0x20;	// JSR
  sp_instr[1] = sp_dispatch_low;
  sp_instr[2] = sp_dispatch_high;
  sp_instr[3] = SP_CMD_CLOSE;
  sp_instr[4] = sp_cmdlist_low;
  sp_instr[5] = sp_cmdlist_high;
  sp_instr[6] = 0x60;	// RTS

  // Call in emulation mode with FWEntry
  fwRec = FWEntry(0, 0, 0, (Word)sp_instr);
  //sp_count = (fwRec.yRegExit << 8) | fwRec.xRegExit;
  sp_error = fwRec.aRegExit;
  return sp_error;
}

int8_t sp_read(uint8_t dest, uint16_t len)
{
  sp_error = 0;
  // build the command list
  sp_cmdlist[0] = SP_READ_PARAM_COUNT;
  sp_cmdlist[1] = dest; // set before calling sp_close();
  sp_cmdlist[2] = (uint8_t)((uint16_t)sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = len & 0xFF;
  sp_cmdlist[5] = len >> 8;
  sp_cmdlist[6] = 0;
  sp_cmdlist[7] = 0;
  sp_cmdlist[8] = 0;

  sp_cmdlist_low = (uint8_t)((uint16_t)sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)sp_cmdlist >> 8) & 0xFF;
  sp_dispatch_low = (uint8_t)((uint16_t)sp_dispatch & 0x00FF);
  sp_dispatch_high = (uint8_t)((uint16_t)sp_dispatch >> 8) & 0xFF;

  // Make firmware call
  sp_instr[0] = 0x20;	// JSR
  sp_instr[1] = sp_dispatch_low;
  sp_instr[2] = sp_dispatch_high;
  sp_instr[3] = SP_CMD_READ;
  sp_instr[4] = sp_cmdlist_low;
  sp_instr[5] = sp_cmdlist_high;
  sp_instr[6] = 0x60;	// RTS

  // Call in emulation mode with FWEntry
  fwRec = FWEntry(0, 0, 0, (Word)sp_instr);
  sp_count = (fwRec.yRegExit << 8) | fwRec.xRegExit;
  sp_error = fwRec.aRegExit;
  return sp_error;
}

int8_t sp_find_fuji(void)
{
  // const char fuji[9] = "THE_FUJI";
  const char fuji[14] = "FUJINET_DISK_0";
  const uint8_t fuji_len = sizeof(fuji);
  int8_t err, num, i, j;

  err = sp_status(0x00, 0x00); // get number of devices
	if (err)
    return -err;
  num = sp_payload[0];
	num++;
	for (i = 1; i < num; i++)
	{
    //do
      err = sp_status(i, 0x03); // get DIB
    //while (err);
    if (sp_payload[4] == fuji_len)
    {
      for (j = 0; j < fuji_len; j++)
        if (fuji[j]!=sp_payload[5+j])
          return 0;
      sp_dest = i; // store the fuji unit #
      return i;
    }
	}
  return 0;
}

int8_t sp_find_network(void)
{
  const char net[7] = "NETWORK";
  const uint8_t net_len = sizeof(net);
  int8_t err, num, i, j;

  err = sp_status(0x00, 0x00); // get number of devices
  if (err)
    return -err;
  num = sp_payload[0];
  num+=2;
  for (i = 1; i < num; i++)
    {
      err = sp_status(i, 0x03); // get DIB
      if (sp_payload[4] == net_len)
        {
          for (j = 0; j < net_len; j++)
            if (net[j]!=sp_payload[5+j])
              return 0;
          sp_net = i; // store the fuji unit #
          return i;
        }
    }
  return 0;
}

/**
 * Apple // SmartPort routines for CC65
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

/**
 * Check for SmartPort presence
 * @return slot #, or 0xFF if not present.
 */
uint8_t sp_find_slot(void)
{
  uint8_t s=0;

  for (s=7; s > 0; s--)
    {
      uint16_t a = 0xc000 + (s * 0x100);
      if ((PEEK(a+1) == 0x20) &&
          (PEEK(a+3) == 0x00) &&
          (PEEK(a+5) == 0x03) &&
          (PEEK(a+7) == 0x00))
        return s;
    }

  // Did not find.
  return 0;
}

/**
 * Return dispatch address for Smartport slot.
 * @param s Slot # (1-7)
 * @return smartport dispatch address
 */
uint16_t sp_dispatch_address(uint8_t slot)
{
  uint16_t a = (slot * 0x100) + 0xC000;
  uint8_t j = PEEK(a+0xFF);

  return a + j + 3;
}

void sp_get_buffer(void)
{
  sp_handle = NewHandle(0x500, MyID, 0xC011, 0L);
  if (sp_handle == NULL)
    SysErr();
  sp_payload = *sp_handle;
  sp_cmdlist = *sp_handle + 1024;
  sp_instr = *sp_handle + 1034;
}

void sp_init(void)
{
  uint8_t n;

  sp_get_buffer();
  sp_dispatch = sp_dispatch_address(5); // SmartPort is supposed to be on slot 5
  n = sp_find_network();
  if (n < 1)
    AlertWindow(0, NULL, (Ref)"24/Network device not found!/^#6");
}

void sp_done(void)
{
  DisposeHandle(sp_handle);
}
