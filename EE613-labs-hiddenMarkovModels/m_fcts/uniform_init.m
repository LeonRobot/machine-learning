function [model] = uniform_init(model)
%Uniform initialization
model.Trans = ones(model.nbStates,model.nbStates);
model.Trans = model.Trans ./ repmat(sum(model.Trans,2),1,model.nbStates);
model.StatesPriors = ones(model.nbStates,1);
model.StatesPriors = model.StatesPriors / sum(model.StatesPriors);
