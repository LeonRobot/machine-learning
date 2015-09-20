
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

#ifndef LINEAR_REGRESSOR_H
#define LINEAR_REGRESSOR_H

#include "misc.h"

class FunctionalRegressor {
  scalar_t *_coeffs;
  virtual int nb_basis_functions() = 0;
  virtual scalar_t value_basis_function(int nf, scalar_t x) = 0;

public:
  FunctionalRegressor();
  ~FunctionalRegressor();

  scalar_t eval(scalar_t x);
  void fit(int nb_samples, scalar_t *x, scalar_t *y);
	void regularized_fit(scalar_t lambda, int nb_samples, scalar_t *x, scalar_t *y);
};

#endif
