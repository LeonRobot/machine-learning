clear all;
close all;

%%%% Generating some data
Dimp=2;
Np=100;
MeanData=[3 3]';
CovData=eye(2)*6;
X=genData1Gauss(Np,Dimp,MeanData,CovData);

%%  displaying the data points and the parameters (mean and standard
%%  deviation) of the gaussian used to generate the data
Plot_GM(X,1,1,MeanData,CovData,'b-')
hold on;

%% Example of use
[mu,Sigma]=MLGaussPara(X);
Plot_GM(X,1,1,mu,Sigma,'g-')


%% Default Normal-Wishart parameters
%% 
% for j=[0.005, 0.01, 0.1 10 25 50 75 100 200 500 1000],
% para.tau=100;
% para.tau = 100;
% para.tau=j;
% para.tau;
para.tau = Np;
para.m=  [-10 -10]';
para.nu=  100;
SigmaPrior=eye(2)*10; 
para.V=SigmaPrior*100;

% Plot Prior distribution
hold off
figure
Plot_GM(X, 1, 1, para.m, (1 / para.tau) .* CovData, 'r-')

hold on

[mumap,sigmamap]=MAPGaussPara(X,para);
Plot_GM(X,1,1,mumap,sigmamap,'r-')

axis([-20,20,-20,20])
% k = waitforbuttonpress;
% end

%% Plot Wishart prior
% mu_samp = transpose(mean(X, 1));
% Sigma_samp = cov(X);

% Plot_GM(X,1,1,MeanData,CovData,'b-')
% Plot_GM(X,1,1,MeanData,CovData,'b-')
% Plot_GM(X, 1, 1, transpose(para.tau), (1 / para.tau) .* CovData, 'b-')

% para.mnew = mumap;
% para.taunew = para.tau + Np;
% para.nunew = nu + Np;