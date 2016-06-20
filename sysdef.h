/* ------------------------------------------------------------------------- */
/* sysdef.h  TEPCS support
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
/* ------------------------------------------------------------------------- */

#ifndef TE_SYSDEF_H_
#define TE_SYSDEF_H_

/* System Definitions */

#if defined(ST40_ACQ) && defined(ST40_MOD)
	#define FLAVOUR "Thames"
#else
	#if defined(ST40_ACQ)
		#define FLAVOUR "Clyde"
	#elif defined (ST40_MOD)
		#define FLAVOUR "Rhone"
	#else
		#define FLAVOUR "Limpopo"
	#endif
#endif


#define LUN_MAG	0
#define LUN_PSU 1

/* both boxes fake full complement of IO, so are the same */

#define LUN0_AI	128
#define LUN0_DI 1
#define LUN0_AO 32
#define LUN0_DO 1

#define LUN1_AI	128
#define LUN1_DI 1
#define LUN1_AO 32
#define LUN1_DO 1



#endif /* TE_SYSDEF_H_ */