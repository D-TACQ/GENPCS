/*
 * test_multi.c
 *
 *  Created on: 13 Sep 2019
 *      Author: pgm
 */


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


#include "GENPCS_if.h"

void validate_interface(void)
{
	if (sizeof(GENPCS_U.DTACQIN) != sizeof(union VI_OVERLAY)){
		fprintf(stderr, "ERROR: mismatch %s %lu != %lu\n",
				"VI", sizeof(GENPCS_U.DTACQIN), sizeof(union VI_OVERLAY));
		exit(1);
	}
	if (sizeof(GENPCS_Y.DTACQOUT) != sizeof(union VO_OVERLAY)){
		fprintf(stderr, "ERROR: mismatch %s %lu != %lu\n",
				"VO", sizeof(GENPCS_Y.DTACQOUT), sizeof(union VO_OVERLAY));
		exit(1);
	}
}


#define MAXTEAM		4

struct memcpy_def {
	void* to;
	void* from;
	int len;
};

struct memcpy_def scatter_defs[MAXTEAM*3];
struct memcpy_def gather_defs[MAXTEAM*3];
int iscatter = 0, igather = 0;

int iteam;
ACQ* acq_team[4];


int make_scatter_defs(ACQ *acq, int cursor, union VO_OVERLAY* VO)
{
	struct memcpy_def AO = { acq->AO, VO->ACQ.AO0, 32*SS };
	struct memcpy_def DO = { acq->DO, VO->ACQ.DO0, 1*US  };
	struct memcpy_def CA = { acq->CALC, VO->ACQ.CALC, 32*US };

	scatter_defs[cursor++] = AO;
	scatter_defs[cursor++] = DO;
	scatter_defs[cursor++] = CA;
	return cursor;
}


int make_gather_defs(ACQ *acq, int cursor, union VI_OVERLAY* VI)
{
	struct memcpy_def AI = { VI->ACQ.AI0, acq->lbuf+ASI_LUN0_AI, LUN0_AI*SS };
	struct memcpy_def DI = { VI->ACQ.DI0, acq->lbuf+ASI_LUN0_DI, LUN0_DI*US  };
	struct memcpy_def ST = { VI->ACQ.ST0, acq->lbuf+ASI_LUN0_ST, LUN0_ST*US };

	gather_defs[cursor++] = AI;
	gather_defs[cursor++] = DI;
	gather_defs[cursor++] = ST;
	return cursor;
}
void ui(int argc, char* argv[], union VI_OVERLAY* VI, union VO_OVERLAY* VO)
{
	for (int ii = 1; ii < argc; ++ii, ++iteam){
		ACQ* acq = acq_init(atoi(argv[ii]));

		iscatter = make_scatter_defs(acq, iscatter, VO);
		igather = make_gather_defs(acq, igather, VI);
		acq_team[iteam] = acq;
	}
	dbg(1, "file %s flavour %s #uuts: %d", __FILE__, FLAVOUR, iteam);
}

void memcpy_group(struct memcpy_def* defs, int ndefs)
{
	for (; ndefs; --ndefs, ++defs){
		memcpy(defs->to, defs->from, defs->len);
	}
}


#define FOREACHUUT(action)  for (int ii = 0; ii < iteam; ++ii) action(acq_team[ii])

int main(int argc, char* argv[])
{
	union VI_OVERLAY* VI = (union VI_OVERLAY*)GENPCS_U.DTACQIN;
	union VO_OVERLAY* VO = (union VO_OVERLAY*)GENPCS_Y.DTACQOUT;

	linux_rt_init(argc, argv);
	ui(argc, argv, VI, VO);

	GENPCS_initialize();
	goRealTime();

	for (int sample = 0; sample < N_iter; ++sample){
		memcpy_group(scatter_defs, iscatter);	/* scatter XO from model to hardware 	*/
		FOREACHUUT(log_XO);			/* XO logging (could be null)		*/

		FOREACHUUT(acq_IO);			/* DAQ happens here, UUT0 blocks, the rest should already be ready */
		memcpy_group(gather_defs, igather);	/* gather XI from hardware to model */

		GENPCS_step();				/* run the model, data in globals GENPCS_U,GENPCS_Y */
	}

	GENPCS_terminate();
	FOREACHUUT(acq_terminate);
	return 0;
}
