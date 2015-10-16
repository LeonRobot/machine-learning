function [pimap, moymap, sigmaMAP] = GMMadapt (X,K,para)
%%
%  [pimap, moymap, sigmaMAP] = GMMadapt (X,K,para)
%  Perform Map adaptation of the GMM parameters using the EM algorithm
%  Inputs:
%    X : (N x DimX) matrix of data points of DimX, with N number of data points
%    para: parameters for the priors
% Output: 
% pimap,mumap,sigmaMAP: mixture weights abd estimated gaussian parameters for each of the components


%%initialisation
pimap=para.priormixcoef;
moymap=para.priorm';
sigmaMAP=para.V/(para.d-para.Dimp-1);

N = size(X, 1);

% moymap(1,:)
% sigmaMAP(:,:,1)
% [~,p] = chol(sigmaMAP(:,:,1))

niter=para.niter;
for i=1:niter
    %%E-step equivalent to classical EM
    % p(Z|X, theta^{old})
    % sum up p_z for each component given X
    p_z_sum = zeros(size(X, 1), 1);
    for k=1:K
%        sigmaMAP(:,:,k)
       p_z_sum = p_z_sum + pimap(k) * mvnpdf(X, moymap(k, :), sigmaMAP(:, :, k));
%        p_z_sum
    end
    p_z = zeros(N, 1, K);
    for k=1:K
       p_z(:, :, k) = pimap(k) * mvnpdf(X, moymap(k, :), sigmaMAP(:, :, k)) ./ p_z_sum;
    end
    
%     p_z_sum
%     p_z
    
%     waitforbuttonpress;
    
    %%Mstep M step from EM + adaptation
    %%Mstep update the parameters pi, mu and sigma
    for k=1:K
        % update mu
        N_k = sum(p_z(:, :, k));
        weighted_sum_x = sum(bsxfun(@times, p_z(:, :, k), X));
        moy_k = ((1/N_k) * (weighted_sum_x))';
        % incorporate prior
        p_z(:,:,k)
        weighted_sum_x
        para.priorm(:,k)
        moy_k
        moymap(k,:) = (para.tau * para.priorm(:,k) + N_k * moy_k) / (para.tau + N_k);
        
        % update sigma
        weighted_sum_sigma_x = 0;
        for n=1:N
            covar = (X(n,:) - moy(k, :))' * (X(n,:) - moy(k, :));
            weighted_sum_sigma_x = weighted_sum_sigma_x + bsxfun(@times, p_z(n, :, k), covar);
        end
        Sigma_k = 1/N_k * weighted_sum_sigma_x;
%         Sigma_k
%         sigmaMAP(:,:,k)
        sigmaMAP(:,:,k) = (para.V(:,:,k) + N * Sigma_k + (para.tau * N)/(para.tau + N) * (para.priorm - moy_k) * (para.priorm - moyk)') / (V - d + N);
%         sigmaMAP(:,:,k)
%         input(prompt)
        % update pi
        pi_k = N_k / N;
        counts_sum = 0;
        for j=1:K
            N_kp = sum(p_z(:, :, j));
            counts_sum = counts_sum (para.priormixcoef(k) + N_kp);
        end
        pimap(k) = (para.priormixcoef(k) + N_k - 1) / (counts_sum - K);
    end
end
