
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#include "data_set.h"
#include "arrays.h"

DataSet::DataSet() {
  input_dim = 0;
  output_dim = 0;
  nb_samples = 0;
  input_activations = 0;
  output_activations = 0;
}

DataSet::DataSet(int id, int od, int ns) {
  input_activations = 0;
  output_activations = 0;
  resize(id, od, ns);
}

void DataSet::resize(int id, int od, int ns) {
  deallocate_array<scalar_t>(input_activations);
  deallocate_array<scalar_t>(output_activations);
  input_dim = id;
  output_dim = od;
  nb_samples = ns;
  input_activations = allocate_array<scalar_t>(nb_samples, input_dim);
  output_activations = allocate_array<scalar_t>(nb_samples, output_dim);
}

DataSet::~DataSet() {
  deallocate_array<scalar_t>(input_activations);
  deallocate_array<scalar_t>(output_activations);
}

void DataSet::split(int nb_dst1, DataSet *dst1, DataSet *dst2) {
  int *shuffling = new int[nb_samples];
  random_permutation(nb_samples, shuffling);

  dst1->resize(input_dim, output_dim, nb_dst1);
  dst2->resize(input_dim, output_dim, nb_samples - nb_dst1);

  for(int k = 0; k < dst1->nb_samples; k++) {
    int l = shuffling[k];
    for(int d = 0; d < input_dim; d++) { dst1->input_activations[k][d] = input_activations[l][d]; }
    for(int d = 0; d < output_dim; d++) { dst1->output_activations[k][d] = output_activations[l][d]; }
  }

  for(int k = 0; k < dst2->nb_samples; k++) {
    int l = shuffling[k + nb_dst1];
    for(int d = 0; d < input_dim; d++) { dst2->input_activations[k][d] = input_activations[l][d]; }
    for(int d = 0; d < output_dim; d++) { dst2->output_activations[k][d] = output_activations[l][d]; }
  }

  delete[] shuffling;
}
