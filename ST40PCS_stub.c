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
	int ic;
	ST40PCS_Y.DTACQOUT[0] = ST40PCS_U.DTACQIN[0];
	ST40PCS_Y.DTACQOUT[1] = -ST40PCS_U.DTACQIN[0];

	ST40PCS_Y.DTACQOUT[2] = 20*iter;
	ST40PCS_Y.DTACQOUT[3] = -20*iter;

	for (ic = 4; ic < 32; ++ic){
		ST40PCS_Y.DTACQOUT[ic] = 10*iter + ic*500;
	}

	/* 16 channels DO looped back to 16 channels DI
	 * DO.0 will go high when AI0 > 0
	 */
	ST40PCS_Y.DTACQOUT[32] = iter*4 | 		//counter
		((iter&1)? 0x2:0) | 			//toggles at SR
		((ST40PCS_U.DTACQIN[0]>0? 1: 0));	//mod(AI0)

	++iter;
}
void ST40PCS_terminate(void)
{

}
