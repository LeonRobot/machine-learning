
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

#include "pixel_question_set.h"

PixelQuestionSet::PixelQuestionSet(int w, int h) {
  _vignette_width = w;
  _vignette_height = h;
}

int PixelQuestionSet::nb_questions() {
  return _vignette_width * _vignette_height;
};

bool PixelQuestionSet::response(int n_question,
                                VignetteSet *vs, int n_vignette) {
  return vs->pixel(n_vignette,
                   n_question%_vignette_width,
                   n_question/_vignette_width) > 128;
}

