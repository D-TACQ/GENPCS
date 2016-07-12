/* ------------------------------------------------------------------------- */
/* file test_sngl.c                                                                 */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    http://www.d-tacq.com

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                 */
/* ------------------------------------------------------------------------- */

/** @file test_sngl.c  interface for MODEL
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
ST40PCS_U;		// this statement seems redundant, but if it ain't broke ..
ST40PCS_Y;		// ditto
#endif
#include "ST40PCS_if.h"
	int sample;
	int ii;
	ACQ* acq0;
	union VI_OVERLAY* VI = (union VI_OVERLAY*)ST40PCS_U.DTACQIN;
	union VO_OVERLAY* VO = (union VO_OVERLAY*)ST40PCS_Y.DTACQOUT;

	if (sizeof(ST40PCS_U.DTACQIN) != sizeof(union VI_OVERLAY)){
		fprintf(stderr, "ERROR: mismatch %s %lu != %lu\n",
				"VI", sizeof(ST40PCS_U.DTACQIN), sizeof(union VI_OVERLAY));
		return 1;
	}
	if (sizeof(ST40PCS_Y.DTACQOUT) != sizeof(union VO_OVERLAY)){
		fprintf(stderr, "ERROR: mismatch %s %lu != %lu\n",
				"VO", sizeof(ST40PCS_Y.DTACQOUT), sizeof(union VO_OVERLAY));
		return 1;
	}

	linux_rt_init(argc, argv);

	dbg(1, "file %s flavour %s %s", __FILE__, FLAVOUR, MEMCPY);

	acq0 = acq_init(G_lun);
	ST40PCS_initialize();

	dbg(1, "Starting");
	goRealTime();

	for (sample = 0; sample < N_iter; ++sample){
		pmemcpy(acq0->AO, VO->ACQ.AO0, LUN0_AO*SS);
		pmemcpy(acq0->DO, VO->ACQ.DO0, LUN0_DO*US);

		acq_IO(acq0);

		memcpy(VI->ACQ.AI0, acq0->lbuf+ASI_LUN0_AI, LUN0_AI*SS);
		memcpy(VI->ACQ.DI0, acq0->lbuf+ASI_LUN0_DI, LUN0_DI*US);
		memcpy(VI->ACQ.ST0, acq0->lbuf+ASI_LUN0_ST, LUN0_ST*US);

		ST40PCS_step();

                if(verbose > 4 && sample<20) {		/* do not print in RT mode */
			for (ii=0; ii<34; ii+=1) {
				printf("Sample %d  II %d IN %d OUT %d\n",sample,ii,ST40PCS_U.DTACQIN[ii],ST40PCS_Y.DTACQOUT[ii]);
			}
		}               
	}

	ST40PCS_terminate();
	dbg(1, "Terminated");
	acq_terminate(acq0);
	return 0;
}
