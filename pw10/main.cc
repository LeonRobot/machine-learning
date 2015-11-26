
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
// Written by and Copyright (C) Francois Fleuret                         //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports        //
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

using namespace std;

#include "vignette_set.h"
#include "decision_tree.h"
#include "pixel_question_set.h"
#include "min_question_set.h"

#define SMALL_BUFFER_SIZE 1024

void check_opt(int argc, char **argv, int n_opt, int n, const char *help) {
  if(n_opt + n >= argc) {
    cerr << "Missing argument for " << argv[n_opt] << ".\n";
    cerr << "Expecting " << help << ".\n";
    exit(1);
  }
}

void print_help_and_exit(int e) {
  cout<< "\n";
  exit(e);
}

void check_data_sets(VignetteSet *train_image_set,
                     VignetteSet *test_image_set) {

  if(train_image_set->nb_vignettes() < 0) {
    cerr << "The train set is empty. Did you forget --train-data-files?" << endl;
    exit(1);
  }

  if(test_image_set->nb_vignettes() < 0) {
    cerr << "The test set is empty. Did you forget --test-data-files?" << endl;
    exit(1);
  }
}

void computation_demo(VignetteSet *train_image_set,
                      VignetteSet *test_image_set) {

  PixelQuestionSet qs(train_image_set->width(),
                      train_image_set->height());

  ofstream out_train("train_error.dat");
  ofstream out_test("test_error.dat");
  scalar_t train_error, test_error;

  for(int d = 1; d < 20; d++) {
    cout << "Depth " << d << " ... "; cout.flush();
    DecisionTree dt(&qs,
                    d,  // Depth max
                    1, // Number of samples min to split
                    100); // How many question sampled per node

    dt.train(train_image_set);

    train_error =
      scalar_t(dt.nb_errors(train_image_set)) /
      scalar_t(train_image_set->nb_vignettes());

    test_error =
      scalar_t(dt.nb_errors(test_image_set)) /
      scalar_t(test_image_set->nb_vignettes());

    out_train << d << " " << train_error << endl;
    out_test << d << " " << test_error << endl;

    cout << "Train_error/test_error ("
         << train_error * 100
         << "% / "
         << test_error * 100
         << "%)." << endl;
  }
}

void computation_question1(VignetteSet *train_image_set,
                           VignetteSet *test_image_set) {
  // cerr << "Not implemented!" << endl;
	VignetteSet train_image_set_sub;
	//int nb=16;
	
	int nb;

  for(int nb_base = 4; nb_base < 16; nb_base++) {

		nb=pow(2, nb_base);
		
		train_image_set_sub.bootstrap(nb, train_image_set);

		PixelQuestionSet qs(train_image_set_sub.width(),
												train_image_set_sub.height());

		ofstream out_train("train_error.dat");
		ofstream out_test("test_error.dat");
		scalar_t train_error, test_error;
		int d=10;
		
		DecisionTree dt(&qs,
					d,  // Depth max
					10, // Number of samples min to split
					100); // How many question sampled per node

		dt.train(&train_image_set_sub);

		train_error =
		scalar_t(dt.nb_errors(&train_image_set_sub)) /
		scalar_t(train_image_set_sub.nb_vignettes());

    test_error =
      scalar_t(dt.nb_errors(test_image_set)) /
      scalar_t(test_image_set->nb_vignettes());

		out_train << d << " " << train_error << endl;
		out_test << d << " " << test_error << endl;

		cout << "nb:"
			   << nb
		     << "Train_error/test_error ("
				 << train_error * 100
				 << "% / "
				 << test_error * 100
				 << "%)." << endl;
	}

}

void computation_question2(VignetteSet *train_image_set,
                           VignetteSet *test_image_set) {

  MinQuestionSet qs(train_image_set->width(),
                    train_image_set->height());

  ofstream out_train("train_error.dat");
  ofstream out_test("test_error.dat");
  scalar_t train_error, test_error;

  for(int d = 1; d < 20; d++) {
    cout << "Depth " << d << " ... "; cout.flush();
    DecisionTree dt(&qs,
                    d,  // Depth max
                    1, // Number of samples min to split
                    100); // How many question sampled per node

    dt.train(train_image_set);

    train_error =
      scalar_t(dt.nb_errors(train_image_set)) /
      scalar_t(train_image_set->nb_vignettes());

    test_error =
      scalar_t(dt.nb_errors(test_image_set)) /
      scalar_t(test_image_set->nb_vignettes());

    out_train << d << " " << train_error << endl;
    out_test << d << " " << test_error << endl;

    cout << "Train_error/test_error ("
         << train_error * 100
         << "% / "
         << test_error * 100
         << "%)." << endl;
  }
}

void computation_question3(VignetteSet *train_image_set,
                           VignetteSet *test_image_set) {
  cerr << "Not implemented!" << endl;
  exit(1);
}

void computation_question4(VignetteSet *train_image_set,
                           VignetteSet *test_image_set) {
  cerr << "Not implemented!" << endl;
  exit(1);
}

int main(int argc, char **argv) {
  int nb_training_examples;
  int nb_validation_examples;

  VignetteSet train_image_set, test_image_set;

  // Parsing the command line parameters ///////////////////////////////

  int i = 1;

  while(i < argc) {

    if(argc == 1 || strcmp(argv[i], "--help") == 0) print_help_and_exit(0);

    else if(strcmp(argv[i], "--train-data-files") == 0) {
      check_opt(argc, argv, i, 2,
                "<string: pixel filename> <string: label filename>");

      cout << "Loading the train data ..."; cout.flush();
      train_image_set.load_mnist_format(argv[i+1], argv[i+2]);
      cout << " done.\n"; cout.flush();

      i += 3;
    }

    else if(strcmp(argv[i], "--test-data-files") == 0) {
      check_opt(argc, argv, i, 2,
                "<string: pixel filename> <string: label filename>");

      cout << "Loading the test data ..."; cout.flush();
      test_image_set.load_mnist_format(argv[i+1], argv[i+2]);
      cout << " done.\n"; cout.flush();

      i += 3;
    }

    else if(strcmp(argv[i], "--nb-training-examples") == 0) {
      check_opt(argc, argv, i, 1,
                "<int: number of examples for the training>");
      nb_training_examples = atoi(argv[i+1]);
      i += 2;
    }

    else if(strcmp(argv[i], "--nb-validation-examples") == 0) {
      check_opt(argc, argv, i, 1,
                "<int: number of examples for the validation>");
      nb_validation_examples = atoi(argv[i+1]);
      i += 2;
    }

    else if(strcmp(argv[i], "demo") == 0) {
      check_data_sets(&train_image_set, &test_image_set);
      computation_demo(&train_image_set, &test_image_set);
      i++;
    }

    else if(strcmp(argv[i], "question1") == 0) {
      check_data_sets(&train_image_set, &test_image_set);
      computation_question1(&train_image_set, &test_image_set);
      i++;
    }

    else if(strcmp(argv[i], "question2") == 0) {
      check_data_sets(&train_image_set, &test_image_set);
      computation_question2(&train_image_set, &test_image_set);
      i++;
    }

    else if(strcmp(argv[i], "question3") == 0) {
      check_data_sets(&train_image_set, &test_image_set);
      computation_question3(&train_image_set, &test_image_set);
      i++;
    }

    else if(strcmp(argv[i], "question4") == 0) {
      check_data_sets(&train_image_set, &test_image_set);
      computation_question4(&train_image_set, &test_image_set);
      i++;
    }

    else {
      cerr << "Unknown option " << argv[i] << "\n";
      print_help_and_exit(1);
    }
  }
}
