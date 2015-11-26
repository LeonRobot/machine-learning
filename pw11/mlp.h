
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#ifndef MLP_H
#define MLP_H

#include "misc.h"
#include "arrays.h"
#include "mlp_layer.h"
#include "data_set.h"

class MLP {
  TransferFunction *_transfer_function;
  LossFunction *_loss_function;

  int _nb_layers, _nb_samples;
  int _input_dim, _output_dim;

  MLPLayer **_layers;

  scalar_t **_input_activations;

  int _show_loss_during_training;

  int _initialized;

public:

  MLP(TransferFunction *transfer_function, LossFunction *loss_function,
      int input_dim, int nb_layers, const int *layer_dim);

  ~MLP();

  inline int input_dim() { return _input_dim; }

  inline int output_dim() { return _output_dim; }

  void show_loss_during_training(int flag);

  void retrieve_activations(scalar_t **activations);

  void resize_responses(int nb_samples);

  void randomize_weights_and_biases(scalar_t epsilon);

  void forward_pass(int nb_samples,
                    scalar_t **input_activations);

  void compute_derivatives(int nb_samples,
                           scalar_t **input_activations,
                           scalar_t **desired_output_activations);

  void gradient_step(scalar_t step_size,
                     int nb_samples,
                     scalar_t **input_activations,
                     scalar_t **desired_output_activations);

  scalar_t compute_loss(DataSet *data_set);

  void train(DataSet *train_set,
             int nb_epochs_min,
             int max_nb_epochs_after_noticeable_reduction, scalar_t noticeable_loss_reduction);

  void train(DataSet *train_set);

  void test(scalar_t *input_activations, scalar_t *output_activations);
};

#endif
