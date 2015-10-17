function [MU_ML, W_ML, sigma_ML]=MyPPCA(X,M)
%%
% Performs the extraction of the Probabilistic PCA components given a
% dataset to a M dimensional subspace
% Input:    X, NxD matrix of N points x of dimension D ; 
%           M, subspace dimension
% Output:  
%    MU_ML : row vector of D columns
%    W_ML  : D rows and M column
%    sigma_ML : scalar, residual variance
% 

[Lambda,U,meanX] = MyPCA(X);

D = size(U,1);

MU_ML = meanX;

UM = U(:,1:M);

LM = diag(Lambda(1:M));

R = eye(M);

sigma_ML = (1/(D-M))*sum(Lambda(M+1:end));

SIGMA_ML_MAT = sigma_ML * eye(M);

W_ML = UM * (LM - SIGMA_ML_MAT)^0.5 * R;

