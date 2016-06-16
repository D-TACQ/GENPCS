/*
 * ST40PCS.h
 *
 * Code generation for model "ST40PCS".
 *
 * Model version              : 1.4
 * Simulink Coder version : 8.10 (R2016a) 10-Feb-2016
 * C source code generated on : Thu Jun  9 10:14:36 2016
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Emulation hardware selection:
 *    Differs from embedded hardware (Intel->x86-64 (Linux 64))
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_ST40PCS_h_
#define RTW_HEADER_ST40PCS_h_
#include <math.h>
#include <float.h>
#include <string.h>
#include <stddef.h>
#ifndef ST40PCS_COMMON_INCLUDES_
# define ST40PCS_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#endif                                 /* ST40PCS_COMMON_INCLUDES_ */

#include "ST40PCS_types.h"

/* Shared type includes */
#include "multiword_types.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
# define rtmGetFinalTime(rtm)          ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWLogInfo
# define rtmGetRTWLogInfo(rtm)         ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  ((rtm)->Timing.taskTime0)
#endif

#ifndef rtmGetTFinal
# define rtmGetTFinal(rtm)             ((rtm)->Timing.tFinal)
#endif

/* External inputs (root inport signals with auto storage) */
typedef struct {
  int16_T DTACQIN[130];                /* '<Root>/DTACQIN' */
} ExtU_ST40PCS_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  int16_T DTACQOUT[34];                /* '<Root>/DTACQOUT' */
} ExtY_ST40PCS_T;

/* Parameters (auto storage) */
struct P_ST40PCS_T_ {
  real_T SATGain1[4096];               /* Variable: SATGain1
                                        * Referenced by: '<Root>/Gain'
                                        */
  int16_T SATGain2[4];                 /* Variable: SATGain2
                                        * Referenced by: '<Root>/Gain1'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_ST40PCS_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T taskTime0;
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    time_T tFinal;
    boolean_T stopRequestedFlag;
  } Timing;
};

/* Block parameters (auto storage) */
extern P_ST40PCS_T ST40PCS_P;

/* External inputs (root inport signals with auto storage) */
extern ExtU_ST40PCS_T ST40PCS_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY_ST40PCS_T ST40PCS_Y;

/* Model entry point functions */
extern void ST40PCS_initialize(void);
extern void ST40PCS_step(void);
extern void ST40PCS_terminate(void);

/* Real-time Model object */
extern RT_MODEL_ST40PCS_T *const ST40PCS_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'ST40PCS'
 */
#endif                                 /* RTW_HEADER_ST40PCS_h_ */
