/* ------------------------------------------------------------------------- */
/* sysdef.h  GENPCS support
 * Project: AFHBA404
 * Created: 20 Jun 2016  			/ User: pgm
 * ------------------------------------------------------------------------- *
 *   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd         *
 *                      <peter dot milne at D hyphen TACQ dot com>           *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * TODO 
 * TODO
\* ------------------------------------------------------------------------- */

#ifndef GEN_SYSDEF_H_
#define GEN_SYSDEF_H_

/* System Definitions */

#if defined(GEN_ACQ) && defined(GEN_MOD)
	#define FLAVOUR "ACQ_MOD"
#else
	#if defined(GEN_ACQ)
		#define FLAVOUR "ACQ"
	#elif defined (GEN_MOD)
		#define FLAVOUR "MOD"
	#else
		#define FLAVOUR "FAIL"
	#endif
#endif

/* LUN : Logical unit numbers of the 3 boxes /dev/afhba.$LUN.* */
#define LUN_ALPHA	0
#define LUN_BRAVO	1
#define LUN_CHARLIE	2

/* both boxes fake full complement of IO, so are the same */

#define LUN0_AI	160
#define LUN0_DI 1
#define LUN0_AO 0
#define LUN0_DO 1
#define LUN0_ST 4
#define LUN0_CALC 32

/** ASI : ACQ Short Index .. index in shorts */
#define ASI_LUN0_AI	0
#define ASI_LUN0_DI	LUN0_AI
#define ASI_LUN0_ST	(ASI_LUN0_DI+2)

#define LUN1_AI	128
#define LUN1_DI 1
#define LUN1_AO 32
#define LUN1_DO 1
#define LUN1_ST 4
#define LUN1_CALC 0

#define ASI_LUN1_AI	0
#define ASI_LUN1_DI	LUN1_AI
#define ASI_LUN1_ST	(ASI_LUN1_DI+2)

/* MODEL definition : MUST MATCH GENPCS.h */

#define MODEL_AI 256	/* shorts */
#define MODEL_DI 2	/* u32 */
#define MODEL_ST 16     /* u32 */

#define MODEL_AO 64	/* shorts */
#define MODEL_DO 2      /* u32 */

#define MODEL_VI_LEN	(MODEL_AI*SS + MODEL_DI*US + MODEL_ST*US)
#define MODEL_VO_LEN	(MODEL_AO*SS + MODEL_DO*US)

#endif /* GEN_SYSDEF_H_ */
