
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#include "mlp_layer.h"
#include "arrays.h"

MLPLayer::MLPLayer(TransferFunction *tf, LossFunction *lf, int d, int pd) {
  transfer_function = tf;
  loss_function = lf;
  dim = d;
  previous_dim = pd;
  weights = allocate_array<scalar_t>(d, pd);
  backup_weights = 0;
  biases = new scalar_t[d];
  backup_biases = 0;

  dloss_dweights = allocate_array<scalar_t>(d, pd);
  dloss_dbiases = new scalar_t[d];

  activations = 0;
  sums = 0;
  dloss_dactivations = 0;
  dloss_dsums = 0;
  nb_samples = 0;
}

MLPLayer::~MLPLayer() {
  deallocate_array<scalar_t>(weights);
  delete[] biases;
  deallocate_array<scalar_t>(dloss_dweights);
  delete[] dloss_dbiases;

  deallocate_array<scalar_t>(activations);
  deallocate_array<scalar_t>(sums);
  deallocate_array<scalar_t>(dloss_dactivations);
  deallocate_array<scalar_t>(dloss_dsums);
}

void MLPLayer::backup() {
  if(backup_weights || backup_biases) {
    cerr << "Cannot backup twice." << endl;
    abort();
  }

  backup_weights = allocate_array<scalar_t>(dim, previous_dim);
  copy_array<scalar_t>(dim, previous_dim, backup_weights, weights);

  backup_biases = new scalar_t[dim];
  copy_vector<scalar_t>(dim, backup_biases, biases);
}

void MLPLayer::restore() {
  if(!backup_weights || !backup_biases) {
    cerr << "Restore without backup." << endl;
    abort();
  }

  copy_array<scalar_t>(dim, previous_dim, weights, backup_weights);
  deallocate_array<scalar_t>(backup_weights);
  backup_weights = 0;

  copy_vector<scalar_t>(dim, biases, backup_biases);
  delete[] backup_biases;
  backup_biases = 0;
}

void MLPLayer::resize_responses(int ns, scalar_t **input_activations, MLPLayer *previous_layer) {
  if(ns != nb_samples) {
    nb_samples = ns;

    deallocate_array<scalar_t>(activations);
    deallocate_array<scalar_t>(sums);
    deallocate_array<scalar_t>(dloss_dactivations);
    deallocate_array<scalar_t>(dloss_dsums);

    activations = allocate_array<scalar_t>(dim, nb_samples);
    sums =  allocate_array<scalar_t>(dim, nb_samples);
    dloss_dactivations =  allocate_array<scalar_t>(dim, nb_samples);
    dloss_dsums =  allocate_array<scalar_t>(dim, nb_samples);
  }

  previous_activations = previous_layer ? previous_layer->activations : input_activations;
  previous_dloss_dactivations = previous_layer ? previous_layer->dloss_dactivations : 0;
}

void MLPLayer::randomize_weights_and_biases(scalar_t epsilon) {
  for(int i = 0; i < dim; i++) {
    biases[i] = epsilon * (2.0 * drand48() - 1.0);
    for(int j = 0; j < previous_dim; j++) {
      weights[i][j] = epsilon * (2.0 * drand48() - 1.0);
    }
  }
}

void MLPLayer::forward_pass() {
  for(int n = 0; n < nb_samples; n++) {
    for(int i = 0; i < dim; i++) {
      scalar_t u = biases[i];
      for(int j = 0; j < previous_dim; j++) {
        u += weights[i][j] * previous_activations[j][n];
      }
      sums[i][n] = u;
      activations[i][n] = transfer_function->eval(u);
    }
  }
}

void MLPLayer::backward_pass() {
  for(int n = 0; n < nb_samples; n++) {
    for(int i = 0; i < dim; i++) {
      dloss_dsums[i][n] = transfer_function->deval(sums[i][n]) * dloss_dactivations[i][n];
    }
  }

  if(previous_dloss_dactivations) {
    for(int n = 0; n < nb_samples; n++) {
      for(int k = 0; k < previous_dim; k++) {
        scalar_t u = 0.0;
        for(int i = 0; i < dim; i++) {
          u += weights[i][k] * dloss_dsums[i][n];
        }
        previous_dloss_dactivations[k][n] = u;
      }
    }
  }
}

scalar_t MLPLayer::compute_loss(scalar_t **target_activations) {
  scalar_t u = 0.0;
  for(int n = 0; n < nb_samples; n++) {
    for(int i = 0; i < dim; i++) {
      u += loss_function->eval(activations[i][n], target_activations[n][i]);
    }
  }
  return u;
}

void MLPLayer::set_dloss_dactivation(scalar_t **target_activations) {
  for(int n = 0; n < nb_samples; n++) {
    for(int j = 0; j < dim; j++) {
      dloss_dactivations[j][n] =
        loss_function->deval(activations[j][n], target_activations[n][j]);
    }
  }
}

void MLPLayer::compute_derivatives() {
  for(int i = 0; i < dim; i++) {
    dloss_dbiases[i] = 0.0;
    for(int j = 0; j < previous_dim; j++) {
      dloss_dweights[i][j] = 0.0;
    }
    for(int n = 0; n < nb_samples; n++) {
      dloss_dbiases[i] += dloss_dsums[i][n];
      for(int j = 0; j < previous_dim; j++) {
        dloss_dweights[i][j] +=
          dloss_dsums[i][n] * previous_activations[j][n];
      }
    }
  }
}

void MLPLayer::gradient_step(scalar_t step_size) {
  for(int i = 0; i < dim; i++) {
    biases[i] -= step_size * dloss_dbiases[i];
    for(int j = 0; j < previous_dim; j++) {
      weights[i][j] -= step_size * dloss_dweights[i][j];
    }
  }
}

void MLPLayer::retrieve_activations(scalar_t **result_activations) {
  for(int n = 0; n < nb_samples; n++) {
    for(int i = 0; i < dim; i++) {
      result_activations[n][i] = activations[i][n];
    }
  }
}
