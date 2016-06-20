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

#ifdef ST40_MOD
#include "ST40PCS.h"
/* @@todo pgm .. don't understand how next two lines compile ML secret sauce? */
ST40PCS_U;
ST40PCS_Y;
#else

#include "ST40PCS_stub.h"
#include "acq.h"

ExtU_ST40PCS_T ST40PCS_U;
ExtY_ST40PCS_T ST40PCS_Y;
#endif

#include "sysdef.h"


int main(int argc, char* argv[])
{
	int sample;
	ACQ* acq0;
	ACQ* acq1;

	linux_rt_init(argc, argv);

	dbg(1, "file %s flavour %s", __FILE__, FLAVOUR);

	acq0 = acq_init(LUN_MAG);
	acq1 = acq_init(LUN_PSU);
	ST40PCS_initialize();

	for (sample = 0; sample < N_iter; ++sample){
		int ix = 0;
		memcpy(acq0->AO, ST40PCS_Y.DTACQOUT+ix,	LUN0_AO*SS); 	ix += LUN0_AO;
		memcpy(acq0->DO, ST40PCS_Y.DTACQOUT+ix, LUN0_DO*US); 	ix += LUN0_DO*US/SS;
		memcpy(acq1->AO, ST40PCS_Y.DTACQOUT+ix, LUN0_AO*SS);	ix += LUN1_AO;
		memcpy(acq1->DO, ST40PCS_Y.DTACQOUT+ix, LUN0_DO*SS);

		acq_IO(acq0);	/* blocks */
		acq_IO(acq1);	/* should come right back.. */

		ix = 0;
		memcpy(ST40PCS_U.DTACQIN+ix, acq0->AI, LUN0_AI*SS);	ix += LUN0_AI;
		memcpy(ST40PCS_U.DTACQIN+ix, acq0->DI, LUN0_DI*US);	ix += LUN0_DI*US/SS;
		/* ST40PCS_U.?? = acq0->sample_count); */

		memcpy(ST40PCS_U.DTACQIN+ix, acq1->AI, 128*SS);		ix += LUN1_AI;
		memcpy(ST40PCS_U.DTACQIN+ix, acq1->AI, 128*SS);
		/* ST40PCS_U.?? = acq1->sample_count); */

		ST40PCS_step();
	}

	ST40PCS_terminate();
	acq_terminate(acq0);
	acq_terminate(acq1);
	return 0;
}
