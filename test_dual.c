/* ------------------------------------------------------------------------- */
/* file test_dual.c                                                                 */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    http://www.d-tacq.com

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                 */
/* ------------------------------------------------------------------------- */

/** @file test_dual.c  interface for MODEL
 * Refs:
*/

#include "local.h"
#include "linux_rt.h"



#ifndef GEN_MOD
#include "GENPCS_stub.h"
#endif

#include "acq.h"
#include "sysdef.h"

/*
 * MODEL buffers are contiguous for all AI, DI, AO, DO
 * ACQ buffers are split .. because there are more than one ACQ modules
 *
 * SCATTER XO from MODEL to ACQ :
 *     split contiguous MODEL XO to component ACQ XO
 *
 *  GATHER XI from ACQ to MODEL :
 *     join component ACQ XI to MODEL XO
 */

int main(int argc, char* argv[])
{
#include "GENPCS_if.h"
	int sample;
	ACQ* acq0;
	ACQ* acq1;
	union VI_OVERLAY* VI = (union VI_OVERLAY*)GENPCS_U.DTACQIN;
	union VO_OVERLAY* VO = (union VO_OVERLAY*)GENPCS_Y.DTACQOUT;

	if (sizeof(GENPCS_U.DTACQIN) != sizeof(union VI_OVERLAY)){
		fprintf(stderr, "ERROR: mismatch %s %lu != %lu\n",
				"VI", sizeof(GENPCS_U.DTACQIN), sizeof(union VI_OVERLAY));
		return 1;
	}
	if (sizeof(GENPCS_Y.DTACQOUT) != sizeof(union VO_OVERLAY)){
		fprintf(stderr, "ERROR: mismatch %s %lu != %lu\n",
				"VO", sizeof(GENPCS_Y.DTACQOUT), sizeof(union VO_OVERLAY));
		return 1;
	}

	linux_rt_init(argc, argv);

	dbg(1, "file %s flavour %s", __FILE__, FLAVOUR);

	acq0 = acq_CREATE(LUN_ALPHA);
	acq1 = acq_CREATE(LUN_BRAVO);
	GENPCS_initialize();
	goRealTime();

	for (sample = 0; sample < N_iter; ++sample){
		/* scatter XO from model to hardware */
		memcpy(acq0->AO, VO->ACQ.AO0, LUN0_AO*SS);
		memcpy(acq0->DO, VO->ACQ.DO0, LUN0_DO*US);
		memcpy(acq1->AO, VO->ACQ.AO1, LUN1_AO*SS);
		memcpy(acq1->DO, VO->ACQ.DO1, LUN1_DO*US);
		memcpy(acq0->CALC, VO->ACQ.CALC, MAX_CALC*US);
		log_XO(acq0);
		log_XO(acq1);

		acq_IO(acq0);	/* blocks */
		acq_IO(acq1);	/* should come right back.. */

		/* gather XI from hardware to model */
		memcpy(VI->ACQ.AI0, acq0->lbuf+ASI_LUN0_AI, LUN0_AI*SS);
		memcpy(VI->ACQ.DI0, acq0->lbuf+ASI_LUN0_DI, LUN0_DI*US);
		memcpy(VI->ACQ.ST0, acq0->lbuf+ASI_LUN0_ST, LUN0_ST*US);
		memcpy(VI->ACQ.AI1, acq1->lbuf+ASI_LUN1_AI, LUN1_AI*SS);
		memcpy(VI->ACQ.DI1, acq1->lbuf+ASI_LUN1_DI, LUN1_DI*US);
		memcpy(VI->ACQ.ST1, acq1->lbuf+ASI_LUN1_ST, LUN1_ST*US);

		GENPCS_step();
	}

	GENPCS_terminate();
	acq_terminate(acq0);
	acq_terminate(acq1);
	return 0;
}
