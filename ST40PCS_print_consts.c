/* ------------------------------------------------------------------------- */
/* ST40PCS_print_consts.c TEPCS						     */
/* ------------------------------------------------------------------------- */
/* ST40PCS_print_consts.c  D-TACQ ACQ400 FMC  DRIVER
 * Project: TEPCS
 * Created: 4 Jul 2016  			/ User: pgm
 * ------------------------------------------------------------------------- *
 *   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd         *
 *                      <peter dot milne at D hyphen TACQ dot com>           *
 *                                                                           *
 *  This program is free software; you can redistribute it and/or modify     *
 *  it under the terms of Version 2 of the GNU General Public License        *
 *  as published by the Free Software Foundation;                            *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
\* ------------------------------------------------------------------------- */

#include "local.h"
#include "linux_rt.h"



#ifndef ST40_MOD
#include "ST40PCS_stub.h"
#endif

#include "acq.h"
#include "sysdef.h"

#include "ST40PCS_if.h"

#define PRINTLU(value) printf("%20s: %lu\n", #value, value)
#define PRINTI(value) printf("%20s: %d\n", #value, value)

int main()
{
	union VI_OVERLAY VI;
	union VO_OVERLAY VO;

	printf("VI structure sizes in bytes\n");
	PRINTLU(sizeof VI.DTACQIN);
	PRINTLU(sizeof VI.ACQ);

	printf("total interface size shorts\n");
	PRINTI(VI_SHORTS);
	printf("AI count (shorts)\n");
	PRINTI(MAX_AI_PER_BOX);
	printf("DI, STA count (u32)\n");
	PRINTI(MAX_DI_PER_BOX);
	PRINTI(STATUS_LEN);

	printf("Index in DTACQIN (shorts)\n");
	PRINTLU( MSI_AI);
	PRINTLU( MSI_DI);
	PRINTLU( MSI_ST);

/* index to the actual box should it be needed (eg for STA) */
	PRINTLU( MSI_PCS1_AI);
	PRINTLU( MSI_PCS2_AI);

	PRINTLU( MSI_PCS1_DI);
	PRINTLU( MSI_PCS2_DI);

	PRINTLU( MSI_PCS1_ST);
	PRINTLU( MSI_PCS2_ST);

	printf("\nVO structure sizes in bytes\n");
	PRINTLU(sizeof VO.DTACQOUT);
	PRINTLU(sizeof VO.ACQ);

	printf("total interface size shorts\n");
	PRINTI(VO_SHORTS);
	printf("AO count (shorts)\n");
	PRINTI(MAX_AO_PER_BOX);
	printf("DO count (u32)\n");
	PRINTI(MAX_DO_PER_BOX);
	printf("Index in DTACQOUT (shorts)\n");
	PRINTLU(MSI_AO);
	PRINTLU(MSI_DO);
	PRINTLU(MSI_PCS1_AO);
	PRINTLU(MSI_PCS2_AO);
	PRINTLU(MSI_PCS1_DO);
	PRINTLU(MSI_PCS2_DO);
}
