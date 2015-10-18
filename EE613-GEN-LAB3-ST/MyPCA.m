function [Lambda,U,meanX]=MyPCA(X)
%%
% Performs the extraction of the PCA components given a dataset 
% Input:    X, NxD matrix of N points x of dimension D 
% Output:   
%    Lambda : set of eigenvalues of the covariance matrix ranked in decreasing order
%    U      : matrix of eigenvectors (one column per eigenvector, ranked in the same order than eigenvalues)
%    meanX  : average value of the datas in X (row vector of size D)
% 

meanX = mean(X, 1);  % avg value for each sample

X_centered = bsxfun(@minus, X, meanX);

[~, S, U] = svd(X_centered);

Lambda = diag(S);