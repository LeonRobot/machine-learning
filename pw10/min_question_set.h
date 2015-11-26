
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

#ifndef MIN_QUESTION_SET_H
#define MIN_QUESTION_SET_H

#include "question_set.h"

class MinQuestionSet : public QuestionSet {
  int _vignette_width, _vignette_height;
public:
  MinQuestionSet(int w, int h);
  virtual int nb_questions();
  virtual bool response(int n_question,
                        VignetteSet *vs, int n_vignette);
};

#endif
