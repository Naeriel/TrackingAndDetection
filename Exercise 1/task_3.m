%% Pose refinement with non-linear optimization
%% Camera Intrinsics & num vertex
fx = 2960.37845; % focal length
fy = fx;
cx = 1841.68855; 
cy = 1235.23369;
A = [fx 0 cx; 0 fy cy; 0 0 1];
cameraParams = cameraParameters('IntrinsicMat',A'); % Store intrinsics matrix 
c = 4.685; % weight

%Implement this algorithm such that threshold t and number of iterations N can be selected by the user.
prompt = 'What is the threshold? ';
t = input(prompt);
prompt = 'Enter the iteration? ';
N = input(prompt);

path_images = 'data/data/images/tracking'; % Path to the images folder
dir_images = dir(fullfile(path_images,'*.jpg')); % Select .JPG files
num_images = length(dir_images);
camera_locations = zeros(num_images,3);
[ply_vertex_coord, ply_faces] = read_ply('data/data/model/teabox.ply');

%SIFT keypoints corresponding to the tea box and their 3D locations on the model.
SIFT = load('siftPoints.mat');
%% 
for i = 2
    % Read current image and previous image and load SIFT points
    currentImage = imread(fullfile(path_images,dir_images(i).name));
    curImage = currentImage;
    currentImage = single(rgb2gray(currentImage)); % To grayscale to do SIFT
    previousImage = imread(fullfile(path_images,dir_images(i - 1).name));
    previousImage = single(rgb2gray(previousImage)); 
    [f,d] = vl_sift(currentImage);
    [fp,dp] = vl_sift(previousImage);
    
    % Match SIFT points from training images (ex02) with previousImage SIFT
    % and use RANSAC to compute camera location
    [worldOrientation, worldLocation, inlierIdx] = RANSAC(fp, dp, cameraParams, SIFT, N, t);
    %plotCameras(worldOrientation,worldLocation);
    
    [rotationMatrix,translationVector] = cameraPoseToExtrinsics(worldOrientation,worldLocation);
    projectedPoints = worldToImage(cameraParams, rotationMatrix, translationVector, ply_vertex_coord);
    plotBounding3D(projectedPoints', curImage);
    rotationVector = rotationMatrixToVector(rotationMatrix);
    p = [rotationVector,translationVector]'; %vector of parameters (trans + rot)
end
