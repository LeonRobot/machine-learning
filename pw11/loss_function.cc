
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#include "loss_function.h"

LossFunction::~LossFunction() {}

scalar_t LossFunctionQuadratic::eval(scalar_t actual_activation, scalar_t desired_activation) {
  return sq(actual_activation - desired_activation);
}

scalar_t LossFunctionQuadratic::deval(scalar_t actual_activation, scalar_t desired_activation) {
  return 2 * (actual_activation - desired_activation);
}
