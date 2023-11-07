//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: odometer.cpp
//
// Code generated for Simulink model 'odometer'.
//
// Model version                  : 1.5
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Mon Nov  6 21:54:31 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: Generic->Unspecified (assume 32-bit Generic)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "odometer.h"
#include "rtwtypes.h"
#include "odometer_types.h"
#include "odometer_private.h"

// Block signals (default storage)
B_odometer_T odometer_B;

// Continuous states
X_odometer_T odometer_X;

// Block states (default storage)
DW_odometer_T odometer_DW;

// Real-time model
RT_MODEL_odometer_T odometer_M_ = RT_MODEL_odometer_T();
RT_MODEL_odometer_T *const odometer_M = &odometer_M_;

//
// This function updates continuous states using the ODE3 fixed-step
// solver algorithm
//
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  // Solver Matrices
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = static_cast<ODE3_IntgData *>(rtsiGetSolverData(si));
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 1;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  // Save the state values at time t in y, we'll use x as ynew.
  (void) memcpy(y, x,
                static_cast<uint_T>(nXc)*sizeof(real_T));

  // Assumes that rtsiSetT and ModelOutputs are up-to-date
  // f0 = f(t,y)
  rtsiSetdX(si, f0);
  odometer_derivatives();

  // f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*));
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  odometer_step();
  odometer_derivatives();

  // f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*));
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  odometer_step();
  odometer_derivatives();

  // tnew = t + hA(3);
  // ynew = y + f*hB(:,3);
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

// Model step function
void odometer_step(void)
{
  SL_Bus_odometer_std_msgs_Float64 rtb_BusAssignment;
  real_T value;
  if (rtmIsMajorTimeStep(odometer_M)) {
    // set solver stop time
    rtsiSetSolverStopTime(&odometer_M->solverInfo,
                          ((odometer_M->Timing.clockTick0+1)*
      odometer_M->Timing.stepSize0));
  }                                    // end MajorTimeStep

  // Update absolute time of base rate at minor time step
  if (rtmIsMinorTimeStep(odometer_M)) {
    odometer_M->Timing.t[0] = rtsiGetT(&odometer_M->solverInfo);
  }

  // MATLABSystem: '<Root>/Get Parameter1'
  ParamGet_odometer_10.get_parameter(&value);

  // Integrator: '<Root>/Integrator' incorporates:
  //   MATLABSystem: '<Root>/Get Parameter1'

  if (odometer_DW.Integrator_IWORK != 0) {
    odometer_X.Integrator_CSTATE = value;
  }

  // BusAssignment: '<Root>/Bus Assignment' incorporates:
  //   Integrator: '<Root>/Integrator'

  rtb_BusAssignment.Data = odometer_X.Integrator_CSTATE;

  // Outputs for Atomic SubSystem: '<Root>/Publish'
  // MATLABSystem: '<S2>/SinkBlock'
  Pub_odometer_4.publish(&rtb_BusAssignment);

  // End of Outputs for SubSystem: '<Root>/Publish'
  if (rtmIsMajorTimeStep(odometer_M)) {
    boolean_T b_varargout_1;

    // Outputs for Atomic SubSystem: '<Root>/Subscribe'
    // MATLABSystem: '<S3>/SourceBlock'
    b_varargout_1 = Sub_odometer_1.getLatestMessage(&rtb_BusAssignment);

    // Outputs for Enabled SubSystem: '<S3>/Enabled Subsystem' incorporates:
    //   EnablePort: '<S4>/Enable'

    if (b_varargout_1) {
      // SignalConversion generated from: '<S4>/In1'
      odometer_B.In1 = rtb_BusAssignment;
    }

    // End of MATLABSystem: '<S3>/SourceBlock'
    // End of Outputs for SubSystem: '<S3>/Enabled Subsystem'
    // End of Outputs for SubSystem: '<Root>/Subscribe'
  }

  if (rtmIsMajorTimeStep(odometer_M)) {
    // Update for Integrator: '<Root>/Integrator'
    odometer_DW.Integrator_IWORK = 0;
  }                                    // end MajorTimeStep

  if (rtmIsMajorTimeStep(odometer_M)) {
    rt_ertODEUpdateContinuousStates(&odometer_M->solverInfo);

    // Update absolute time for base rate
    // The "clockTick0" counts the number of times the code of this task has
    //  been executed. The absolute time is the multiplication of "clockTick0"
    //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
    //  overflow during the application lifespan selected.

    ++odometer_M->Timing.clockTick0;
    odometer_M->Timing.t[0] = rtsiGetSolverStopTime(&odometer_M->solverInfo);

    {
      // Update absolute timer for sample time: [0.2s, 0.0s]
      // The "clockTick1" counts the number of times the code of this task has
      //  been executed. The resolution of this integer timer is 0.2, which is the step size
      //  of the task. Size of "clockTick1" ensures timer will not overflow during the
      //  application lifespan selected.

      odometer_M->Timing.clockTick1++;
    }
  }                                    // end MajorTimeStep
}

// Derivatives for root system: '<Root>'
void odometer_derivatives(void)
{
  XDot_odometer_T *_rtXdot;
  _rtXdot = ((XDot_odometer_T *) odometer_M->derivs);

  // Derivatives for Integrator: '<Root>/Integrator'
  _rtXdot->Integrator_CSTATE = odometer_B.In1.Data;
}

