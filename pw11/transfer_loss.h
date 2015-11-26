
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#ifndef TRANSFER_LOSS_H
#define TRANSFER_LOSS_H

#include "misc.h"

class TransferFunction {
public:
  virtual ~TransferFunction();
  virtual scalar_t transfer_function(scalar_t sum) = 0;
  virtual scalar_t dtransfer_function(scalar_t sum) = 0;
};

class TransferFunctionSigmoid : public TransferFunction {
public:
  virtual scalar_t transfer_function(scalar_t sum);
  virtual scalar_t dtransfer_function(scalar_t sum);
};

#endif
