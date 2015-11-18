
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

#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include "classifier.h"
#include "question_set.h"

class DecisionTree : public Classifier {
  QuestionSet *_question_set;
  int _question_index;
  DecisionTree *_subtree_yes, *_subtree_no;
  int _label;
  int _depth_max;
  int _nb_min_samples_for_split;
  int _nb_questions_for_optimization;
  void train(VignetteSet *train_set, int nb, int *indexes);
public:
  DecisionTree(QuestionSet *question_set,
               int depth_max,
               int nb_min_samples_for_split,
               int nb_questions_for_optimization);
  virtual ~DecisionTree();
  virtual void train(VignetteSet *train_set);
  virtual int predict(VignetteSet *vs, int n_vignette);
};

#endif
