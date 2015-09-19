
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

#include "hinge_regressor.h"

HingeRegressor::HingeRegressor(int nb_hinges, scalar_t xmin, scalar_t xmax) {
	scalar_t interval;
	scalar_t posi = 0;

 _nb_hinges	= nb_hinges;
 	scalar_t *_hinge_positions = new scalar_t[nb_hinges];

 interval = (xmax - xmin) / (nb_hinges + 1);

 for(int h = 0; h < nb_hinges; h++){
   posi += interval;
	 _hinge_positions[h] = posi;
 }
}

HingeRegressor::~HingeRegressor() {}

int HingeRegressor::nb_basis_functions() {
  return _nb_hinges;
}

scalar_t HingeRegressor::value_basis_function(int nf, scalar_t x) {
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
