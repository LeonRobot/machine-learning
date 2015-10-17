function [Samples] = PpcaGenerateSample(Muppca,Wppca,Sigma2ppca,n_samples)

% Use PPCA generative process to obtain samples 
% n_samples is the number of requested samples
% all other parameters obtained from MyPPCA

% Draw isotropic gaussian latent variables
M = size(Wppca,2);
MU = zeros(1,M);
SIGMA = eye(M);

%draw lower dimensional sample n_samples rows of M dimensions
zhat = mvnrnd(MU, SIGMA, n_samples); 

% draw noise in data space
D=size(Muppca,2);
Depsilon = mvnrnd(zeros(1,D),Sigma2ppca*eye(D)); %noise

Samples = zeros(n_samples,D);

for i=1:n_samples
	Samples(i,:) = Muppca+ (Wppca*(zhat(i,:))')' + Depsilon;
end

end
