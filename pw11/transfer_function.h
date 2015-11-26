
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#ifndef TRANSFER_FUNCTION_H
#define TRANSFER_FUNCTION_H

#include "misc.h"

class TransferFunction {
public:
  virtual ~TransferFunction();
  virtual scalar_t eval(scalar_t sum) = 0;
  virtual scalar_t deval(scalar_t sum) = 0;
};

class TransferFunctionSigmoid : public TransferFunction {
public:
  static const scalar_t factor = 1.1;
  virtual scalar_t eval(scalar_t sum);
  virtual scalar_t deval(scalar_t sum);
};

#endif
