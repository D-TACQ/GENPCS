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



#ifndef ST40_MOD
#include "ST40PCS_stub.h"
#endif

#include "acq.h"
#include "sysdef.h"


int main(int argc, char* argv[])
{
#ifdef ST40_MOD
#include "ST40PCS.h" // don't understand why this has to be in the braces, but it seems to
ST40PCS_U;
ST40PCS_Y;
#endif
#include "ST40PCS_if.h"
	int sample;
	int ii;
	ACQ* acq0;
	ACQ* acq1;
	union VI_OVERLAY* VI = (union VI_OVERLAY*)ST40PCS_U.DTACQIN;
	union VO_OVERLAY* VO = (union VO_OVERLAY*)ST40PCS_Y.DTACQOUT;

	if (sizeof(ST40PCS_U.DTACQIN) != sizeof(union VI_OVERLAY)){
		fprintf(stderr, "ERROR: mismatch %s %d != %d\n",
				"VI", sizeof(ST40PCS_U.DTACQIN), sizeof(union VI_OVERLAY));
		return 1;
	}
	if (sizeof(ST40PCS_Y.DTACQOUT) != sizeof(union VO_OVERLAY)){
		fprintf(stderr, "ERROR: mismatch %s %d != %d\n",
				"VO", sizeof(ST40PCS_Y.DTACQOUT), sizeof(union VO_OVERLAY));
		return 1;
	}

	linux_rt_init(argc, argv);

	dbg(1, "file %s flavour %s", __FILE__, FLAVOUR);

	acq0 = acq_init(LUN_MAG);
	acq1 = acq_init(LUN_PSU);
	ST40PCS_initialize();
	goRealTime();

	for (sample = 0; sample < N_iter; ++sample){
		pmemcpy(acq0->AO, VO->ACQ.AO0+MSI_PCS1_AO, LUN0_AO*SS);
		pmemcpy(acq0->DO, VO->ACQ.DO0+MSI_PCS1_DO, LUN0_DO*US);
		pmemcpy(acq1->AO, VO->ACQ.AO0+MSI_PCS2_AO, LUN1_AO*SS);
		pmemcpy(acq1->DO, VO->ACQ.DO1+MSI_PCS2_DO, LUN1_DO*US);

		acq_IO(acq0);	/* blocks */
		acq_IO(acq1);	/* should come right back.. */

		memcpy(VI->ACQ.AI0, acq0->lbuf+ASI_LUN0_AI, LUN0_AI*SS);
		memcpy(VI->ACQ.DI0, acq0->lbuf+ASI_LUN0_DI, LUN0_DI*US);
		memcpy(VI->ACQ.ST0, acq0->lbuf+ASI_LUN0_ST, LUN0_ST*US);
		memcpy(VI->ACQ.AI0, acq1->lbuf+ASI_LUN1_AI, LUN1_AI*SS);
		memcpy(VI->ACQ.DI1, acq1->lbuf+ASI_LUN1_DI, LUN1_DI*US);
		memcpy(VI->ACQ.ST1, acq1->lbuf+ASI_LUN1_ST, LUN1_ST*US);

		ST40PCS_step();

		if(verbose > 3 && sample<20) {		/* do not print in RT mode */
			for (ii=0; ii<34; ii+=1) {
				printf("Sample %d  II %d IN %d OUT %d\n",sample,ii,ST40PCS_U.DTACQIN[ii],ST40PCS_Y.DTACQOUT[ii]);
			}
		}
	}

	ST40PCS_terminate();
	acq_terminate(acq0);
	acq_terminate(acq1);
	return 0;
}
