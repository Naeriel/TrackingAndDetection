%% Pose refinement with non-linear optimization
%% Camera Intrinsics & num vertex
fx = 2960.37845; % focal length
fy = fx;
cx = 1841.68855; 
cy = 1235.23369;
A = [fx 0 cx; 0 fy cy; 0 0 1];
path_images = 'data/data/images/tracking'; % Path to the images folder
dir_images = dir(fullfile(path_images,'*.jpg')); % Select .JPG files
num_images = length(dir_images);

%% Read first image
currentImage = imread(fullfile(path_images,dir_images(1).name));
currentImage = single(rgb2gray(currentImage)); % To grayscale to do SIFT
[f,d] = vl_sift(currentImage);
save('firstImgFeatures','f','d');