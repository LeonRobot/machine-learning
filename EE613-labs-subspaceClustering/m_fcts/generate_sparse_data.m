function [ X ] = generateSparseData(N, D, d, K, p)
%generateSparseData: generate sparse data exercise 1
%   N: number of datapoints
%   D: dimension of each datapoint
%   d: subspace in which datapoints lie
%   K: number of clusters
%   p: proportion for each cluster (1 x K vector)

% init center of clusters: uniformally distributed within a hypercube of
% unit size: assume independent
rng(0)
principal_length = 1/K;
centers = rand(K, D);
% directions are random
principal_dirs = rand(K, D);
% normalize directions
dir_lengths = sqrt(sum(principal_dirs.^2, 2));
principal_dirs = bsxfun(@times, 1./dir_lengths, principal_dirs);

% lengths are 1/K for visualization purposes
principal_dirs = principal_length .* principal_dirs;

sigmas = zeros(D, D, K);
for k=1:K
    sig_temp = sigmas(:,:,k);
    sig_temp(logical(eye(size(sig_temp)))) = principal_length;
    sigmas(:,:,k) = sig_temp;
    sigmas(D, D, k) = 0;
end

X = zeros(N, D);
for n=1:N
    % draw a cluster, weighted by p
    k = randsample(K, 1, true, p);
    % generate a point
    muk = centers(k, :);
    sigmak = sigmas(:,:,k);
    x = mvnrnd(muk,sigmak);
    X(n,:) = x;
end

end

