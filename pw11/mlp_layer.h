
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#ifndef MLP_LAYER_H
#define MLP_LAYER_H

#include "misc.h"
#include "transfer_function.h"
#include "loss_function.h"

class MLPLayer {
  // Size of the layer and size of the layer before it
  int dim, previous_dim;

  // Functional parameters
  scalar_t **weights, *biases;
  // Accumulators for the derivatives
  scalar_t **dloss_dweights, *dloss_dbiases;

  // Backups parameters
  scalar_t **backup_weights, *backup_biases;

  // We store in there both the layer's parameters but also quantities
  // specific to the batch of samples we are currently processing
  int nb_samples;
  scalar_t **activations, **sums;
  scalar_t **dloss_dactivations, **dloss_dsums;

  // We also have a reference to the activations of the layer before
  // this one
  scalar_t **previous_activations;
  scalar_t **previous_dloss_dactivations;

  TransferFunction *transfer_function;
  LossFunction *loss_function;

public:

  MLPLayer(TransferFunction *transfer_function, LossFunction *loss_function,
           int dim, int previous_dim);

  ~MLPLayer();

  void backup();
  void restore();

  void resize_responses(int nb_samples, scalar_t **input_activations, MLPLayer *previous_layer);

  void randomize_weights_and_biases(scalar_t epsilon);

  // Given that the activations of the previous layer are correct,
  // update, sums and activations.
  void forward_pass();

  // Given that sums and dloss_dactivations are correct, update
  // dloss_dsums and dloss_dactivations in the previous layer
  void backward_pass();

  scalar_t compute_loss(scalar_t **target_activations);
  void set_dloss_dactivation(scalar_t **target_activations);
  void compute_derivatives();
  void gradient_step(scalar_t step_size);

  void retrieve_activations(scalar_t **result_activations);
};

#endif
