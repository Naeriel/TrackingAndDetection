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

%% 
for i = 2:num_images
    % Read current image and previous image and load SIFT points
    currentImage = imread(fullfile(path_images,dir_images(i).name));
    currentImage = single(rgb2gray(currentImage)); % To grayscale to do SIFT
    previousImage = imread(fullfile(path_images,dir_images(i - 1).name));
    previousImage = single(rgb2gray(previousImage)); 
    [f,d] = vl_sift(currentImage);
    [fp,dp] = vl_sift(previousImage);
end
