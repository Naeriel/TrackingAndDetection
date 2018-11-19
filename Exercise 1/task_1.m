%% Camera Intrinsics & num vertex
fx = 2960.37845; % focal length
fy = fx;
cx = 1841.68855; 
cy = 1235.23369;
s = 1; % shift
num_vertex = 8; % max num of corners

intrinsicsMat = [fx 0 0; s fy 0; cx cy 1];
cameraParams = cameraParameters('IntrinsicMat',intrinsicsMat); % Store intrinsics matrix 

%% Load PLY model and image files
[ply_vertex_coord, ] = read_ply('data/data/model/teabox.ply');

path_images = 'data/data/images/init_texture'; % Path to the images folder
dir_images = dir(fullfile(path_images,'*.jpg')); % Select .JPG files
num_images = length(dir_images); % Number of images in folder

image_coord_pts = NaN(num_vertex, 2);
% Iterate over files
for i = 1:num_images
    currentImage = imread(fullfile(path_images,dir_images(i).name));
    fig = figure;
    image(currentImage)  
    title('Mark corners and press Enter/Return to finish. If point not in image, select point outside the image');
    [x,y] = ginput(num_vertex); % Get a maximum of num_vertex corner points
    [height, width, ~]  = size(currentImage);
    
    num_points = length(x);
    cpy_ply_vertex = ply_vertex_coord;
    % Save coordinates of points
    for j = 1:num_vertex
        % Points inside image boundaries, if not, set to -1
        if ((x(j) <= width) && (x(j) >= 0)) && ((y(j) <= height)&& (y(j) >= 0)) 
            image_coord_pts(j,1) = x(j);
            image_coord_pts(j,2) = y(j);
        else 
            image_coord_pts(j,1) = -1;
            image_coord_pts(j,2) = -1;
        end
    end
    image_coord_cpy = image_coord_pts;
    num_delete = 0;
    for k = 1:num_vertex
        if(image_coord_pts(k,1) == -1)
            image_coord_cpy(k - num_delete, :) = [];
            cpy_ply_vertex(k - num_delete, :) = [];
            num_delete = num_delete +1;
        end
    end
    close(fig);
end
