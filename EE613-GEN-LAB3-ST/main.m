clear all;
close all;


DataOption=1;

if DataOption==1
  usps=load('usps.mat'); 
  % Reorganize to be consistent with the face database.
  images=usps.x; % each row is an 16 by 16 image
  for i=1:size(images,1)
    ima=reshape(images(i,:),16,16)';
    images(i,:)=ima(:);
  end

  %
  
  AllImages=images; % each row is an 16 by 16 image stacked in a 1x256 line vector
  labels=usps.y;
  DimD=16;
  SelectedDigit=3;

  %% Selecting only a subset of the images
  imagesIndex=find(labels==SelectedDigit); 
  imagesOneDigit=AllImages(imagesIndex,:);  

  % displaying some images 
  for i=1:5
    image=reshape(AllImages(i,:),DimD,DimD); %reshape the image to display it
    figure;
    imshow(image)
    title(strcat('image label :',num2str(labels(i))));
  end

  % Alternatively display a list of images as a single image
  DisplayListImage(AllImages(1:5,:),DimD,DimD,1,5);

end

if DataOption==2

  %% Loading and displaying a face
   yalefaces=load('Subset1YaleFaces.mat');
   images=yalefaces.X; %each row is a 50x50 image
   labels=yalefaces.Y;
   DimD=50;
   image=reshape(images(1,:),DimD,DimD);
   figure;
   imshow(image/255) % displaying and normalizing because imshow assumes values are between 0 and 1
   title('example of a face');

  %% Load the same faces
  data= load('SameFace.mat');
  SameFaceImages=data.images;

end

%Notice : the images are stacked in row in the above datasets

%%%%% Process Digit once the functions have been defined
Process=1;

if Process==1 
  Mdim=10;
  %% PCA decomposition
  [Lambda,U,meanX]=MyPCA(imagesOneDigit);[Lambda,U,meanX]=MyPCA(imagesOneDigit);

  %% Plot eigenvalues, including normalized cumulative
  plot_eigenvalues(Lambda);

  %% Projection matrix comprising only the Mdim first eigenvectors
  P=U(:,1:Mdim);

  % Recover the coordinates of each vector in the lower manifold of dimension Mdim.
  Y=PCAProjection(imagesOneDigit,meanX,P);

  % Reconstruct input images 
  ReconstructedImages=PCAReconstruction(Y,meanX,P);

  % Display 10 first images and their reconstruction below it
  Ntodisplay=10;
  todisplay=imagesOneDigit(1:Ntodisplay,:)
  todisplay=[todisplay;ReconstructedImages(1:Ntodisplay,:)]
  DisplayListImage(todisplay,DimD,DimD,2,Ntodisplay);
  

end

%% Probabilistic PCA code

showPPCA = 1;

if showPPCA == 1
  Mdim=10;
  % Extract Projection vectors + eigenvalues
  [Muppca,Wppca,Sigma2ppca]=MyPPCA(imagesOneDigit,Mdim);

  % Generative process - synthesize Nsynt images
  Nsynt=12;
  DigitSamples=PpcaGenerateSample(Muppca,Wppca,Sigma2ppca,Nsynt);
  
  % Visualize them as a row of images
  DisplayListImage(DigitSamples,DimD,DimD,1,Nsynt);

  % Apply Ppca even under missing values
  % size of square used for corruption
  NbData=min(800,size(imagesOneDigit,1));
  sizeSquareCor=6;


  PPCAWithMissingValues=0;

  if PPCAWithMissingValues==1

    % Corrupt input images
    corrupteddigits=CorruptImages(imagesOneDigit(1:NbData,:),DimD,DimD,sizeSquareCor);
 
    % solve for PPCA, including reconstructed values
    [imagerecons,ProjectionVect,ProjectedCoord,MeanX]=PpcaMissingValues(corrupteddigits,Mdim);

    % visualize on top of each other original data, corrupted data, reconstructed data
    Nsynt=12;
    IndexVisualize=1:12;
    % first original data
    DataToVisualize=imagesOneDigit(IndexVisualize,:);
    DataToVisualize=[DataToVisualize;corrupteddigits(IndexVisualize,:)];
    DataToVisualize=[DataToVisualize;imagerecons(IndexVisualize,:)];

    DisplayListImage(DataToVisualize,DimD,DimD,3,Nsynt);

  end

end


