/* ------------------------------------------------------------------------- */
/* ST40PCS_if.h  D-TACQ ACQ400 FMC  DRIVER                                   
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
\* ------------------------------------------------------------------------- */

#ifndef ST40PCS_IF_H_
#define ST40PCS_IF_H_

#include <stddef.h>

#ifdef ST40_MOD
#define u32 uint32_T
// #define short uint16_T  // could be problematic..
#endif

/* MODel works in shorts .. compute offsets in shorts */

#define OFFSETS(type, member) (offsetof(type, member)/sizeof(int16_T))

/* this is the interface between ACQ and model */

/* the interface works on maximum payload values */
#define MAX_AI_PER_BOX	(5*32)		// channels
#define MAX_DI_PER_BOX  (4)		// 32 bit values
#define STATUS_LEN	(12)

/* VI is a composite of both boxes */
#define VI_SHORTS	(2*MAX_AI_PER_BOX + (2*MAX_DI_PER_BOX+2*STATUS_LEN)*2)


/** define an overlay for the MODEL and ACQ definitions of the
 * Input Vector VI
 */
union VI_OVERLAY {
	int16_T DTACQIN[VI_SHORTS]; 	// Simulink uses this
	struct VI_ACQ {			// ACQ uses this.
		short AI0[MAX_AI_PER_BOX];
		short AI1[MAX_AI_PER_BOX];
		u32 DI0[MAX_DI_PER_BOX];
		u32 DI1[MAX_DI_PER_BOX];
		u32 ST0[STATUS_LEN];
		u32 ST1[STATUS_LEN];
	} ACQ;
};

/**
 *  MODEL works in shorts .. so generate all model indices in shorts
 * MSI_ : MODEL SHORT INDEX
 */

#define MSI_AI		OFFSETS(struct VI_ACQ, AI0)
#define MSI_DI		OFFSETS(struct VI_ACQ, DI0)
#define MSI_ST 		OFFSETS(struct VI_ACQ, ST0)

/* index to the actual box should it be needed (eg for STA) */
#define MSI_PCS1_AI	OFFSETS(struct VI_ACQ, AI0)
#define MSI_PCS2_AI	OFFSETS(struct VI_ACQ, AI1)

#define MSI_PCS1_DI	OFFSETS(struct VI_ACQ, DI0)
#define MSI_PCS2_DI	OFFSETS(struct VI_ACQ, DI1)

#define MSI_PCS1_ST	OFFSETS(struct VI_ACQ, ST0)
#define MSI_PCS2_ST	OFFSETS(struct VI_ACQ, ST1)

#define MAX_AO_PER_BOX	(1*32)		// channels
#define MAX_DO_PER_BOX  (4)		// 32 bit values

#define MAX_CALC	(32)		// number of CALC values (4 byte, could be unsigned or float)

#define VO_SHORTS	(2*MAX_AO_PER_BOX + 2*MAX_DO_PER_BOX + MAX_CALC)

/** define an overlay for the MODEL and ACQ definitions of the
 * Output Vector VO
 */
union VO_OVERLAY {
	int16_T DTACQOUT[VO_SHORTS];	// Simulink uses this
	struct VO_ACQ {
		short AO0[0];		     // NO AO in BOX1
		short AO1[MAX_AO_PER_BOX];
		short AO2[MAX_AO_PER_BOX];   // expansion possible in BOX2
		u32 DO0[MAX_DO_PER_BOX];
		u32 DO1[MAX_DO_PER_BOX];
		u32 CALC[MAX_CALC];
	} ACQ;
};

#define MSI_AO		OFFSETS(struct VO_ACQ, AO0)
#define MSI_DO		OFFSETS(struct VO_ACQ, DO0)

#define MSI_PCS1_AO	OFFSETS(struct VO_ACQ, AO0)
#define MSI_PCS2_AO	OFFSETS(struct VO_ACQ, AO1)

#define MSI_PCS1_DO	OFFSETS(struct VO_ACQ, DO0)
#define MSI_PCS2_DO	OFFSETS(struct VO_ACQ, DO1)


#endif /* ST40PCS_IF_H_ */
