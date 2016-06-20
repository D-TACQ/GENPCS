/* ------------------------------------------------------------------------- */
/* acq.h  D-TACQ ACQ400 LLC interface
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

#ifndef TE_ACQ_H_
#define TE_ACQ_H_

typedef struct ACQ {
	int lun;
	int fd;

	short* AI; 	int nai;
	unsigned* DI; 	int ndi;		// words
	unsigned sample_count;

	short *AO; 	int nao;
	unsigned* DO; 	int ndo;

	short *VI;
	short *VO;
	unsigned pai;
	unsigned pao;
} ACQ;

ACQ* acq_init(int lun);
void acq_IO(ACQ* acq);			/* blocks until next sample */
void acq_terminate(ACQ* acq);

#define SS	sizeof(short)
#define US	sizeof(unsigned)

#endif /* TE_ACQ_H_ */