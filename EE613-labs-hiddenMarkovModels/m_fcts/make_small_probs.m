function [X] = make_small_probs(X, prob)
indx = find(X == 0);
X(indx) = prob;
