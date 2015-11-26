function demo_GPR01_copy
% Gaussian process regression (GPR) 
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
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
nbVar = 2; %Dimension of datapoint (t,x1,x2)
nbData = 4; %Number of datapoints
nbDataRepro = 100; %Number of datapoints for reproduction
nbSamples = 1; %Number of demonstrations
p(1)=1E0; p(2)=1E-1; p(3)=0E-2; %GPR parameters


%% Load handwriting data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
demos=[];
load('data/2Dletters/G.mat');

npts = 4;
xmin = 0;
xmax = 10;

x=linspace(xmin, xmax, npts);
y=rand(1, 4);

Data=[x; y];

for n=1:nbSamples
	s(n).Data = Data;
end
subplot(2, 2, 1)
scatter(x, y)
ylim([min(y) / 2, max(y) * 2]);
xlim([min(x), max(x)])

%GPR precomputation
xIn = Data(1,:);
xOut = Data(2:end,:);
M = pdist2(xIn', xIn');
K = p(1) * exp(-p(2)^-1 * M.^2);
invK = pinv(K + p(3) * eye(size(K))); 

%Sample from prior distribution with mean 0
prior = mvnrnd(zeros(1, npts), K, 100);

% show my prior
subplot(2, 2, 2)
hold on
for i=1:size(prior, 1)
    plot(x, prior(i, :), 'color', rand(1,3))
end

% Sample from prior distribution get smoother curves
n_pts_more=100;
x_more = linspace(xmin, xmax, n_pts_more);
M_more = pdist2(x_more', x_more');
K_more = p(1) * exp(-p(2)^-1 * M_more.^2);

prior_more = mvnrnd(zeros(1, n_pts_more), K_more, 20);

% show my prior finer detail
subplot(2, 2, 4)
hold on
for i=1:size(prior_more, 1)
    plot(x_more, prior_more(i, :), 'color', rand(1,3))
end

% calculate distance matrices for my xstar
x_star = linspace(xmin, xmax, n_pts_more);
M_star = pdist2(x_star', x_star');
K_star = p(1) * exp(-p(2)^-1 * M_star.^2);
M_K_Kstar = pdist2(x_star', x');
K_K_star = p(1) * exp(-p(2)^-1 * M_K_Kstar.^2);

% add prior for noisy observations
K_star_tilde = K_star + p(3) * eye(size(K_star));
K_tilde = K + p(3) * eye(size(K));

mu_star = K_K_star * inv(K_tilde) * y';
sigma_star = K_star_tilde - K_K_star * inv(K_tilde) * K_K_star';

posterior = mvnrnd(mu_star, diag(diag(sigma_star)), 1);
subplot(2,2,3)
hold on
scatter(x, y, 'blue')
for i=1:size(posterior, 1)
   plot(x_star, posterior(i, :), 'color', rand(1,3))
end
ylim([min(y) / 2, max(y) * 2]);
xlim([min(x), max(x)]) 
hold off
% 
% 
% % calculate distance matrices
% nptsstar = 120;
% xstar = linspace(xmin, xmax, nptsstar);
% Mstar = pdist2(xstar', xstar');
% Kstar = p(1) * exp(-p(2)^-1 * Mstar.^2);
% M_KKstar = pdist2(xstar', xIn');
% KKstar = p(1) * exp(-p(2)^-1 * M_KKstar.^2);
% M_KstarK = pdist2(xIn', xstar');
% KstarK = p(1) * exp(-p(2)^-1 * M_KstarK.^2);
% 
% mustar = KKstar * invK * xOut';
% covstar = (Kstar + p(3) * eye(size(Kstar)))  - KKstar * invK * KstarK;
% 
% % show my posterior
% posterior = mvnrnd(mustar, diag(diag(covstar)), 100);
% 
% subplot(2, 2, 3)
% hold on
% for i=1:size(posterior, 1)
%     plot(xstar, posterior(i, :), 'color', rand(1,3))
% end
% ylim([min(y) / 2, max(y) * 2])
% xlim([min(x), max(x)])

