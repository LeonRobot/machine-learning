
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

#include <stdio.h>  // for debugging with printf

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

void FunctionalRegressor::regularized_fit(scalar_t lambda, 
		int nb_samples, 
		scalar_t *x, 
		scalar_t *y) {
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

				//// debugging
				//printf("m=%i, k=%i, s=%i, xn=%4.2f\n", m, k, n, x[n]);
				//printf("Basis func m=%4.2f\n", value_basis_function(m, x[n]));
				//printf("Basis func k=%4.2f\n", value_basis_function(k, x[n]));
				//printf("f(m) * f(k) = %4.2f\n", value_basis_function(m, x[n]) * value_basis_function(k, x[n]));
				//printf("s=%4.2f\n", s);
				//while (1)
				//{
				//	if ('n' == getchar())
				//	break;
				//}
				//// end debugging
      }
      A.coeff[m][k] = s;
			// add regularization if m == k (all diagonal elements)
			// https://en.wikipedia.org/wiki/Tikhonov_regularization
			if(m == k) A.coeff[m][k] += 2 * lambda;
			
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

				// debugging
				printf("m=%i, k=%i, s=%i, xn=%4.2f\n", m, k, n, x[n]);
				printf("Basis func m=%4.2f\n", value_basis_function(m, x[n]));
				printf("Basis func k=%4.2f\n", value_basis_function(k, x[n]));
				printf("f(m) * f(k) = %4.2f\n", value_basis_function(m, x[n]) * value_basis_function(k, x[n]));
				printf("s=%4.2f\n", s);
				while (1)
				{
					if ('n' == getchar())
					break;
				}
				// end debugging
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
		// printf("Coef %d=%4.2f\n", k, s);
    _coeffs[k] = s;
  }

  delete[] V;
}
