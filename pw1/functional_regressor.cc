
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

#include "functional_regressor.h"
#include "sq_matrix.h"

FunctionalRegressor::FunctionalRegressor() {
  _coeffs = 0;
}

FunctionalRegressor::~FunctionalRegressor() {
  delete[] _coeffs;
}

scalar_t FunctionalRegressor::eval(scalar_t x) {
  scalar_t s = 0;
  for(int n = 0; n < nb_basis_functions(); n++) {
    s += _coeffs[n] * value_basis_function(n, x);
  }
  return s;
}

void FunctionalRegressor::fit(int nb_samples, scalar_t *x, scalar_t *y) {
  delete[] _coeffs;
  _coeffs = new scalar_t[nb_basis_functions()];

  const int d = nb_basis_functions();

  SqMatrix A(d);

  scalar_t *V = new scalar_t[d];

  scalar_t s;
  for(int m = 0; m < d; m++) {
    for(int k = 0; k < d; k++) {
      s = 0;
      for(int n = 0; n < nb_samples; n++) {
        s += value_basis_function(m, x[n]) * value_basis_function(k, x[n]);
      }
      A.coeff[m][k] = s;
    }
    s = 0;
    for(int n = 0; n < nb_samples; n++) {
      s += value_basis_function(m, x[n]) * y[n];
    }
    V[m] = s;
  }

  A.invert();

  for(int k = 0; k < d; k++) {
    s = 0;
    for(int m = 0; m < d; m++) {
      s += A.coeff[m][k] * V[m];
    }
    _coeffs[k] = s;
  }

  delete[] V;
}
