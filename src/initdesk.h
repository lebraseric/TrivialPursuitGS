/* ************************************************************************* */
/*                                                                           */
/* INITDESK.H - Definitions externes du module INITDESK                      */
/*                                                                           */
/* Version 0.2    13/11/89                                                   */
/*                                                                           */
/* ************************************************************************* */

#ifndef INITDESK_H
#define INITDESK_H

#include <types.h>

extern Word MyID;

Word InitTools(Word mode);
void SysErr(void);

#endif /* INITDESK_H */
