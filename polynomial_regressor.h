
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

#ifndef POLYNOMIAL_REGRESSOR_H
#define POLYNOMIAL_REGRESSOR_H

#include "misc.h"
#include "functional_regressor.h"

class PolynomialRegressor : public FunctionalRegressor {
  int _degree_max;
  virtual int nb_basis_functions();
  virtual scalar_t value_basis_function(int nf, scalar_t x);

public:
  PolynomialRegressor(int dm);
  virtual ~PolynomialRegressor();
};

#endif
