function demo_LS_polFit01
% Polynomial fitting with least squares 
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
nbVarIn = 4; %Dimension of input vector
nbVarOut = 1; %Dimension of output vector
nbData = 50; %Number of datapoints


%% Generate data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
A0 = [-.2; -.2; .2; 1]; %Polynomimal relationship between input and output (to be estimated)
x = linspace(-1,1,nbData)';
X=[];
for i=0:nbVarIn-1
	X = [X, x.^i]; %-> X=[1, x, x.^2, x.^3]
end
Y = X * A0 + randn(nbData,nbVarOut)*2E-2; %Output data (with noise)

%Array used to display more points
xp = linspace(min(x),max(x),200)';
Xp = [];
for i=0:nbVarIn-1
	Xp = [Xp, xp.^i];
end


%% Regression
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
A = X'*X \ X' * Y;
%Compute fitting error
e = norm(Y-X*A);

%% Ridge regression
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Gamma = eye(nbVarIn)*10^LambdaFac(lst);
A = (X'*X + Gamma) \ X' * Y;

% %Ridge regression with augmented matrices (ch.7.5.2 from Kevin Murphy's book)
% Xa = [X; chol(Gamma)];
% Ya = [Y; zeros(nbVarIn,1)]; 
% A2 = (Xa'*Xa) \ Xa' * Ya;
% norm(A-A2)
% %A2 = (X'*X) \ X' * Y;


%% Plot
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure('position',[10,10,1300,700]); hold on;
title(['Least squares - Polynomial fit of degree ' num2str(nbVarIn-1) ' (e=' num2str(e,'%.2f') ')'],'fontsize',12);
i = 2; %Dimension to display for input data
j = 1; %Dimension to display for output data
for t=1:nbData
	plot([X(t,i) X(t,i)], [Y(t,j) X(t,:)*A(:,j)], '-','linewidth',1,'color',[0 0 0]);
	plot(X(t,i), Y(t,j), '.','markersize',14,'color',[0 0 0]);
end
plot(Xp(:,i), Xp*A(:,j), 'r-','linewidth',2);
xlabel('x_1'); ylabel('y_1');
axis([min(X(:,i)) max(X(:,i)) min(Y(:,j))-0.1 max(Y(:,j))+0.1]);

%print('-dpng','graphs/demo_LS_polFit01.png');
%pause;
%close all;
