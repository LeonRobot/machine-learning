
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#include "mlp.h"

// All this is not optimized at all, for pedagogism

MLP::MLP(TransferFunction *transfer_function, LossFunction *loss_function,
         int input_dim, int nb_layers, const int *layer_dim) {
  _show_loss_during_training = 1;
  _initialized = 0;

  _transfer_function = transfer_function;
  _loss_function = loss_function;

  _input_dim = input_dim;
  _output_dim = layer_dim[nb_layers-1];

  _nb_layers = nb_layers;
  _layers = new MLPLayer *[_nb_layers];
  _input_activations = 0;
  for(int l = 0; l < _nb_layers; l++) {
		cout << "l=" << l << "layer_dim=" << layer_dim[l] << endl;
    _layers[l] = new MLPLayer(_transfer_function, _loss_function,
                              layer_dim[l], l == 0 ? _input_dim : layer_dim[l-1]);
  }
}

MLP::~MLP() {
  for(int l = 0; l < _nb_layers; l++) {
    delete _layers[l];
  }

  deallocate_array<scalar_t>(_input_activations);
  delete[] _layers;
}

void MLP::resize_responses(int nb_samples) {
  _nb_samples = nb_samples;
  deallocate_array<scalar_t>(_input_activations);
  _input_activations = allocate_array<scalar_t>(_input_dim, _nb_samples);
  for(int l = 0; l < _nb_layers; l++) {
    _layers[l]->resize_responses(nb_samples, _input_activations, l == 0 ? 0 : _layers[l-1]);
  }
}

void MLP::show_loss_during_training(int flag) {
  _show_loss_during_training = flag;
}

void MLP::retrieve_activations(scalar_t **activations) {
  _layers[_nb_layers - 1]->retrieve_activations(activations);
}

void MLP::randomize_weights_and_biases(scalar_t epsilon) {
  for(int l = 0; l < _nb_layers; l++) {
    _layers[l]->randomize_weights_and_biases(epsilon);
  }
  _initialized = 1;
}

void MLP::forward_pass(int nb_samples, scalar_t **input_activations) {
  resize_responses(nb_samples);

  for(int n = 0; n < _nb_samples; n++) {
    for(int i = 0; i < _input_dim; i++) {
      _input_activations[i][n] = input_activations[n][i];
    }
  }

  for(int l = 0; l < _nb_layers; l++) {
    _layers[l]->forward_pass();
  }
}

void MLP::compute_derivatives(int nb_samples,
                              scalar_t **input_activations,
                              scalar_t **desired_output_activations) {

  forward_pass(nb_samples, input_activations);
  _layers[_nb_layers - 1]->set_dloss_dactivation(desired_output_activations);

  for(int l = _nb_layers - 1; l >= 0; l--) {
    _layers[l]->backward_pass();
    _layers[l]->compute_derivatives();
  }
}

void MLP::gradient_step(scalar_t step_size,
                        int nb_samples,
                        scalar_t **input_activations,
                        scalar_t **desired_output_activations) {

  compute_derivatives(nb_samples, input_activations, desired_output_activations);

  for(int l = 0; l < _nb_layers; l++) {
    _layers[l]->gradient_step(step_size);
  }
}

scalar_t MLP::compute_loss(DataSet *data_set) {
  forward_pass(data_set->nb_samples, data_set->input_activations);
  return _layers[_nb_layers-1]->compute_loss(data_set->output_activations);
}

void MLP::train(DataSet *full_set,
                int nb_epochs_min,
                int max_nb_epochs_after_noticeable_reduction, scalar_t noticeable_loss_reduction) {
  const scalar_t step_size_acceleration = 1.1;
  const scalar_t step_size_deceleration = 0.5;
  scalar_t prev_loss = -1;
  scalar_t step_size;

  if(!_initialized) {
    cerr << "The weights and biases have not been initialized." << endl;
    abort();
  }

  DataSet train_set, validation_set;

  full_set->split(full_set->nb_samples/2, &train_set, &validation_set);

  int *shuffling = new int[train_set.nb_samples];

  {
    scalar_t candidate_step_size = 1.0, best_step_size = -1, best_loss;
    random_permutation(train_set.nb_samples, shuffling);

    for(int q = 0; q < 10; q++) {
      for(int l = 0; l < _nb_layers; l++) {
        _layers[l]->backup();
      }

      for(int k = 0; k < train_set.nb_samples; k++) {
        int j = shuffling[k];
        gradient_step(candidate_step_size,
                      1, train_set.input_activations + j,
                      train_set.output_activations + j);
      }

      scalar_t loss = compute_loss(&train_set);

      if(q == 0 || loss < best_loss) {
        best_loss = loss;
        best_step_size = candidate_step_size;
      }

      for(int l = 0; l < _nb_layers; l++) {
        _layers[l]->restore();
      }

      candidate_step_size *= 0.1;
    }

    step_size = best_step_size;
  }

  if(_show_loss_during_training) {
    cout << "STEP_SIZE " << step_size << endl;
  }

  int e = 0, best_e;
  scalar_t best_loss;

  while(e < nb_epochs_min || e < best_e + max_nb_epochs_after_noticeable_reduction) {

    random_permutation(train_set.nb_samples, shuffling);

    for(int k = 0; k < train_set.nb_samples; k++) {
      int j = shuffling[k];
      gradient_step(step_size,
                    1, train_set.input_activations + j,
                    train_set.output_activations + j);
    }

    scalar_t loss = compute_loss(&validation_set);

    if(e > 0) {
      if(loss > prev_loss) {
        step_size *= step_size_deceleration;
      } else {
        step_size *= step_size_acceleration;
      }
    }

    if(e == 0 || loss < noticeable_loss_reduction * best_loss) {
      best_loss = loss;
      best_e = e;
    }

    prev_loss = loss;

    if(_show_loss_during_training) {
      cout << "LOSS " << e << " " << loss << endl;
    }

    e++;
  }

  delete[] shuffling;
}

void MLP::train(DataSet *train_set) {
  train(train_set, 10, 10, 0.999);
}

void MLP::test(scalar_t *input_activations, scalar_t *output_activations) {
  forward_pass(1, &input_activations);
  _layers[_nb_layers - 1]->retrieve_activations(&output_activations);
}
