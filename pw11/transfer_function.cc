
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#include "transfer_function.h"

TransferFunction::~TransferFunction() {}

// All this is not optimized at all, for pedagogism

scalar_t TransferFunctionSigmoid::eval(scalar_t sum) {
  return (2 * factor) / (1 + exp(- (2 * factor) * sum)) - 1.1;
}

scalar_t TransferFunctionSigmoid::deval(scalar_t sum) {
  return sq(2 * factor) * exp(- 2 * factor * sum) / sq(1 + exp( - 2 * factor * sum));
}