// Model initialize function
void odometer_initialize(void)
{
  // Registration code
  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&odometer_M->solverInfo,
                          &odometer_M->Timing.simTimeStep);
    rtsiSetTPtr(&odometer_M->solverInfo, &rtmGetTPtr(odometer_M));
    rtsiSetStepSizePtr(&odometer_M->solverInfo, &odometer_M->Timing.stepSize0);
    rtsiSetdXPtr(&odometer_M->solverInfo, &odometer_M->derivs);
    rtsiSetContStatesPtr(&odometer_M->solverInfo, (real_T **)
                         &odometer_M->contStates);
    rtsiSetNumContStatesPtr(&odometer_M->solverInfo,
      &odometer_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&odometer_M->solverInfo,
      &odometer_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&odometer_M->solverInfo,
      &odometer_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&odometer_M->solverInfo,
      &odometer_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&odometer_M->solverInfo, (&rtmGetErrorStatus
      (odometer_M)));
    rtsiSetRTModelPtr(&odometer_M->solverInfo, odometer_M);
  }

  rtsiSetSimTimeStep(&odometer_M->solverInfo, MAJOR_TIME_STEP);
  odometer_M->intgData.y = odometer_M->odeY;
  odometer_M->intgData.f[0] = odometer_M->odeF[0];
  odometer_M->intgData.f[1] = odometer_M->odeF[1];
  odometer_M->intgData.f[2] = odometer_M->odeF[2];
  odometer_M->contStates = ((X_odometer_T *) &odometer_X);
  rtsiSetSolverData(&odometer_M->solverInfo, static_cast<void *>
                    (&odometer_M->intgData));
  rtsiSetIsMinorTimeStepWithModeChange(&odometer_M->solverInfo, false);
  rtsiSetSolverName(&odometer_M->solverInfo,"ode3");
  rtmSetTPtr(odometer_M, &odometer_M->Timing.tArray[0]);
  odometer_M->Timing.stepSize0 = 0.2;
  rtmSetFirstInitCond(odometer_M, 1);

  {
    char_T b_zeroDelimTopic_0[6];
    char_T b_zeroDelimTopic[5];
    char_T b_zeroDelimName[3];
    static const char_T tmp[5] = { 'v', 'e', 'l', '_', 'x' };

    // InitializeConditions for Integrator: '<Root>/Integrator'
    if (rtmIsFirstInitCond(odometer_M)) {
      odometer_X.Integrator_CSTATE = 0.0;
    }

    odometer_DW.Integrator_IWORK = 1;

    // End of InitializeConditions for Integrator: '<Root>/Integrator'

    // SystemInitialize for Atomic SubSystem: '<Root>/Publish'
    // Start for MATLABSystem: '<S2>/SinkBlock'
    odometer_DW.obj_b.matlabCodegenIsDeleted = false;
    odometer_DW.obj_b.isInitialized = 1;
    b_zeroDelimTopic[0] = 'o';
    b_zeroDelimTopic[1] = 'd';
    b_zeroDelimTopic[2] = 'o';
    b_zeroDelimTopic[3] = 'm';
    b_zeroDelimTopic[4] = '\x00';
    Pub_odometer_4.createPublisher(&b_zeroDelimTopic[0], 1);
    odometer_DW.obj_b.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<Root>/Publish'

    // SystemInitialize for Atomic SubSystem: '<Root>/Subscribe'
    // SystemInitialize for Enabled SubSystem: '<S3>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S4>/In1' incorporates:
    //   Outport: '<S4>/Out1'

    odometer_B.In1 = odometer_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S3>/Enabled Subsystem'

    // Start for MATLABSystem: '<S3>/SourceBlock'
    odometer_DW.obj_n.matlabCodegenIsDeleted = false;
    odometer_DW.obj_n.isInitialized = 1;
    for (int32_T i = 0; i < 5; i++) {
      b_zeroDelimTopic_0[i] = tmp[i];
    }

    b_zeroDelimTopic_0[5] = '\x00';
    Sub_odometer_1.createSubscriber(&b_zeroDelimTopic_0[0], 1);
    odometer_DW.obj_n.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S3>/SourceBlock'
    // End of SystemInitialize for SubSystem: '<Root>/Subscribe'

    // Start for MATLABSystem: '<Root>/Get Parameter1'
    odometer_DW.obj.matlabCodegenIsDeleted = false;
    odometer_DW.obj.isInitialized = 1;
    b_zeroDelimName[0] = 'x';
    b_zeroDelimName[1] = '0';
    b_zeroDelimName[2] = '\x00';
    ParamGet_odometer_10.initialize(&b_zeroDelimName[0]);
    ParamGet_odometer_10.initialize_error_codes(0, 1, 2, 3);
    ParamGet_odometer_10.set_initial_value(0.0);
    odometer_DW.obj.isSetupComplete = true;
  }

  // set "at time zero" to false
  if (rtmIsFirstInitCond(odometer_M)) {
    rtmSetFirstInitCond(odometer_M, 0);
  }
}

// Model terminate function
void odometer_terminate(void)
{
  // Terminate for MATLABSystem: '<Root>/Get Parameter1'
  if (!odometer_DW.obj.matlabCodegenIsDeleted) {
    odometer_DW.obj.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Parameter1'

  // Terminate for Atomic SubSystem: '<Root>/Publish'
  // Terminate for MATLABSystem: '<S2>/SinkBlock'
  if (!odometer_DW.obj_b.matlabCodegenIsDeleted) {
    odometer_DW.obj_b.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S2>/SinkBlock'
  // End of Terminate for SubSystem: '<Root>/Publish'

  // Terminate for Atomic SubSystem: '<Root>/Subscribe'
  // Terminate for MATLABSystem: '<S3>/SourceBlock'
  if (!odometer_DW.obj_n.matlabCodegenIsDeleted) {
    odometer_DW.obj_n.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S3>/SourceBlock'
  // End of Terminate for SubSystem: '<Root>/Subscribe'
}

//
// File trailer for generated code.
//
// [EOF]
//
