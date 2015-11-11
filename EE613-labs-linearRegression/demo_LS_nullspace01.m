function demo_LS_nullspace01
% Multivariate ordinary least squares with nullspace projection.
% 
% Copyright (c) 2015 Idiap Research Institute, http://idiap.ch/
% Written by Sylvain Calinon, http://calinon.ch/
% 
% This file is part of PbDlib, http://www.idiap.ch/software/pbdlib/
% 
% PbDlib is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License version 3 as
% published by the Free Software Foundation.
% 
% PbDlib is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with PbDlib. If not, see <http://www.gnu.org/licenses/>.


%% Parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
nbVarIn = 5; %Dimension of input vector
nbVarOut = 2; %Dimension of output vector
nbData = 3; %Number of datapoints


%% Generate data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
A0 = rand(nbVarIn,nbVarOut)-0.5; %Linear relation between input and output (to be estimated)
X = rand(nbData,nbVarIn); %Input data
Y = X * A0 + randn(nbData,nbVarOut)*5E-1; %Output data (with noise)


%% Regression with nullspace 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Least squares estimate
A = pinv(X) * Y;

%Nullspace projection operator - Version 1
N = eye(nbVarIn) - pinv(X)*X

%Nullspace projection operator - Version 2
[U,S,V] = svd(pinv(X));
sp = (sum(S,2)<1E-1); %Span of zero rows
N = U(:,sp) * U(:,sp)' %N = U * [0 0 0; 0 0 0; 0 0 1] * U'

%Estimate with optimization in the nullspace
A2 = pinv(X) * Y + N * ones(nbVarIn,nbVarOut) * 2;


%% Plot
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure('PaperPosition',[0 0 8 4],'position',[10,10,1300,600]); 
for i=1:nbVarIn
	for j=1:nbVarOut	
		subplot(nbVarOut,nbVarIn,(j-1)*nbVarIn+i); hold on;
		for t=1:nbData
			plot([X(t,i) X(t,i)], [Y(t,j) X(t,i)*A(i,j)], '-','linewidth',2,'color',[1 .7 .7]);
			plot([X(t,i) X(t,i)], [Y(t,j) X(t,i)*A2(i,j)], '-','linewidth',1,'color',[.7 1 .7]);
			plot(X(t,i), Y(t,j), '.','markersize',14,'color',[0 0 0]);
		end
		plot([0 1], [0 A(i,j)], '-','linewidth',2,'color',[.8 0 0]);
		plot([0 1], [0 A2(i,j)], '-','linewidth',2,'color',[0 .7 0]);
		xlabel(['x_' num2str(i)]); ylabel(['y_' num2str(j)]);
		%axis([0 1 -1 6]);
	end
end
legend(['||Y-X*A||=' num2str(norm(Y-X*A),'%.4f')], ['||Y-X*A2||=' num2str(norm(Y-X*A2),'%.4f')]);

%print('-dpng','-r600','graphs/demo_LS_nullspace01.png');
%pause;
%close all;
