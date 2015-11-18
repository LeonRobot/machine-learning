
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

#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "vignette_set.h"

class Classifier {
public:
  // Train the classifier with a training set
  virtual void train(VignetteSet *train_set) = 0;
  // Compute the response on a specified vignette of the set
  virtual int predict(VignetteSet *vs, int n_vignette) = 0;
  // Compute the number of classification error on the provided set
  int nb_errors(VignetteSet *vs);
};

#endif
