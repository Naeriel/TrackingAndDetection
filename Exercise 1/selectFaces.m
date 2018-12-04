function [select_faces] = selectFaces(ply_vertex_coord,ply_faces, cameraVector)
    out_list = [];
    filter_angle = 95;
    select_faces = ply_faces;
    
        % Select visible faces for each image
        for j = 1:length(ply_faces)
            vector1 = ply_vertex_coord(ply_faces(j,2)+1,:) - ply_vertex_coord(ply_faces(j,1)+1,:);
            vector2 = ply_vertex_coord(ply_faces(j,3)+1,:) - ply_vertex_coord(ply_faces(j,1)+1,:);
            normal = cross(vector1,vector2);
            cos_angle = dot(cameraVector,normal)/(norm(cameraVector)*norm(normal));
            angle_degrees = acosd(cos_angle);

            if angle_degrees < filter_angle
                out_list = [out_list;j];
            end
        end

        select_faces(out_list,:) = [];
end

