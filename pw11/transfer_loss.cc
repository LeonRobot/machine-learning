
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#include "transfer_loss.h"

TransferFunction::~TransferFunction() {}

scalar_t TransferFunctionSigmoid::transfer_function(scalar_t sum) {
  return 2 / (1 + exp(-2 * x)) - 1;
}

scalar_t TransferFunctionSigmoid::dtransfer_function(scalar_t sum) {
  return 4 / (exp(x) + exp(-x));
}
