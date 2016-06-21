/* ------------------------------------------------------------------------- */
/* pig.c AFHBA404						     */
/* ------------------------------------------------------------------------- */
/* pig.c  D-TACQ ACQ400 FMC  DRIVER                                   
 * Project: AFHBA404
 * Created: 20 Jun 2016  			/ User: pgm
 * ------------------------------------------------------------------------- *
 *   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd         *
 *                      <peter dot milne at D hyphen TACQ dot com>           *
 *                                                                           *
 *  This program is free software; you can redistribute it and/or modify     *
 *  it under the terms of Version 2 of the GNU General Public License        *
 *  as published by the Free Software Foundation;                            *
 *                                                                           *
/* ------------------------------------------------------------------------- */

#include "local.h"
#include "linux_rt.h"

int main(int argc, char* argv[])
{
	linux_rt_init(argc, argv);

	while(1){
		;
	}
}
