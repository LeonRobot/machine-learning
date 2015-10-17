function [Xreconstructed,ProjectionVect,ProjectedCoord,MeanX]=PpcaMissingValues(X,NbDim)

%
% Performs PPCA analysis. Returns:
% - Xreconstructed: the inputs with reconstructed missing values
% - ProjectionVect  : ProjectionVectors that form an orthonormal basis of
%   the ppca space of dimension NbDim
% - ProjectedCoord: the coordinates of each sample in the ProjectionVect
%   basis
%   

opt = statset('ppca');
opt.TolFun  = 1.;
opt.MaxIter = 50;

[ProjectionVect,ProjectedCoord,VarPrincipalComponents,MeanX,v,S] = ppca(X,NbDim,'Options',opt);
Xreconstructed=S.Recon;

end
