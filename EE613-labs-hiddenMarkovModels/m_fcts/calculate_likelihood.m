function [s] = calculate_likelihood(s, model)
% calculate likelihood by calculating alphas and summing them up

s = calculate_alpha(s, model);

nbSamples = length(s);
for n=1:nbSamples
    s(n).L = sum(s(n).ALPHA(:,s(n).nbData));
end