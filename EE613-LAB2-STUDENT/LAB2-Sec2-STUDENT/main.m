%%%%%%%%%%%%%%%%%%%%
clear all;
close all;


%% --------------------------------------------------
% get the different data 
data=load('Training.mat');
xtrain=5*data.xtrain;
ztrain=data.ztrain;


data=load('TestPerson1.mat');
x1=5*data.x1;
z1=data.z1;

data=load('TestPerson2.mat');
x2=5*data.x2;
z2=data.z2;

%% --------------------------------------------------
%% Select a trainign option

option=2;
switch(option)
    case 1
        disp('option 1: learn with the training data')
        [piLabel,means, SigmaLabel]=GMMLabeledObservation(3,xtrain',ztrain');

    case 2
        disp('option 2: learn with a GMM')

        % means from training data will be needed 
        % to reorder the means estimated by unsupervised EM
        %
        [piLabel,means, SigmaLabel]=GMMLabeledObservation(3,xtrain',ztrain)
        % waitforbuttonpress

        para.niter=10; % number of EM iteration, should be sufficient
        
        % show x1 is GMM
        x1trans = x1';
        plot(x1trans(:, 1), x1trans(:, 2), '.')
        
        % show x2 is gmm
        x2trans = x2';
        plot(x2trans(:, 1), x2trans(:, 2), '.')
        
        
        [pi1,mu1,Sigmaem1]=GMMEM(x1',3,para)
%         pi1_old=pi1;
%         mu1_old=mu1;
%         Sigmaem1_old=Sigmaem1;
        [pi1,mu1,Sigmaem1]=Reorder(pi1,mu1,Sigmaem1,means)
        % mu1 = mu1';
        
        [pi2,mu2,Sigmaem2]=GMMEM(x2',3,para);
        [pi2,mu2,Sigmaem2]=Reorder(pi2,mu2,Sigmaem2,means);
        % mu2 = mu2';
    case 3
        disp('option 3: learn with MAP adaptation')
        [piLabel,means, SigmaLabel]=GMMLabeledObservation(3,xtrain',ztrain'); % parameters learned supervisely are needed to set the priors.
        %parameter initialisation Some default values are provided, others need to be set, 
        % feel free to play with them (increase/lowering priors depending of what you want to enhance)
        % to observe the behaviours.
        para.K=3; %number of component
        para.Dimp=2; % dimension


        %%% =============================================
        %%  Handling person 1 
        %%
        % dirichlet prior
        % para.priormixcoef= % alpha coefficients for the Dirichlet prior on the pi coefficient 
        % (to be set)
%         para.priormixcoef = (1 / para.K) .* ones(3, 1);
        para.priormixcoef = [0.3060; 0.2694; 0.4246];

        % normal wishart prior

        % parameters for the mean
        % to give on average (for each component) the same weight to the prior and to the observation
        para.tau=length(x1)/para.K; % tau parameter for the normal prior on the mean
        para.priorm= [-26.3560 14.0723 0.9330; 5.7972 4.8853 -11.3125];  % from estimates of option2

        % parameters for the variance
        para.d=length(x1)/para.K; % nu parameter of the Inverse Wishart distribution
        % para.V=% param of the Inverse Wishart distribution 
        sigma0 = zeros(2,2,para.K);
        sigma0(:,:,1) = [83.3 18.5056; 18.5056 128.2860];
        sigma0(:,:,2) = [104.3769 2.6030; 2.6030 195.5050];
        sigma0(:,:,3) = [139.1009 -29.2664; -29.2664 137.2115];
        para.V = sigma0 * (para.d - para.Dimp - 1);
        % to be set recall that V = sigma0 * (d-Dimp -1) with sigma0 the expectation of the prior covariance
        %

        para.niter=10; % number of EM iteration, should be sufficient
        [pimap1, moymap1, sigmamap1] = GMMadapt (x1',para.K,para);

        %%% =============================================
        %%% =============================================
        %%  Handling person 2 
        %%
	%%   same process for person 2 
	%%  (so the same prior on mean and variance should be used)
        %% 
        %%
        % dirichlet prior
        % para.priormixcoef= % alpha coefficients for the Dirichlet prior on the pi coefficient
        % (to be set)

        % normal wishart prior

        % parameters for the mean
        % to give on average (for each component) the same weight to the prior and to the observation
        para.tau=length(x2)/para.K; % tau parameter for the normal prior on the mean
        % para.priorm= % prior mean m for each gaussian  (to be set )

        % parameters for the variance
        para.d=length(x2)/para.K; % nu parameter of the Inverse Wishart distribution
        % para.V=% param of the Inverse Wishart distribution 
        % to be set recall that V = sigma0 * (d-Dimp -1) with sigma0 the expectation of the prior covariance
        %

        para.niter=10; % number of EM iteration, should be sufficient
        [pimap2, moymap2, sigmamap2] = GMMadapt (x2',para.K,para);


end

% 4. draw the ellipses from the learnt parameters

%% Here we assume that the ellipses obtained from training are always there
figtrain=figure;
hold on; 
Plot_GM(xtrain',3,piLabel,means,SigmaLabel,'-b',1)
plot(xtrain(1,ztrain==1),xtrain(2,ztrain==1),'+b')
plot(xtrain(1,ztrain==2),xtrain(2,ztrain==2),'+k')
plot(xtrain(1,ztrain==3),xtrain(2,ztrain==3),'+g')
title('person training')

% % compute models for person 1 and 2 using ground true and draw their ellipses. 
% Can be usefull for studiyng the comparisons with the estimated models in option 2 and 3
%
%[mixcoef1, means1, sigma1]=GMMLabeledObservation(3,x1,z1);
%figure;
%hold on;
%Plot_GM(x1',3,mixcoef1, means1, sigma1,'-b',1)  
%plot(x1(1,z1==1),x1(2,z1==1),'+b')
%plot(x1(1,z1==2),x1(2,z1==2),'+k')
% plot(x1(1,z1==3),x1(2,z1==3),'+g')
% title('person 1')
% 
% [mixcoef2, means2, sigma2]=GMMLabeledObservation(3,x2,z2);
% figure;
% Plot_GM(x2',3,mixcoef2, means2, sigma2,'-b',1)
% plot(x2(1,z2==1),x2(2,z2==1),'+b')
% plot(x2(1,z2==2),x2(2,z2==2),'+k')
% plot(x2(1,z2==3),x2(2,z2==3),'+g')
% title('person 2')


%%for each option: 
% accuracy computation
% visualization of the estimated parameters if any

if option==1
    fpred = predictLabel(piLabel,means,SigmaLabel,x1);
    disp('accuracy for person 1 supervised')
    mean(z1==fpred')

    fpred = predictLabel(piLabel,means,SigmaLabel,x2);
    disp('accuracy for person 2 supervised case')
    mean(z2==fpred')
end


if option == 2
    figpers1=figure;
    % if model computed: 
    %Plot_GM(x1',3,mixcoef1, means1, sigma1,'-r',1) %plot the models estimated using the ground truth for comparison purposes.
    figure(figpers1);
    hold on;
    Plot_GM(x1',3,pi1,mu1,Sigmaem1,'-b',1)
%     Plot_GM(x1', 3, pi1_old, mu1_old', Sigmaem1, '-r', 1)
    title('person 1 and ML EM (with reordering)')
    fpred1 = predictLabel(pi1,mu1,Sigmaem1,x1);
%     fpred1_old = predictLabel(pi1_old, mu1_old, Sigmaem1_old, x1);
    disp('accuracy for person 1, ML EM plus reordering')
    mean(z1==fpred1')
    mu1
    
    figpers2=figure;
    % if model computed
    %Plot_GM(x2',3,mixcoef2, means2, sigma2,'-r',1)  %plot the models estimated using the ground truth for comparison purposes.
    figure(figpers2);
    hold on;
    Plot_GM(x2',3,pi2,mu2,Sigmaem2,'-b',1)
    title('person2 and ML EM (with reordering)')
    fpred2 = predictLabel(pi2,mu2,Sigmaem2,x2);
    disp('accuracy for person 2, ML EM plus reordering')
    mean(z2==fpred2')
    mu2
end

if option == 3
    figpers1=figure;
    %Plot_GM(x1',3,mixcoef1, means1, sigma1,'-r',1)
    figure(figpers1);
    hold on;
    
    Plot_GM(x1',3,pimap1,moymap1,sigmamap1,'-b',1)
    title('person 1 and map EM')
    fpred1 = predictLabel(pimap1,moymap1,sigmamap1,x1);
    disp('accuracy for person 1 map EM')
    mean(z1==fpred1')

    figpers2=figure;
    %Plot_GM(x2',3,mixcoef2, means2, sigma2,'-r',1)
    figure(figpers2);
    hold on;
    Plot_GM(x2',3,pimap2,moymap2,sigmamap2,'-b',1)
    title('person2 and map EM')
    fpred2 = predictLabel(pimap2,moymap2,sigmamap2,x2);
    disp('accuracy for person 2 map EM')
    mean(z2==fpred2')
end

