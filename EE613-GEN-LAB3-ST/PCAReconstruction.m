function [Ztilde]=PCAReconstruction(Y,meanX,P)
%%
% Reconstructs data points given their coordinates Y in the space spanned by the M eigenvectors of P
% Input:
%    Y     : NxM  coordinates in low M dimensional space of the N points to (re)construct 
%    meanX : mean of data points provided by MyPCA (one row of D values)
%    P     : DxM projection matrix containing the first M eigenvectors obtained from MyPCA
% Output:   
%    Ztilde     : NxD matrix containing the constructed vectors 
% 

Z_temp = Y * P';
Ztilde = bsxfun(@plus, Z_temp, meanX);