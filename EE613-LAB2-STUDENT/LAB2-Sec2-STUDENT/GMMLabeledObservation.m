function [pi,mu, GMMsigma]=GMMLabeledObservation(K,X,Z);
%%
% Performs  Maximum Likelihood estimation of the GMM parameters given labels
% Input:
% K : number of mixtures
% X : (N x DimX) matrix of data points of DimX, with N number of points
% Z : column vector of size N containing the component indices of X
% Output: pi,mu,GMMsigma: mixture weights and 
%          estimated gaussian parameters for each of the components

pi = zeros(K,1);
mu = zeros(size(X,2), K);
GMMsigma = zeros(size(X,2), size(X,2));

N_total = length(X);

for i=1:K,
   X_i = X(find(ismember(Z, i)), :);
   pi(i) = length(X_i) / N_total;
   [mu_i, sigma_i] = MLGaussPara(X_i);
   mu(:,i) = mu_i';
   GMMsigma(:,:,i) = sigma_i;
   % [mu(i,:), GMMsigma(s_i_end:s_i_end, s_i_start:s_i_end)] = MLGaussPara(X_i);
   % [mu(i,:), GMMsigma(s_i_end:s_i_end, s_i_start:s_i_end)] = MLGaussPara(X_i);
end

