/* ------------------------------------------------------------------------- */
/* file ST40PCS_stub.c                                                                 */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    http://www.d-tacq.com

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                 */
/* ------------------------------------------------------------------------- */

/** @file ST40PCS_stub.c  interface for MODEL
 * Refs:
*/

#include "local.h"
#include "ST40PCS_stub.h"
#include "sysdef.h"

void ST40PCS_initialize(void)
{
	dbg(2, "file %s flavour %s", __FILE__, FLAVOUR);
}

/** fake "feedback algorithm" */
void ST40PCS_step(void)
{
	ST40PCS_Y.DTACQOUT[0] = ST40PCS_U.DTACQIN[0];
	ST40PCS_Y.DTACQOUT[1] = -ST40PCS_U.DTACQIN[0];
}
void ST40PCS_terminate(void)
{

}
