function plotHSMM(Trans, StatesPriors, Pd)
% Plot the transition graph and the state duration distribution of an HSMM
%
% Writing code takes time. Polishing it and making it available to others takes longer! 
% If some parts of the code were useful for your research of for a better understanding 
% of the algorithms, please reward the authors by citing the related publications, 
% and consider making your own research available in this way.
%
% @article{Calinon15,
%   author="Calinon, S.",
%   title="A Tutorial on Task-Parameterized Movement Learning and Retrieval",
%   journal="Intelligent Service Robotics",
%   year="2015"
% }
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

nbStates = size(Trans,1);
nbD = size(Pd,2);
valAlpha = 1;
szPd = .5;
graphRadius = 1;
nodeRadius = .2;
nodePts = 40;
%nodeAngle = linspace(pi,2*pi+pi,nbStates+1);
nodeAngle = linspace(pi/2,2*pi+pi/2,nbStates+1);
for i=1:nbStates
	nodePos(:,i) = [cos(nodeAngle(i)); sin(nodeAngle(i))] * graphRadius;
end
clrmap = lines(nbStates);
ff = .8 / max(max(Trans-diag(diag(Trans))));

for i=1:nbStates
	%Plot StatesPriors 
	posTmp = [cos(nodeAngle(i)); sin(nodeAngle(i))] * graphRadius + ...
		[cos(nodeAngle(i)+pi/3); sin(nodeAngle(i)+pi/3)] * nodeRadius*2;
	dirTmp = nodePos(:,i) - posTmp;
	dirTmp = (norm(dirTmp)-nodeRadius) * dirTmp/norm(dirTmp);
	plot2DArrow(posTmp, dirTmp, max([.8 .8 .8]-StatesPriors(i)*.8,0)); 
	
	for j=i+1:nbStates
		%Plot Trans
		dirTmp = nodePos(:,j)-nodePos(:,i);
		dirTmp = (norm(dirTmp)-2*nodeRadius) * dirTmp/norm(dirTmp);
		offTmp = [dirTmp(2); -dirTmp(1)] / norm(dirTmp);
		posTmp = nodePos(:,i) + nodeRadius * dirTmp/norm(dirTmp) + offTmp*0.05;
		plot2DArrow(posTmp, dirTmp, max([.8 .8 .8]-Trans(i,j)*ff,0)); 
	end
	for j=1:i
		%Plot Trans
		dirTmp = nodePos(:,j)-nodePos(:,i);
		dirTmp = (norm(dirTmp)-2*nodeRadius) * dirTmp/norm(dirTmp);
		offTmp = [dirTmp(2); -dirTmp(1)] / norm(dirTmp);
		posTmp = nodePos(:,i) + nodeRadius * dirTmp/norm(dirTmp) + offTmp*0.05;
		plot2DArrow(posTmp, dirTmp, max([.8 .8 .8]-Trans(i,j)*ff,0)); 
	end
end

%Plot nodes
for i=1:nbStates
	a = linspace(0,2*pi,nodePts);
	meshTmp = [cos(a); sin(a)] * nodeRadius + repmat(nodePos(:,i),1,nodePts);
	patch(meshTmp(1,:), meshTmp(2,:), clrmap(i,:),'edgecolor',clrmap(i,:)*0.5, 'facealpha', valAlpha,'edgealpha', valAlpha);
	text(nodePos(1,i),nodePos(2,i),num2str(i),'HorizontalAlignment','center','FontWeight','bold','fontsize',16);
end

%Plot Pd
for i=1:nbStates
	posTmp = [cos(nodeAngle(i)); sin(nodeAngle(i))] * graphRadius * 1.6;
	yTmp = Pd(i,:) / max(Pd(i,:));
	meshTmp = ([[0, linspace(0,1,nbD), 1]; [0, yTmp, 0]]-0.5) * szPd + repmat(posTmp,1,nbD+2);
	patch(meshTmp(1,:), meshTmp(2,:), clrmap(i,:), 'edgecolor',clrmap(i,:)*0.5, 'facealpha', valAlpha,'edgealpha', valAlpha);
	meshTmp = ([[0, 0, 1]; [1, 0, 0]]-0.5) * szPd + repmat(posTmp,1,3);
	plot(meshTmp(1,:), meshTmp(2,:), 'color',[0 0 0]);
end

axis equal;
