addpath('./m_fcts/');

N=500;
D=3;
d=2;
K=2;
pi=[0.3 0.7];

X = generateSparseData(N, D, d, K, pi);

figure
scatter3(X(:,1), X(:,2), X(:,3),10*ones(size(X, 1), 1),X(:,4))
view(-30,30)

% view one cluster
indx = find(X(:,4) == 1);
x1 = X(indx,1);
y1 = X(indx,2);
z1 = X(indx,3);
col1 = X(indx,4);
figure
scatter3(x1, y1, z1,10*ones(size(x1, 1), 1),'black')
view(-30,30)