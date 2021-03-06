%% Camera Intrinsics & num vertex
fx = 2960.37845; % focal length
fy = fx;
cx = 1841.68855; 
cy = 1235.23369;
s = 1; % shift
num_vertex = 8; % max num of corners

intrinsicsMat = [fx 0 0; s fy 0; cx cy 1];
cameraParams = cameraParameters('IntrinsicMat',intrinsicsMat); % Store intrinsics matrix 

%SIFT keypoints corresponding to the tea box and their 3D locations on the model.
SIFT = load('siftPoints.mat');

%Implement this algorithm such that threshold t and number of iterations N can be selected by the user.
prompt = 'What is the threshold? ';
t = input(prompt);
prompt = 'Enter the iteration? ';
N = input(prompt);

%% Load image files
%Windows
%[ply_vertex_coord, ply_faces] = read_ply('data\data\model\teabox.ply'); %
% MacOS / Linux
[ply_vertex_coord, ply_faces] = read_ply('data/data/model/teabox.ply');

%Windows
%path_images = 'data\images\detection';              % Path to the images folder
% MacOS / Linux
path_images = 'data/data/images/detection';
dir_images = dir(fullfile(path_images,'*.jpg'));    % Select .JPG files
num_images = length(dir_images);                    % Number of images in folder

%figure
%pcshow(ply_vertex_coord,'VerticalAxis','Y','VerticalAxisDir','down','MarkerSize',200);
% %hold on

for i = 1:num_images
    currentImage = imread(fullfile(path_images,dir_images(i).name));
    curImage = currentImage;
    currentImage = single(rgb2gray(currentImage));
    [f,d] = vl_sift(currentImage) ;
    
    %Determine the set of data points Si from all 2D-3D correspondences where reprojection error (Euclidean distance) is below the threshold t. 
    %The set Si is the consensus set of the sample and defines the inliers of S.
    
    [worldOrientation, worldLocation, inlierIdx] = RANSAC(f, d, cameraParams, SIFT, N, t);

    %For each image from the test sequence you are required to provide visualization of 3D bounding boxes for the detected object as shown in Figure 1.
%    plotCamera('Size', 0.05, 'Orientation', worldOrientation, 'Location', worldLocation);
    
    [rotationMatrix,translationVector] = cameraPoseToExtrinsics(worldOrientation,worldLocation);
    projectedPoints = worldToImage(cameraParams, rotationMatrix, translationVector, ply_vertex_coord);

    plotBounding3D(projectedPoints', curImage);
    
end