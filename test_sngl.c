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
	int sample;
	int ii;
	ACQ* acq0;

	linux_rt_init(argc, argv);

	dbg(1, "file %s flavour %s", __FILE__, FLAVOUR);

	acq0 = acq_init(G_lun);
	ST40PCS_initialize();

	if (verbose > 2){
		FILE *fp = popen("hexdump", "w");
		printf("initial OUT values\n");
		fwrite(ST40PCS_Y.DTACQOUT, sizeof(ST40PCS_Y.DTACQOUT), 1, fp);
		pclose(fp);
	}
	dbg(1, "Starting");
	goRealTime();

	for (sample = 0; sample < N_iter; ++sample){
		memcpy(acq0->AO, ST40PCS_Y.DTACQOUT, LUN0_AO*SS);
		memcpy(acq0->DO, ST40PCS_Y.DTACQOUT+LUN0_AO, LUN0_DO*US);

		acq_IO(acq0);

		memcpy(ST40PCS_U.DTACQIN, acq0->AI, LUN0_AI*SS);
		memcpy(ST40PCS_U.DTACQIN+128, acq0->DI, LUN0_DI*US);
		/* ST40PCS_U.?? = acq0->sample_count); */
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
