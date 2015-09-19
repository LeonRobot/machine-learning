
///////////////////////////////////////////////////////////////////////////
// This program is free software: you can redistribute it and/or modify  //
// it under the terms of the version 3 of the GNU General Public License //
// as published by the Free Software Foundation.                         //
//                                                                       //
// This program is distributed in the hope that it will be useful, but   //
// WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      //
// General Public License for more details.                              //
//                                                                       //
// You should have received a copy of the GNU General Public License     //
// along with this program. If not, see <http://www.gnu.org/licenses/>.  //
//                                                                       //
// (C) Idiap Research Institute                                          //
// Written by Francois Fleuret                                           //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports        //
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#include "sq_matrix.h"
#include "polynomial_regressor.h"

void example() {
  int degree = 4;
  int nb_train_samples = 100;
  int nb_test_values = 1000;

  scalar_t *x_train = new scalar_t[nb_train_samples];
  scalar_t *y_train = new scalar_t[nb_train_samples];

  for(int k = 0; k < nb_train_samples; k++) {
    x_train[k] = random_0_to_1() * 2 * M_PI - M_PI;
    y_train[k] = sin(x_train[k]);
		// printf("X_traink: %4.2f", x_train[k]);
  }

  PolynomialRegressor regressor(degree);
  regressor.fit(nb_train_samples, x_train, y_train);

  ofstream out_train("/tmp/example_train.dat");

  for(int s = 0; s < nb_train_samples; s++) {
    out_train << x_train[s] << " " << y_train[s] << endl;
  }

  ofstream out("/tmp/example.dat");

  for(int s = 0; s < nb_test_values; s++) {
		// printf("x: %4.2f s: %i s/nb: %4.2f", x, s, scalar_t(x) / scalar_t(nb_test_values);
		// printf("x: %4.2f s: %i s/nb: %4.2f\n", x, s, scalar_t(s) / scalar_t(nb_test_values));
    scalar_t x = scalar_t(s) / scalar_t(nb_test_values) * 2 * M_PI - M_PI;
    out << x << " " << sin(x) << " " << regressor.eval(x) << endl;
  }

  delete[] x_train;
  delete[] y_train;
}

void question1() {
	//int degree = 2;
 	int nb_train_samples = 100;
	int nb_test_values = 1000;
	int nb_degrees = 7;
	int degrees[] = {0,1,2,4,8,16,32};

 	scalar_t *x_train = new scalar_t[nb_train_samples];
	scalar_t *y_train = new scalar_t[nb_train_samples];

//	printf(nb_train_samples)

  for(int k = 0; k < nb_train_samples; k++){
  	x_train[k] = random_0_to_1();
  if (x_train[k] < 0.5){
  	y_train[k] = 0;
  } 
	else {
  	y_train[k] = 1;
  }
	// printf("X, Y: %4.2f %4.2f\n", x_train[k], y_train[k]);
  }	

	ofstream out_train("/tmp/question1_train.dat");
	ofstream out("/tmp/question1.dat");

	for(int s = 0; s < nb_train_samples; s++) {
		out_train << x_train[s] << " " << y_train[s] << endl;
	}

	for (int d = 0; d < nb_degrees; d++){
		int degree = degrees[d];

		PolynomialRegressor regressor(degree);
		regressor.fit(nb_train_samples, x_train, y_train);

		
		printf("Degree: %i\n", degree);
		for(int s = 0; s < nb_test_values; s++) {
			scalar_t x = scalar_t(s) / scalar_t(nb_test_values);
			out << degree << " " << x << " " << regressor.eval(x) << endl;
		}
	}

  delete[] x_train;
  delete[] y_train;
}

void question2() {
}

void question3() {
}

void question4() {
}

int main(int argc, char **argv) {
  for(int i = 1; i < argc; i++) {
    if(strcmp(argv[i], "example") == 0) {
      example();
    }

    else if(strcmp(argv[i], "question1") == 0) {
      question1();
    }

    else if(strcmp(argv[i], "question2") == 0) {
      question2();
    }

    else if(strcmp(argv[i], "question3") == 0) {
      question3();
    }

    else if(strcmp(argv[i], "question4") == 0) {
      question4();
    }

    else {
      cerr << "Unknown argument " << argv[i] << endl;
      exit(EXIT_FAILURE);
    }
  }
}
