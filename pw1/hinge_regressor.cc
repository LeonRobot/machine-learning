
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
#include <stdio.h>

HingeRegressor::HingeRegressor(int nb_hinges, scalar_t xmin, scalar_t xmax) {
  _nb_hinges	= nb_hinges;
	_xmin = xmin;
	_xmax = xmax;
	_interval = (xmax - xmin) / (nb_hinges + 1);
//  	scalar_t *_hinge_positions = new scalar_t[nb_hinges];
//  	// scalar_t _hinge_positions[] = new scalar_t[nb_hinges];
// 
//  interval = (xmax - xmin) / (nb_hinges + 1);
// 
//  for(int h = 0; h < nb_hinges; h++){
//    posi += interval;
// 	 _hinge_positions[h] = posi;
// 	 printf("Hinge position %i=%4.2f\n", h, posi);
// 	 // printf("Hinge position %i=%4.2f\n", h, _hinge_positions[h]);
//  }
//  delete[] _hinge_positions;
}

HingeRegressor::~HingeRegressor() {}

int HingeRegressor::nb_basis_functions() {
  return _nb_hinges;
}

scalar_t HingeRegressor::value_basis_function(int nf, scalar_t x) {
	scalar_t r;
 	scalar_t *_hinge_positions = new scalar_t[_nb_hinges];
	scalar_t posi = _xmin;

 for(int h = 0; h < _nb_hinges; h++){
   posi += _interval;
	 _hinge_positions[h] = posi;
	 // printf("Hinge position %i=%4.2f\n", h, posi);
	 // printf("Hinge position %i=%4.2f\n", h, _hinge_positions[h]);
 }

	if(x < _hinge_positions[nf]){
		r = 0;
	}
	else{
		r = x - _hinge_positions[nf];
	}
	// printf("hinge_pos=%4.2f, nf=%i, x=%4.2f, r=%4.2f\n", _hinge_positions[nf], nf, x, r);
	delete[] _hinge_positions;
	return r;
}
