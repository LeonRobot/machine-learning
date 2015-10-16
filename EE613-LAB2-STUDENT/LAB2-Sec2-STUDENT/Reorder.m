function [ pireorder,mureorder,sigmareorder] = Reorder(pitest,mutest,sigmatest,meansref)
% search the best correspondance i,j between meansref and mutest to minimize the
% distance : ||meansref(:,i) mutest(:,j)||
%  greedy algorithm. one could have found the optimal permutation
% Input: 
% pitest, mutest, sigmatest GMM parameters which need to be reordered,
% mutest should be a d by K matrix with d the data dimensionality and K the
% number of GMM components
% pitest is an K by 1 matrix, and sigmatest a d by d by K matrix
%
% meansref: means which are used for reordering, should be of same size as mutest
%
% Output
% pireorder,mureorder,sigmareorder: reordered gaussian parameters



mutest=mutest';

%mutest
%meansref

K=size(meansref,2);
dist=inf(K);
for i=1:K
    for j=1:K
        dist(i,j)=norm(meansref(:,i)-mutest(:,j));
    end
end

mureorder=nan(size(mutest));
pireorder=nan(size(pitest));
sigmareorder=nan(size(sigmatest));
for c=1:K
    [val,idx]=min(dist(:));
    [i,j]=ind2sub([K,K],idx);
    mureorder(:,i)=mutest(:,j);
    pireorder(i)=pitest(j);
    sigmareorder(:,:,i)=sigmatest(:,:,j);
    dist(i,:)=inf;
    dist(:,j)=inf;
end

mureorder


end


