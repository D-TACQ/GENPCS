/* ------------------------------------------------------------------------- */
/* file ST40PCS_stub.h                                                                 */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    http://www.d-tacq.com

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                 */
/* ------------------------------------------------------------------------- */

/** @file ST40PCS_stub.h  interface for MODEL
 * Refs:
*/


#ifndef __ST40PCS_H__
#define __ST40PCS_H__

typedef short int16_T;
typedef unsigned uint32_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  int16_T DTACQIN[384];                /* '<Root>/DTACQIN' */
} ExtU_ST40PCS_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  int16_T DTACQOUT[80];                /* '<Root>/DTACQOUT' */
} ExtY_ST40PCS_T;


/* External inputs (root inport signals with auto storage) */
extern ExtU_ST40PCS_T ST40PCS_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY_ST40PCS_T ST40PCS_Y;

/* Model entry point functions */
extern void ST40PCS_initialize(void);
extern void ST40PCS_step(void);
extern void ST40PCS_terminate(void);

#endif                                 /* RTW_HEADER_ST40PCS_h_ */
