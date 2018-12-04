%% Task 1
% Model preparation
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
[ply_vertex_coord, ply_faces] = read_ply('data/data/model/teabox.ply');

path_images = 'data/data/images/init_texture'; % Path to the images folder
dir_images = dir(fullfile(path_images,'*.jpg')); % Select .JPG files
num_images = length(dir_images); % Number of images in folder
orient_matrix = [];
loc_matrix = [];
image_coord_pts = NaN(num_vertex, 2);
% Iterate over files
for i = 1:num_images
    currentImage = imread(fullfile(path_images,dir_images(i).name));
    fig = figure;
    image(currentImage)  
    title('Mark corners and press Enter/Return to finish. If point not in image, select point outside the image');
    [x,y] = ginput(num_vertex); % Get a maximum of num_vertex corner points
    [height, width, ~]  = size(currentImage);
    
    cam_coord = zeros(num_images,3);
    
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
    [worldOrientation, worldLocation] = estimateWorldCameraPose(image_coord_cpy, cpy_ply_vertex, cameraParams,'MaxReprojectionError',10);
    orient_matrix = [orient_matrix;worldOrientation];
    loc_matrix = [loc_matrix; worldLocation];
    close(fig);
end
save('coord.mat', 'orient_matrix', 'loc_matrix');
clear i j k;
%% Plot box and camera locations
pcshow(ply_vertex_coord,'VerticalAxis','Y',...
       'VerticalAxisDir','down','MarkerSize',200);
hold on
for i=1:num_images
    plotCamera('Size', 0.05, 'Orientation', orient_matrix((i-1)*3+1:i*3,1:3), 'Location',...
         loc_matrix(i,1:3));
     xlim([-1,1])
     ylim([-1, 1])
     hold on 
end
hold off
clear i;
%% SIFT Keypoints
scatter_points_merged = [];
descriptor_points_merged = [];
centroid = [0.165 0.063 0.093]./2;
ply_faces(9:10,:) = []; % triangles 9 and 10 are never visible

% Iterate over images
for i = 1:num_images
    I = imread(dir_images(i).name);
    I = single(rgb2gray(I));
    [f,d] = vl_sift(I);
    descriptor_points = [];
    
    cameraVector = centroid - loc_matrix(i,1:3);
    [select_faces] = selectFaces(ply_vertex_coord,ply_faces, cameraVector)
    
    % Collect scattering points and descriptor points
    current_location = loc_matrix(i,:);
    current_orientation = orient_matrix((i-1)*3+1:i*3,1:3);
    for j = (1:size(facestemp,1))
        [scatter_points, descriptor_points] = projectPoints(f,d,current_orientation,current_location,intrinsicsMat',ply_vertex_coord,ply_faces,select_faces(j,:) + 1);
        size(scatter_points,1);
        scatter_points_merged = [scatter_points_merged; scatter_points];
        descriptor_points_merged = [descriptor_points_merged,descriptor_points];
    end
end
save('siftPoints.mat','scatter_points_merged', 'descriptor_points_merged'); % save descriptors for task 2
figure;
scatter3(scatter_points_merged(:,1),scatter_points_merged(:,2),scatter_points_merged(:,3));
%clear all;