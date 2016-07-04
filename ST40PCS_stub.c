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

#include "ST40PCS_if.h"

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

void _ST40PCS_step(short *AI, short* AO, short* DO)
{
	int ic;
	AO[0] = AI[0];
	AO[1] = -AI[0];

	AO[2] = 20*iter;
	AO[3] = -20*iter;

	for (ic = 4; ic < 32; ++ic){
		AO[ic] = 10*iter + ic*500;
	}

	/* 16 channels DO looped back to 16 channels DI */

	DO[0] = iter<<8 | 		//counter
		(ST40PCS_U.DTACQIN[0]>0? 1<<1: 0) |	//mod(AI0)
		(iter>>1&1? 1<<0:0);	 		//toggles at SR

	++iter;
}
/** fake "feedback algorithm"
 * Example doubles up to provide SAME function both boxes.
 * we don't know if both boxes are present (and we don't care)..
 */
void ST40PCS_step(void)
{
	_ST40PCS_step(
		ST40PCS_U.DTACQIN  + MSI_PCS1_AI,
		ST40PCS_Y.DTACQOUT + MSI_PCS1_AO,
		ST40PCS_Y.DTACQOUT + MSI_PCS1_DO);

	_ST40PCS_step(
		ST40PCS_U.DTACQIN  + MSI_PCS2_AI,
		ST40PCS_Y.DTACQOUT + MSI_PCS2_AO,
		ST40PCS_Y.DTACQOUT + MSI_PCS2_DO);

	++iter;
}
void ST40PCS_terminate(void)
{

}
