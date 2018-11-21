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
    [rotationMatrix, translationVector] = cameraPoseToExtrinsics(worldOrientation, worldLocation);
    orient_matrix = [orient_matrix;worldOrientation];
    loc_matrix = [loc_matrix; worldLocation];
    %cam_coord(i,:)=(cpy_ply_vertex * rotationMatrix) + translationVector;
    close(fig);
end

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

%% SIFT Keypoints

loc1 = loc_matrix(1,1:3);
ori1 = orient_matrix(1:3,1:3);
[scatter_points_merged, descriptor_points_merged] = projectPoints('DSC_9743.JPG',ori1,loc1,intrinsicsMat',ply_vertex_coord,ply_faces,[5,6,11,12]);

1

loc2 = loc_matrix(2,1:3);
ori2 = orient_matrix(4:6,1:3);
[scatter_points, descriptor_points] = projectPoints('DSC_9744.JPG',ori2,loc2,intrinsicsMat',ply_vertex_coord,ply_faces,[5,6,11,12,4,3]);
scatter_points_merged = [scatter_points_merged;scatter_points];
descriptor_points_merged = [descriptor_points_merged,descriptor_points];
2

loc3 = loc_matrix(3,1:3);
ori3 = orient_matrix(7:9,1:3);
[scatter_points, descriptor_points] = projectPoints('DSC_9745.JPG',ori3,loc3,intrinsicsMat',ply_vertex_coord,ply_faces,[6,5,4,3]);
scatter_points_merged = [scatter_points_merged;scatter_points];
descriptor_points_merged = [descriptor_points_merged,descriptor_points];
3

loc4 = loc_matrix(4,1:3);
ori4 = orient_matrix(10:12,1:3);
[scatter_points, descriptor_points] = projectPoints('DSC_9746.JPG',ori4,loc4,intrinsicsMat',ply_vertex_coord,ply_faces,[4,3,5,6,1,2]);
scatter_points_merged = [scatter_points_merged;scatter_points];
descriptor_points_merged = [descriptor_points_merged,descriptor_points];
4

loc5 = loc_matrix(5,1:3);
ori5 = orient_matrix(13:15,1:3);
[scatter_points, descriptor_points] = projectPoints('DSC_9747.JPG',ori5,loc5,intrinsicsMat',ply_vertex_coord,ply_faces,[1,2,5,6]);
scatter_points_merged = [scatter_points_merged;scatter_points];
descriptor_points_merged = [descriptor_points_merged,descriptor_points];
5

loc6 = loc_matrix(6,1:3);
ori6 = orient_matrix(16:18,1:3);
[scatter_points, descriptor_points] = projectPoints('DSC_9748.JPG',ori6,loc6,intrinsicsMat',ply_vertex_coord,ply_faces,[1,2,7,8,5,6]);
scatter_points_merged = [scatter_points_merged;scatter_points];
descriptor_points_merged = [descriptor_points_merged,descriptor_points];
6

loc7 = loc_matrix(7,1:3);
ori7 = orient_matrix(19:21,1:3);
[scatter_points, descriptor_points] = projectPoints('DSC_9749.JPG',ori7,loc7,intrinsicsMat',ply_vertex_coord,ply_faces,[7,8,5,6]);
scatter_points_merged = [scatter_points_merged;scatter_points];
descriptor_points_merged = [descriptor_points_merged,descriptor_points];
7

loc8 = loc_matrix(8,1:3);
ori8 = orient_matrix(22:24,1:3);
[scatter_points, descriptor_points] = projectPoints('DSC_9750.JPG',ori8,loc8,intrinsicsMat',ply_vertex_coord,ply_faces,[7,8,11,12,5,6]);
scatter_points_merged = [scatter_points_merged;scatter_points];
descriptor_points_merged = [descriptor_points_merged,descriptor_points];
8

save('siftPoints.mat','scatter_points_merged', 'descriptor_points_merged');
figure;
scatter3(scatter_points_merged(:,1),scatter_points_merged(:,2),scatter_points_merged(:,3));
