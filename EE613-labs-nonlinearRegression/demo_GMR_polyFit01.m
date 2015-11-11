function demo_GMR_polyFit01
% Polynomial fitting of handwriting motion with multivariate GMR
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

addpath('./m_fcts/');


%% Parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
model.nbStates = 4; %Number of states in the GMM
model.nbVarIn = 2; %Dimension of input vector (degree of polynomial)
model.nbVarOut = 2; %Dimension of output vector
model.nbVar = model.nbVarIn + model.nbVarOut; %Number of variables (input+output)
nbData = 200; %Length of a trajectory
nbSamples = 5; %Number of demonstrations


%% Load handwriting data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
demos = [];
load('data/2Dletters/B.mat');
Data = [];
for n=1:nbSamples
	s(n).Data = spline(1:size(demos{n}.pos,2), demos{n}.pos, linspace(1,size(demos{n}.pos,2),nbData)); %Resampling
	Data = [Data s(n).Data]; 
end

%Transformation of input data into polynomial basis functions
x = repmat(linspace(1,200,nbData),1,nbSamples)';
X = [];
for i=1:model.nbVarIn
	X = [X, x.^i]; %-> X = [x, x.^2, x.^3]
end
DataPol = [X'; Data]; 


%% Learning and reproduction
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
model = init_GMM_kbins(DataPol, model, nbSamples);
model = EM_GMM(DataPol, model);

%Regression
tIn = linspace(min(x),max(x),nbData);
DataIn = [];
for d=1:model.nbVarIn
	DataIn = [DataIn; tIn.^d]; %-> X = [x, x.^2, x.^3]
end
[DataOut, SigmaOut, H] = GMR(model, DataIn, 1:model.nbVarIn, model.nbVarIn+1:model.nbVar); 


%% Plots
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure('PaperPosition',[0 0 16 4],'position',[10,10,1300,500],'color',[1 1 1]); 
xx = round(linspace(1,64,model.nbStates));
clrmap = colormap('jet')*0.5;
clrmap = min(clrmap(xx,:),.9);

%Spatial plot
axes('Position',[0 0 .2 1]); hold on; axis off;
plot(Data(1,:),Data(2,:),'.','markersize',8,'color',[.7 .7 .7]);
plot(DataOut(1,:),DataOut(2,:),'.','markersize',16,'linewidth',3,'color',[.8 0 0]);
axis square; axis equal;

%Timeline plot 
axes('Position',[.25 .58 .7 .4]); hold on; 
for n=1:nbSamples
	plot(tIn, Data(1,(n-1)*nbData+1:n*nbData), '-','linewidth',1,'color',[.7 .7 .7]);
end
[~,id] = max(H,[],1);
in = 1:model.nbVarIn; 
out = model.nbVarIn+1:model.nbVar;
for i=1:model.nbStates
	DataInTmp = [];
	for d=1:model.nbVarIn 
		DataInTmp = [DataInTmp; tIn(id==i).^d]; %Version 1
	end
	MuTmp = model.Mu(out(1),i) + model.Sigma(out(1),in,i)/model.Sigma(in,in,i) * (DataInTmp - repmat(model.Mu(in,i),1,sum(id==i)));
	plot(tIn(id==i), MuTmp, '.','linewidth',6,'markersize',26,'color',min(clrmap(i,:)+0.5,1));
end
plot(tIn, DataOut(1,:), '-','linewidth',2,'color',[.8 0 0]);
axis([min(tIn) max(tIn) min(Data(1,:))-1 max(Data(1,:))+1]);
ylabel('y_{t,1}','fontsize',16);

%Timeline plot of the basis functions activation
axes('Position',[.25 .12 .7 .4]); hold on; 
for i=1:model.nbStates
	patch([tIn(1), tIn, tIn(end)], [0, H(i,:), 0], min(clrmap(i,:)+0.5,1), 'EdgeColor', min(clrmap(i,:)+0.2,1), ...
		'linewidth',2,'facealpha', .4, 'edgealpha', .4);
end
axis([min(tIn) max(tIn) 0 1.1]);
xlabel('t','fontsize',16); 
ylabel('h_i(x_t)','fontsize',16);


%print('-dpng','graphs/demo_GMRpolyFit01.png');
%pause;
%close all;
