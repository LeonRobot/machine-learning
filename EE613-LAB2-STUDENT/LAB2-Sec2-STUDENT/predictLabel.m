function zpred=predictLabel(mixingcoeff,means,sigma,x)
%%
% assign each of the data point in x to it's closest gaussian component
% Inputs: 
% mixingcoeff,means,sigma,x: gaussian parameters and data points
% -x: d by N matrix with d the data dim and N the number of data point
% -mixingcoeff: 1 by K matrix with K the number of components
% -means: d by K matrix
% -sigma: d by d by X matrix
% Outputs:
% zpred: predicted labels

N=length(x);
zpred=zeros(N,1);
K=length(mixingcoeff);
for i=1:N
    maxi=0;
    for j=1:K
        if mixingcoeff(j)*mvnpdf(x(:,i)',means(:,j)',sigma(:,:,j)) > maxi
            maxi=mixingcoeff(j)*mvnpdf(x(:,i)',means(:,j)',sigma(:,:,j));
            zpred(i)=j;
        end
    end
end

% footnote: all function can be summarized in 1 matlab style command
%[maxi,zpred]=max([ mixingcoeff(1)*mvnpdf(x',means(:,1)',sigma(:,:,1)), mixingcoeff(2)*mvnpdf(x',means(:,2)',sigma(:,:,2)), mixingcoeff(3)*mvnpdf(x',means(:,3)',sigma(:,:,3)) ],[],2);