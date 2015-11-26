function [ X ] = generateSparseData(N, D, d, K, p)
%generateSparseData: generate sparse data exercise 1
%   N: number of datapoints
%   D: dimension of each datapoint
%   d: subspace in which datapoints lie
%   K: number of clusters
%   p: proportion for each cluster (1 x K vector)

% init center of clusters: uniformally distributed within a hypercube of
% unit size: assume independent
rng(0);
principal_length = 1/K * ones(1, d);
noise_scale = 0.01;

centers = rand(K, D);
% directions are random
dirs_all = zeros(D, D, K);
for k=1:K
    % begin with principal dir
    principal_dirs = rand(D, 1);
    % normalize directions
    principal_dirs = principal_dirs / norm(principal_dirs);
    dirs_all(:, 1, k) = principal_dirs;
    dirs_all(:, 2:D, k) = null(principal_dirs');
end

% take first d eigenvectors
dirs_proj = dirs_all(:, 1:d, :);

% multiply by length
for k=1:K
    dirs_proj(:,:,k) = bsxfun(@times, principal_length, dirs_proj(:, :, k));
end

colors = zeros(N, 1);
for n=1:N
    % draw a cluster, weighted by p
    k = randsample(K, 1, true, p);
    % generate a point
    muk = centers(k, :);
    x = mvnrnd(zeros(1, 2), eye(d));
    % project 2d to 3d
    x_proj = dirs_proj(:,:,k) * x';
    x_proj = x_proj';
    % add noise and mean
    x_proj = x_proj + muk + (noise_scale * randn(1, D));
    X(n,:) = x_proj;
    colors(n, 1) = k;
end
X = [X, colors];

end

