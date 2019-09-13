/* ------------------------------------------------------------------------- */
/* file GENPCS_stub.c                                                                 */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    http://www.d-tacq.com

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                 */
/* ------------------------------------------------------------------------- */

/** @file GENPCS_stub.c  interface for MODEL
 * Refs:
*/

#include "local.h"
#include "GENPCS_stub.h"
#include "sysdef.h"

#include "GENPCS_if.h"

ExtU_GENPCS_T GENPCS_U;
ExtY_GENPCS_T GENPCS_Y;

void GENPCS_initialize(void)
{
	int ic;

	dbg(2, "file %s flavour %s", __FILE__, FLAVOUR);

	for (ic = 0; ic < 32; ++ic){
		GENPCS_Y.DTACQOUT[ic] = 0x1000 + (ic<<4);
	}
	GENPCS_Y.DTACQOUT[32] = 0x0bad;
	GENPCS_Y.DTACQOUT[33] = 0x1dea;
}

static int iter;

void _GENPCS_step(short *AI, short* AO, short* DO, unsigned id)
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
		(iter>>2&1? id : 0)|
		(AI[0]>0? 1<<2: 0) |	//mod(AI0)
		(AI[0]>0? 1<<1: 0) |	//mod(AI0)
		(iter>>1&1? 1<<0:0);	 		//toggles at SR
}
/** fake "feedback algorithm"
 * Example doubles up to provide SAME function both boxes.
 * we don't know if both boxes are present (and we don't care)..
 */
void GENPCS_step(void)
{
	_GENPCS_step(
		GENPCS_U.DTACQIN  + MSI_PCS1_AI,
		GENPCS_Y.DTACQOUT + MSI_PCS1_AO,
		GENPCS_Y.DTACQOUT + MSI_PCS1_DO, 0xa0);

	_GENPCS_step(
		GENPCS_U.DTACQIN  + MSI_PCS1_AI,  	/* pick up AI01 */
		GENPCS_Y.DTACQOUT + MSI_PCS2_AO,
		GENPCS_Y.DTACQOUT + MSI_PCS2_DO+1, 0xb0); /* force hi word */

	++iter;
}
void GENPCS_terminate(void)
{

}
