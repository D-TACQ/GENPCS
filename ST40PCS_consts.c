/* ------------------------------------------------------------------------- */
/* ST40PCS_consts.c TEPCS						     */
/* ------------------------------------------------------------------------- */
/* ST40PCS_consts.c  D-TACQ ACQ400 FMC  DRIVER                                   
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
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program; if not, write to the Free Software              *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
 *
 * TODO 
 * TODO
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2015 Peter Milne, D-TACQ Solutions Ltd                    *
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
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program; if not, write to the Free Software              *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */

#include "local.h"
#include "linux_rt.h"



#ifndef ST40_MOD
#include "ST40PCS_stub.h"
#endif

#include "acq.h"
#include "sysdef.h"

#include "ST40PCS_if.h"

#define PRINT(value) printf("%20s: %d\n", #value, value)

int main()
{
	union VI_OVERLAY VI;
	union VO_OVERLAY VO;

	printf("VI structure sizes in bytes\n");
	PRINT(sizeof VI.DTACQIN);
	PRINT(sizeof VI.ACQ);

	printf("total interface size shorts\n");
	PRINT(VI_SHORTS);
	printf("AI count (shorts)\n");
	PRINT(MAX_AI_PER_BOX);
	printf("DI, STA count (u32)\n");
	PRINT(MAX_DI_PER_BOX);
	PRINT(STATUS_LEN);

	printf("Index in DTACQIN (shorts)\n");
	PRINT( MSI_AI);
	PRINT( MSI_DI);
	PRINT( MSI_STA);

/* index to the actual box should it be needed (eg for STA) */
	PRINT( MSI_PCS1_AI);
	PRINT( MSI_PCS2_AI);

	PRINT( MSI_PCS1_DI);
	PRINT( MSI_PCS2_DI);

	PRINT( MSI_PCS1_STA);
	PRINT( MSI_PCS2_STA);

	printf("\nVO structure sizes in bytes\n");
	PRINT(sizeof VO.DTACQOUT);
	PRINT(sizeof VO.ACQ);

	printf("total interface size shorts\n");
	PRINT(VO_SHORTS);
	printf("AO count (shorts)\n");
	PRINT(MAX_AO_PER_BOX);
	printf("DO count (u32)\n");
	PRINT(MAX_DO_PER_BOX);
	printf("Index in DTACQOUT (shorts)\n");
	PRINT(MSI_AO);
	PRINT(MSI_DO);
	PRINT(MSI_PCS1_AO);
	PRINT(MSI_PCS2_AO);
	PRINT(MSI_PCS1_DO);
	PRINT(MSI_PCS2_DO);
}
