
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#ifndef LOSS_FUNCTION_H
#define LOSS_FUNCTION_H

#include "misc.h"

class LossFunction {
public:
  virtual ~LossFunction();
  virtual scalar_t eval(scalar_t actual_activation, scalar_t desired_activation) = 0;
  virtual scalar_t deval(scalar_t actual_activation, scalar_t desired_activation) = 0;
};

class LossFunctionQuadratic : public LossFunction {
public:
  virtual scalar_t eval(scalar_t actual_activation, scalar_t desired_activation);
  virtual scalar_t deval(scalar_t actual_activation, scalar_t desired_activation);
};

#endif
