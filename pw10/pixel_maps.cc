
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

#include "pixel_maps.h"

PixelMaps::PixelMaps(int size) : _nb_ref(0),_core(new unsigned char[size]) {}

PixelMaps::~PixelMaps() {
  delete[] _core;
}

PixelMaps *PixelMaps::add_ref() {
  _nb_ref++;
  return this;
}

void PixelMaps::del_ref() {
  _nb_ref--;
  if(_nb_ref == 0) delete this;
}
