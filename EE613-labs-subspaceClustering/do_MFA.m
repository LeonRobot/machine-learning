addpath('./m_fcts/');

% Init my 2 2D clusters in 3D space
N=500;
D=3;
d=2;
K=2;
pi=[0.3 0.7];

X = generateSparseData(N, D, d, K, pi);

scatter3(X(:,1), X(:,2), X(:,3),50*ones(size(X, 1), 1),X(:,4))
h = rotate3d;
h.RotateStyle = 'box';
h.Enable = 'on';


%% Parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
model.nbStates = 2; %Number of states in the GMM
model.nbVar = 3; %Number of variables [x1,x2,x3,x4]
model.nbFA = 2; %Dimension of the subspace (number of factor analyzers)



%% Parameters estimation MFA
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure

Data = X(:,1:3)';
size(Data)
model = init_GMM_kmeans(Data, model);

model_MFA = EM_MFA(Data, model); 
plotGMM3D(model_MFA.Mu, model_MFA.Sigma, [0.7, 0.2, 0.9], 0.5, 1)

model_MPPCA = EM_MPPCA(Data, model); 
plotGMM3D(model_MPPCA.Mu, model_MPPCA.Sigma, [0.1, 0.2, 0.3], 0.5, 1)

model_HDGMM = EM_HDGMM(Data, model); 
plotGMM3D(model_HDGMM.Mu, model_HDGMM.Sigma, [0.4, 0.8, 0.6], 0.5, 1)
%% Parameters estimation MPPCA

