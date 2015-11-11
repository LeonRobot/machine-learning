function [model] = init3(model)
% Initialize as a modulo

model.Trans = diag(ones(1, model.nbStates));
for i=1:model.nbStates
    indxbefore = i - 1;
    indxbefore = mod(indxbefore, model.nbStates);
    indxafter = i + 1;
    indxafter = mod(indxafter, model.nbStates);
    if indxafter == 0;
        indxafter = model.nbStates;
    end
    if indxbefore == 0;
        indxbefore = model.nbStates;
    end
    model.Trans(i, indxbefore) = 1;
    model.Trans(i, indxafter) = 1;
end

model.StatesPriors = zeros(model.nbStates, 1);
model.StatesPriors(1, 1) = 1;
model.Trans = model.Trans ./ repmat(sum(model.Trans, 2), 1, model.nbStates);

