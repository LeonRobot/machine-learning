function [ALPHA] = foward_algorithm()
%Forward variable ALPHA (rescaled)
ALPHA(:,1) = model.StatesPriors .* s(n).B(:,1);
for t=2:s(n).nbData
    s(n).ALPHA(:,t) = (s(n).ALPHA(:,t-1)'*model.Trans)' .* s(n).B(:,t); 
end