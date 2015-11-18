
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

#ifndef VIGNETTE_SET_H
#define VIGNETTE_SET_H

#include "misc.h"
#include "pixel_maps.h"

class VignetteSet {
  int _nb_vignettes, _nb_classes;
  int _width, _height;
  PixelMaps *_pixel_maps;
  unsigned char **_pixels, *_labels;

public:
  VignetteSet();
  ~VignetteSet();

  void free();

  inline int nb_vignettes() { return _nb_vignettes; }
  inline int nb_classes() { return _nb_classes; }
  inline int width() { return _width; }
  inline int height() { return _height; }

  // Value of pixel (x, y) in vignette p
  inline unsigned char pixel(int p, int x, int y) {
    ASSERT(p >= 0 && p < _nb_vignettes &&
           x >= 0 && x < _width &&
           y >= 0 && y < _height);
    return _pixels[p][x + y * _width];
  }

  // Label of vignette p
  inline unsigned char label(int p) { return _labels[p]; }

  // Load a data file in the MNIST (MATLAB) format (erase the current
  // content of the set)
  void load_mnist_format(char *picture_file_name, char *label_file_name);

  // Replaces the content with nb vignettes sampled uniformly in vs
  void bootstrap(int nb, VignetteSet *vs);
};

#endif
