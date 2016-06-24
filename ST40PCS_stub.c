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

ExtU_ST40PCS_T ST40PCS_U;
ExtY_ST40PCS_T ST40PCS_Y;

void ST40PCS_initialize(void)
{
	int ic;

	dbg(2, "file %s flavour %s", __FILE__, FLAVOUR);

	for (ic = 0; ic < 32; ++ic){
		ST40PCS_Y.DTACQOUT[ic] = 0x1000 + (ic<<4);
	}
	ST40PCS_Y.DTACQOUT[32] = 0x0bad;
	ST40PCS_Y.DTACQOUT[33] = 0x1dea;
}

static int iter;

/** fake "feedback algorithm" */
void ST40PCS_step(void)
{
	ST40PCS_Y.DTACQOUT[0] = ST40PCS_U.DTACQIN[0];
	ST40PCS_Y.DTACQOUT[1] = -ST40PCS_U.DTACQIN[0];

	ST40PCS_Y.DTACQOUT[2] = 10*iter*10;
	ST40PCS_Y.DTACQOUT[3] = 10*iter*20;
	ST40PCS_Y.DTACQOUT[4] = 10*iter*40;
	ST40PCS_Y.DTACQOUT[5] = 10*iter*80;
	/* DIO pattern not sure which short will map to high byte
	 * guessing, the first one, but this will make it really obvious.
	 */
	ST40PCS_Y.DTACQOUT[32] = iter;
	ST40PCS_Y.DTACQOUT[33] = -iter;
	++iter;
}
void ST40PCS_terminate(void)
{

}
