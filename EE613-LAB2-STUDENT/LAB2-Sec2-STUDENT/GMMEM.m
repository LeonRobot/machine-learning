function [pi,moy,Sigma]=GMMEM(X,K,para);
%%
% Perform Maximum Likelihood estimation of the GMM parameters using the EM algorithm
% Inputs:
% X : (N x DimX) matrix of data points of DimX, with N number of data points
% K : number of components
% para : parameter options
% Output: 
% pi,mu,Sigma: mixture weights and estimated gaussian parameters for each of the components


%%initialisation of parameters pi, mu and sigma, for instance using kmeans matlab function
N=size(X,1);

%%initialisation cX is label, moy is mean for each component K
[ cX, moy ]= kmeans(X,K);
for i=1:K
    Sigma(:,:,i)=cov(X(cX==i,:));
    pi(i)=sum(cX==i)/N;
end


niter=para.niter; 
for i=1:niter
    %%E-step compute responsabilities
    % p(Z|X, theta^{old})
    % sum up p_z for each component given X
    p_z_sum = zeros(size(X, 1), 1);
    for k=1:K
       p_z_sum = p_z_sum + pi(k) * mvnpdf(X, moy(k, :), Sigma(:, :, k));
    end
    p_z = zeros(size(X, 1), 1, K);
    for k=1:K
       p_z(:, :, k) = pi(k) * mvnpdf(X, moy(k, :), Sigma(:, :, k)) ./ p_z_sum;
    end
%     p_z_sum
%     p_z
%     waitforbuttonpress;
    
    %%Mstep update the parameters pi, mu and sigma
    for k=1:K
        % update mu
        N_k = sum(p_z(:, :, k));
        weighted_sum_x = sum(bsxfun(@times, p_z(:, :, k), X));
        moy(k, :) = 1/N_k * weighted_sum_x;
        
        % update sigma
        weighted_sum_sigma_x = 0;
        for n=1:N
            covar = (X(n,:) - moy(k, :))' * (X(n,:) - moy(k, :));
            weighted_sum_sigma_x = weighted_sum_sigma_x + bsxfun(@times, p_z(n, :, k), covar);
        end
        Sigma(:, :, k) = 1/N_k * weighted_sum_sigma_x;
        
        % update pi
        pi(k) = N_k / size(X, 1);
    end
    

%moy = moy';
%Sigma = Sigma';
end

pi = pi';
%moy = moy';  % should output as K by d so it fits into reorder function
