function [pimap, moymap, sigmaMAP] = GMMadapt (X,K,para)
%%
%  [pimap, moymap, sigmaMAP] = GMMadapt (X,K,para)
%  Perform Map adaptation of the GMM parameters using the EM algorithm
%  Inputs:
%    X : (N x DimX) matrix of data points of DimX, with N number of data points
%    para: parameters for the priors
% Output: 
% pimap,mumap,sigmaMAP: mixture weights abd estimated gaussian parameters for each of the components


%%initialisation
pimap=para.priormixcoef;
moymap=para.priorm';
sigmaMAP=para.V/(para.d-para.Dimp-1);

niter=para.niter;
for i=1:niter
    %%E-step equivalent to classical EM
    
    %%Mstep M step from EM + adaptation

end
