function [s] = calculate_alpha(s, model)
% calculate likelihood by calculating alphas and summing them up

nbSamples = length(s);
for n=1:nbSamples
    %Emission probabilities
    for i=1:model.nbStates
        s(n).B(i,:) = gaussPDF(s(n).Data, model.Mu(:,i), model.Sigma(:,:,i));
    end

    %Forward variable ALPHA initialize
    s(n).ALPHA(:,1) = model.StatesPriors .* s(n).B(:,1);

    for t=2:s(n).nbData
        s(n).ALPHA(:,t) = (s(n).ALPHA(:,t-1)'*model.Trans)' .* s(n).B(:,t);
    end
end