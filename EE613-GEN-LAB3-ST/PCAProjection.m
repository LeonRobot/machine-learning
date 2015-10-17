function [Y]=PCAProjection(Z,meanX,P)
%%
% Projects a matrix of data points Z on the first M eigenvectors
% Input:
%    Z     : NxD data matrix (N rows of data zi of dimension D)
%    meanX : mean of data points provided by MyPCA (row of D dimensions)
%    P     : DxM projection matrix containing the first M eigenvectors obtained from MyPCA
% Output:   
%    Y     : NxM matrix containing the components in PCA subspace for all data points of Z
% 

% X_centered = bsxfun(@minus, X, meanX);
Z_centered = bsxfun(@minus, Z, meanX);

Y = Z_centered * P;