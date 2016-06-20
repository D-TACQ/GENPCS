/* ------------------------------------------------------------------------- */
/* acq.c  D-TACQ ACQ400 LLC device shim body
 * Project: AFHBA404
 * Created: 20 Jun 2016  			/ User: pgm
 * ------------------------------------------------------------------------- *
 *   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd         *
 *                      <peter dot milne at D hyphen TACQ dot com>           *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * TODO
 * TODO
/* ------------------------------------------------------------------------- */

#include "local.h"
#include "acq.h"

/** @todo pgm would far prefer C++, but not sure SL compat. */

ACQ* acq_init(int lun)
{
	int ii;
	ACQ* acq = calloc(1, sizeof(ACQ));

	acq->AI = calloc(128, SS);
	acq->DI = calloc(1, 2*SS);
	acq->AO = calloc(32, SS);
	acq->DO = calloc(1, 2*SS);

	for (ii=0; ii<10; ii+=1){
		acq->AI[ii]=125*ii;
	}
	acq->DI[0] = (-3<<16) | 170;
	return acq;
}
void acq_IO(ACQ* acq)
{

}
void acq_terminate(ACQ* acq)
{
	/* free the memory. Actually, Linux can do this .. */
}
