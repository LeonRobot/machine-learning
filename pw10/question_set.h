
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

#ifndef QUESTION_SET_H
#define QUESTION_SET_H

#include "vignette_set.h"

class QuestionSet {
public:
  // How many questions are there in that set
  virtual int nb_questions() = 0;
  // Response of question n_question on the vignette n_vignette of the
  // provided set
  virtual bool response(int n_question,
                        VignetteSet *vs, int n_vignette) = 0;
};

#endif
