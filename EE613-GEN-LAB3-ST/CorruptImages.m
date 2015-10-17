function [CorruptedImages]=CorruptImages(images,Nbli,Nbco,sizeSquare)

%  Corrupt initial images by putting a square within each images as non
%  observed 

nbim=size(images,1)

LocationMax= min(Nbli,Nbco)-sizeSquare+1;

CorruptedImages=[];
for i=1:nbim
   % select random x and y location
   wli=randi(LocationMax);
   wco=randi(LocationMax);
   imares=reshape(images(i,:),Nbli,Nbco);
   imares(wli:wli+sizeSquare-1,wco:wco+sizeSquare-1)=NaN;
   imacor=imares(:);
   CorruptedImages=[CorruptedImages;imacor'];
   
end