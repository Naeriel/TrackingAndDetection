%% Camera Intrinsics & num vertex
fx = 2960.37845; % focal length
fy = fx;
cx = 1841.68855; 
cy = 1235.23369;
s = 1; % shift
num_vertex = 8;

intrinsicsMat = [fx 0 0; s fy 0; cx cy 1];
cameraParams = cameraParameters('IntrinsicMat',intrinsicsMat); % Store intrinsics matrix 

%% Load PLY model and image files
[vertex, ] = read_ply('data/data/model/teabox.ply');

path_images = 'data/data/images/init_texture'; % Path to the images folder
dir_images = dir(fullfile(path_images,'*.jpg')); % Select .JPG files
num_images = length(dir_images); % Number of images in folder

% Iterate over files
for i = 1:num_images
    currentImage = imread(fullfile(path_images,dir_images(i).name));
    fig = figure;
    image(currentImage)  
    title('Mark corners and press Enter/Return to finish');
    [x,y] = ginput(num_vertex); % Get a maximum of num_vertex corner points
    [height, width, ~]  = size(currentImage);
    
    num_points = length(x);
    coord_points = zeros(num_points, 2);
    % Save coordinates of points
    for j = 1:num_points
        % Points inside image boundaries
        if ((x(j) <= width) && (x(j) >= 0)) || ((y(j) <= height)&& (y(j) >= 0)) 
            coord_points(j,1) = x(j);
            coord_points(j,2) = y(j);
        end
    end
    close(fig);
end
