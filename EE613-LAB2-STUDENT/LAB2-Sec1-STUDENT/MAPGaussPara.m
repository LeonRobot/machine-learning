function [mu,Sigma]=MAPGaussPara(X,para)
%%
% Performs the MAP estimation of a multivariate gaussian parameter using normal-wishart prior
%
%  See homework text 
%
% Input: X: data points
%        para: structure containing the wishart parameters.
%           by default:
%           1. para.tau=100; 
%           2. para.m=  [-10 -10]';
%           3. para.nu=  100;
%           4. SigmaPrior=eye(2)*10; 
%           5. para.V=SigmaPrior*100;
%
% Output: mu,Sigma: estimated gaussian parameters


%% To be completed....

mu_samp = transpose(mean(X, 1));
Sigma_samp = cov(X);
[N, d] = size(X);

mu = (para.tau .* para.m + N .* mu_samp) ./ (para.tau + N);
Sigma = (para.V + N .* Sigma_samp + (para.tau .* N ./ (para.tau + N)) .* (para.m - mu_samp) * transpose(para.m - mu_samp)) ./ ... 
    (para.nu - d + N);

