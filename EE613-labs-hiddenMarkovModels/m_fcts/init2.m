function [model] = init2(model)
% Initialize as a chain
model.Trans = diag(ones(1, model.nbStates));
for i=1:(model.nbStates - 1)
    model.Trans(i, i + 1) = 1;
end

% Initialize chain 2
for i=1:(model.nbStates - 2)
    model.Trans(i, i + 2) = 1;
end

model.StatesPriors = zeros(model.nbStates, 1);
model.StatesPriors(1, 1) = 1;
model.Trans = model.Trans ./ repmat(sum(model.Trans, 2), 1, model.nbStates);

