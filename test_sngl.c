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

#ifdef ST40_MOD
#include "ST40PCS.h"
/* @@todo pgm .. don't understand how next two lines compile ML secret sauce? */
ST40PCS_U;
ST40PCS_Y;
#else
#include "ST40PCS_stub.h"
ExtU_ST40PCS_T ST40PCS_U;
ExtY_ST40PCS_T ST40PCS_Y;
#endif

#include "acq.h"
#include "sysdef.h"


int main(int argc, char* argv[])
{
	int sample;
	ACQ* acq0;

	linux_rt_init(argc, argv);

	dbg(1, "file %s flavour %s", __FILE__, FLAVOUR);

	acq0 = acq_init(0);
	ST40PCS_initialize();
	goRealTime();

	for (sample = 0; sample < N_iter; ++sample){
		memcpy(acq0->AO, ST40PCS_Y.DTACQOUT, LUN0_AO*SS);
		memcpy(acq0->DO, ST40PCS_Y.DTACQOUT+LUN0_AO, LUN0_DO*US);

		acq_IO(acq0);

		memcpy(ST40PCS_U.DTACQIN, acq0->AI, LUN0_AI*SS);
		memcpy(ST40PCS_U.DTACQIN+128, acq0->DI, LUN0_DI*US);
		/* ST40PCS_U.?? = acq0->sample_count); */
		ST40PCS_step();
	}

	ST40PCS_terminate();
	acq_terminate(acq0);
	return 0;
}
