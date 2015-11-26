
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#ifndef DATA_SET_H
#define DATA_SET_H

#include "misc.h"

class DataSet {
public:
  int input_dim, output_dim;
  int nb_samples;
  scalar_t **input_activations;
  scalar_t **output_activations;

  DataSet();
  DataSet(int input_dim, int output_dim, int nb_samples);
  ~DataSet();

  void resize(int id, int od, int ns);
  void split(int nb_dst1, DataSet *dst1, DataSet *dst2);
};

#endif
