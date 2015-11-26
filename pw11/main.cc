
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "misc.h"
#include "arrays.h"
#include "rgb_image.h"
#include "mlp.h"

void write_mlp_activation_image(const char *image_name,
                                int size, scalar_t factor,
                                MLP *mlp) {
  if(mlp->input_dim() != 2 || mlp->output_dim() != 1) {
    cerr << "write_mlp_activation_image needs two dimensions as input and one as output." << endl;
    abort();
  }

  cout << "Writing MLP activation image ... "; cout.flush();

  const int width = size;
  const int height = size;

  scalar_t **input_activations = allocate_array<scalar_t>(width, 2);
  scalar_t **output_activations = allocate_array<scalar_t>(width, 1);

  RGBImage image(size, size);

  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      input_activations[x][0] = scalar_t(x) / scalar_t(width);
      input_activations[x][1] = scalar_t(y) / scalar_t(height);
    }
    mlp->forward_pass(size, input_activations);
    mlp->retrieve_activations(output_activations);
    for(int x = 0; x < size; x++) {
      int r, g, b;
      scalar_t v = factor * output_activations[x][0];
      if(v < 0) {
        r = max(0, 255 + int(255 * v));
        g = r;
        b = 255;
      } else {
        r = 255;
        g = max(0, 255 - int(255 * v));
        b = g;
      }
      r = min(252, max(r, 3));
      g = min(252, max(g, 3));
      b = min(252, max(b, 3));
      image.set_pixel(x, y, r, g, b);
    }
  }

  deallocate_array<scalar_t>(input_activations);
  deallocate_array<scalar_t>(output_activations);

  image.write_jpg(image_name, 75);

  cout << "done. Wrote \"" << image_name << "\"." << endl;
}

//////////////////////////////////////////////////////////////////////

void generate_disc_data_set(DataSet *data_set) {
  for(int n = 0; n < data_set->nb_samples; n++) {
    scalar_t x, y;
    x = random_0_1(); y = random_0_1();
    data_set->input_activations[n][0] = x;
    data_set->input_activations[n][1] = y;
    data_set->output_activations[n][0] =
      (sq(x - 0.5) + sq(y - 0.5) <= 1.0 / (2.0 * M_PI)) ? 1.0 : -1.0;
  // cout << "x: " << x << "\ny:" << y << "\noutput:" << data_set->output_activations[n][0] << endl;
  }
}

void generate_square_data_set(DataSet *data_set) {
	for (int n = 0; n < data_set->nb_samples; n++) {
		scalar_t x, y, x_noise, y_noise;
		scalar_t square_size = 0.1;  // size of square region for scaling noise

		x = random_0_1(); y = random_0_1();
		// Force x and y to corners
		if (x < 0.5)
			x = 0;
		else
			x = 1;
		if (y < 0.5)
			y = 0;
		else
			y = 1;
		// Add noise
		x_noise = random_0_1() * square_size;
		y_noise = random_0_1() * square_size;
		x += x_noise;
		y += y_noise;
		// Add to data set
		data_set->input_activations[n][0] = x;
		data_set->input_activations[n][1] = y;
		// Set labels based on squares
		if (x <= square_size && y >= 1){
			data_set->output_activations[n][0] = 1;
		} else if (x <= square_size && y <= square_size){
			data_set->output_activations[n][0] = -1;
		} else if (x >= 1 && y <= square_size){
			data_set->output_activations[n][0] = 1;
		} else if (x >= 1 && y >= 1){
			data_set->output_activations[n][0] = -1;
		} else {
			cout << "Shouldnt be here" << endl;
		}
	// cout << "x: " << x << "\ny:" << y << "\noutput:" << data_set->output_activations[n][0] << endl;
	}
}


void function_example() {
  int input_dim = 2, hidden_dim = 100, output_dim = 1;
  int nb_train_samples = 10000;

  
	DataSet train_set(input_dim, output_dim, nb_train_samples);

  generate_disc_data_set(&train_set);
	for (int n = 0; n < train_set.nb_samples; n++) {
		cout << "Train set " << n << ": " << train_set.output_activations[n][0] << endl;
	}

  int layer_dim[] = { hidden_dim, output_dim };

  TransferFunctionSigmoid transfer_function;
  LossFunctionQuadratic loss_function;

	cout << "Size of layer_dim:" << sizeof(layer_dim) << endl;
	cout << "layer_dim 0:" << layer_dim[0] << endl;
	cout << "layer_dim 1:" << layer_dim[1] << endl;

  MLP mlp(&transfer_function, &loss_function,
          input_dim, sizeof(layer_dim)/sizeof(int), layer_dim);
  mlp.randomize_weights_and_biases(1.0);
  mlp.train(&train_set);

  cout << "TRAIN_LOSS " << mlp.compute_loss(&train_set) << endl;

  DataSet test_set(input_dim, output_dim, nb_train_samples);
  generate_disc_data_set(&test_set);
  cout << "TEST_LOSS " << mlp.compute_loss(&test_set) << endl;

  write_mlp_activation_image("example.jpg", 512, 1.0, &mlp);
}

void function_question1() {
  int input_dim = 2, hidden_dim = 0, output_dim = 1;
  int nb_train_samples = 1000;
	int n_layers;

	// init number of layers to determine layer_dim
	if (hidden_dim == 0)
		n_layers = 1;
	else 
		n_layers = 2;
	int layer_dim[n_layers];
	if (hidden_dim == 0)
		layer_dim[0] = output_dim;
	else 
		layer_dim[0] = hidden_dim;
		layer_dim[1] = output_dim;
 
	DataSet train_set(input_dim, output_dim, nb_train_samples);

	generate_square_data_set(&train_set);

	cout << "Size of layer_dim:" << sizeof(layer_dim) << endl;
	cout << "Size of int:" << sizeof(int) << endl;
	cout << "layer_dim0:" << layer_dim[0] << endl;
	cout << "layer_dim1:" << layer_dim[1] << endl;

  TransferFunctionSigmoid transfer_function;
  LossFunctionQuadratic loss_function;

  MLP mlp(&transfer_function, &loss_function,
          input_dim, sizeof(layer_dim)/sizeof(int), layer_dim);
  mlp.randomize_weights_and_biases(1.0);
  mlp.train(&train_set);

  // cout << "TRAIN_LOSS " << mlp.compute_loss(&train_set) << endl;

  DataSet test_set(input_dim, output_dim, nb_train_samples);
  generate_disc_data_set(&test_set);
  // cout << "TEST_LOSS " << mlp.compute_loss(&test_set) << endl;

  write_mlp_activation_image("question1.jpg", 512, 1.0, &mlp);
}

void function_question2() {
  cerr << "Not implemented yet!" << endl;
}

void function_question3() {
  cerr << "Not implemented yet!" << endl;
}

//////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
  int arg_error = 0;

  if(argc == 2) {

    if(strcmp(argv[1], "example") == 0) {
      function_example();
    }

    else if(strcmp(argv[1], "question1") == 0) {
      function_question1();
    }

    else if(strcmp(argv[1], "question2") == 0) {
      function_question2();
    }

    else if(strcmp(argv[1], "question3") == 0) {
      function_question3();
    }

    else {
      arg_error = 1;
    }

  } else {
    arg_error = 1;
  }

  if(arg_error) {
    cerr << argv[0] << " <example|question1|question2|question3>" << endl;
    exit(EXIT_FAILURE);
  }
}
