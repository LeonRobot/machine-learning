function X=genData1Gauss(T,p,mean,covar)
X=randn(T,p)*covar^(0.5)+repmat(mean',T,1);