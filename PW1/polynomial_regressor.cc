
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

#include "polynomial_regressor.h"

PolynomialRegressor::PolynomialRegressor(int dm) {
  _degree_max = dm;
}

PolynomialRegressor::~PolynomialRegressor() {}

int PolynomialRegressor::nb_basis_functions() {
  return _degree_max + 1;
}

scalar_t PolynomialRegressor::value_basis_function(int nf, scalar_t x) {
  if(nf == 0) return 1.0;
  else if(nf == 1) return x;
  else {
    scalar_t r;
    r = value_basis_function(nf/2, x);
    r *= r;
    if(nf%2) r *= x;
    return r;
  }
}
