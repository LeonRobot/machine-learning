function plot_eigenvalues( Lambda )

% The function plots the seigenvalues including their cumulative distribution
% Lambda is the set of all eigenvalues of the data covariance matrix
% ranked in decreasing order
% 
    
dim = length(Lambda);
subplot(2,1,1), plot(1:dim,Lambda)
xlabel('component')
ylabel('eigenvalue')

%cumulative
cumul = zeros(1,dim);
total_variance = sum(Lambda);
for i=1:dim
   cumul(i) = sum(Lambda(1:i))/total_variance; 
end
subplot(2,1,2), plot(1:dim,cumul)
xlabel('component')
ylabel('cumulative')
end

