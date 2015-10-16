function [mu,Sigma]=MLGaussPara(X)
%%
% Perfors Maximum Likelihood estimation of gaussian parameters
% Input: X: data points
% Output:mu,Sigma: estimated gaussian parameters

%% To be completed....

mu = transpose(mean(X, 1));
Sigma = cov(X);
