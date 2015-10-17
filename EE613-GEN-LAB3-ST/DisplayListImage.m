function DisplayListImage(images,Nbli,Nbco,NbliAffiche,NbcoAffiche)

% Display a list of images of the same size as a single image of NbliAffiche x NbcoAffiche images
% each image is of size Nbli x Nbco
% all images are stored as rows in images (which contains NbliAffiche x NbcoAffiche rows)

img = {};

for i=1:NbliAffiche*NbcoAffiche
    img{i} = reshape(images(i,:),Nbli,Nbco);
end

display_matrix_of_images( img, NbliAffiche,NbcoAffiche);

end




function display_matrix_of_images( IMGS, H, W )
%IMGS is a cell list of images that we want to display as H*W

%dimension of each image
w = size(IMGS{1},1);
h = size(IMGS{1},2);
depth = size(IMGS{1},3);

%MATRIX
MATRIX = zeros(h*H,w*W,depth);

for i=1:length(IMGS)
    l = floor((i-1)/W)+1; %line
    c = i-(l-1)*W; %column
    MATRIX(w*(l-1)+1:w*l, h*(c-1)+1:h*c,:) = IMGS{i};
end
imshow(MATRIX);

end
