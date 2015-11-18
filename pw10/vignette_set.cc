
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

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

#include "vignette_set.h"

const unsigned int mnist_pictures_magic = 0x00000803;
const unsigned int mnist_labels_magic = 0x00000801;

inline unsigned int read_high_endian_int(istream &is) {
  unsigned int result;
  char *s = (char *) &result;
  char c;
  is.read(s, sizeof(result));
  c = s[0]; s[0] = s[3]; s[3] = c;
  c = s[1]; s[1] = s[2]; s[2] = c;
  return result;
}

VignetteSet::VignetteSet() : _nb_vignettes(-1), _nb_classes(0),
                             _width(-1), _height(-1),
                             _pixel_maps(0), _pixels(0), _labels(0) { }

VignetteSet::~VignetteSet() {
  free();
}

void VignetteSet::free() {
  if(_pixel_maps) _pixel_maps->del_ref();
  delete[] _pixels;
  delete[] _labels;
}

void VignetteSet::load_mnist_format(char *picture_file_name,
                                    char *label_file_name) {

  unsigned int magic;

  ifstream picture_is(picture_file_name);

  if(picture_is.fail()) {
    cerr << "Can not open file [" << picture_file_name << "].\n";
    exit(1);
  }

  magic = read_high_endian_int(picture_is);
  if(magic != mnist_pictures_magic) {
    cerr << "Invalid magic for picture, file "
         << "[" << picture_file_name << "] number [" << magic << "]\n";
    exit(1);
  }

  free();

  _nb_vignettes = read_high_endian_int(picture_is);
  _width = read_high_endian_int(picture_is);
  _height = read_high_endian_int(picture_is);

  ifstream label_is(label_file_name);
  if(label_is.fail()) {
    cerr << "Can not open file [" << label_file_name << "].\n";
    exit(1);
  }

  magic = read_high_endian_int(label_is);
  if(magic != mnist_labels_magic) {
    cerr << "Invalid magic for labels, file"
         << " [" << label_file_name << "] number [" << magic << "]\n";
    exit(1);
  }

  int nb_vignettes_labels = read_high_endian_int(label_is);

  if(nb_vignettes_labels != _nb_vignettes) {
    cerr << "Inconsistency between the number of pictures in"
         << " [" << picture_file_name << "] (" << _nb_vignettes << ")"
         << " and the number of labels in"
         << " [" << label_file_name << "] (" << nb_vignettes_labels << ").\n";
    exit(1);
  }

  PixelMaps *pm = new PixelMaps(_nb_vignettes * _width * _height);
  _pixel_maps = pm->add_ref();
  _pixels = new unsigned char *[_nb_vignettes];
  _labels = new unsigned char[_nb_vignettes];

  picture_is.read((char *) _pixel_maps->_core,
                  _nb_vignettes * _width * _height);
  label_is.read((char *) _labels, _nb_vignettes);

  for(int i = 0; i < _nb_vignettes * _width * _height; i++) {
    _pixel_maps->_core[i] = 255 - _pixel_maps->_core[i];
  }

  _nb_classes = 0;
  for(int n = 0; n < _nb_vignettes; n++) {
    _pixels[n] = _pixel_maps->_core + n * _width * _height;
    if(_labels[n] > _nb_classes) _nb_classes = _labels[n];
  }
  _nb_classes++;
}

void VignetteSet::bootstrap(int nb, VignetteSet *vs) {
  free();
  _nb_vignettes = nb;
  _width = vs->_width;
  _height = vs->_height;
  _nb_classes = vs->_nb_classes;
  _pixel_maps = vs->_pixel_maps->add_ref();
  _pixels = new unsigned char *[_nb_vignettes];
  _labels = new unsigned char[_nb_vignettes];
  for(int n = 0; n < _nb_vignettes; n++) {
    int m = int(drand48() * vs->_nb_vignettes);
    _pixels[n] = vs->_pixels[m];
    _labels[n] = vs->_labels[m];
  }
}
