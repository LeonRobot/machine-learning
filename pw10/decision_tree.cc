
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

#include "misc.h"
#include "decision_tree.h"

DecisionTree::DecisionTree(QuestionSet *question_set,
                           int depth_max,
                           int nb_min_samples_for_split,
                           int nb_questions_for_optimization) {
  _question_set = question_set;
  _depth_max = depth_max;
  _nb_min_samples_for_split = nb_min_samples_for_split;
  _nb_questions_for_optimization = nb_questions_for_optimization;
}

DecisionTree::~DecisionTree() {
  if(_subtree_yes) delete _subtree_yes;
  if(_subtree_no)  delete _subtree_no;
}

void DecisionTree::train(VignetteSet *train_set,
                         int nb_samples, int *indexes) {

  int nb_classes = train_set->nb_classes();

  int nb_per_class[nb_classes];

  for(int c = 0; c < nb_classes; c++) {
    nb_per_class[c] = 0;
  }

  for(int s = 0; s < nb_samples; s++) {
    nb_per_class[train_set->label(indexes[s])]++;
  }

  _label = 0; // Note that this procedure keep 0 in case of tie

  for(int c = 1; c < nb_classes; c++) {
    if(nb_per_class[c] > nb_per_class[_label]) {
      _label = c;
    }
  }

  _subtree_yes = 0;
  _subtree_no  = 0;

  if(_depth_max > 0) {

    int nb_per_class_0[nb_classes], nb_per_class_1[nb_classes];

    int n0, n1, best_n0 = 0, best_n1 = 0;

    scalar_t entropy, best_entropy = -1;
    int candidate_question_index;

    for(int q = 0; q < _nb_questions_for_optimization; q++) {
      candidate_question_index = int(drand48() * _question_set->nb_questions());

      for(int l = 0; l < nb_classes; l++) {
        nb_per_class_0[l] = 0;
        nb_per_class_1[l] = 0;
      }

      n0 = 0;
      n1 = 0;

      for(int s = 0; s < nb_samples; s++) {
        if(_question_set->response(candidate_question_index,
                                   train_set, indexes[s])) {
          n1++;
          nb_per_class_1[train_set->label(indexes[s])]++;
        } else {
          n0++;
          nb_per_class_0[train_set->label(indexes[s])]++;
        }
      }

      entropy = 0;
      if(n0 > 0) {
        entropy += discrete_entropy(nb_per_class_0, nb_classes) * scalar_t(n0);
      }
      if(n1 > 0) {
        entropy += discrete_entropy(nb_per_class_1, nb_classes) * scalar_t(n1);
      }
      entropy /= scalar_t(n0 + n1);

      if(best_entropy < 0 || entropy < best_entropy) {
        best_entropy = entropy;
        _question_index = candidate_question_index;
        best_n0 = n0;
        best_n1 = n1;
      }
    }

    if(best_n0 >= _nb_min_samples_for_split &&
       best_n1 >= _nb_min_samples_for_split) {

      int *tmp_indexes = new int[nb_samples];
      int a = 0, b = nb_samples;
      for(int s = 0; s < nb_samples; s++) {
        if(_question_set->response(_question_index,
                                   train_set, indexes[s])) {
          b--;
          tmp_indexes[b] = indexes[s];
        } else {
          tmp_indexes[a] = indexes[s];
          a++;
        }
      }

      _subtree_yes  = new DecisionTree(_question_set,
                                       _depth_max - 1,
                                       _nb_min_samples_for_split,
                                       _nb_questions_for_optimization);

      _subtree_no = new DecisionTree(_question_set,
                                     _depth_max - 1,
                                     _nb_min_samples_for_split,
                                     _nb_questions_for_optimization);

      _subtree_yes->train(train_set, nb_samples - a, tmp_indexes + a);

      _subtree_no->train(train_set, a, tmp_indexes);

      _label = -1;

      delete[] tmp_indexes;
    }
  }
}

void DecisionTree::train(VignetteSet *train_set) {
  int *indexes = new int[train_set->nb_vignettes()];
  for(int k = 0; k < train_set->nb_vignettes(); k++) {
    indexes[k] = k;
  }
  train(train_set, train_set->nb_vignettes(), indexes);
  delete[] indexes;
}

int DecisionTree::predict(VignetteSet *vs, int n_vignette) {
  if(_label >= 0) {
    return _label;
  } else {
    if(_question_set->response(_question_index,
                               vs, n_vignette)) {
      return _subtree_yes->predict(vs, n_vignette);
    } else {
      return _subtree_no->predict(vs, n_vignette);
    }
  }
}
