function [scatter_points,descriptor_points] = projectPoints(f, d, R_t, C_t, K, vertex, face, faces_pick)
    % Transform to homogeneous coordinates
    m_nonhomo = f(1:2,1:size(f,2));
    homogenize_ones = ones(1,size(f,2));
    m_homo = [m_nonhomo;homogenize_ones];
 
    % Convert to correct coordinates
    [R, t] = cameraPoseToExtrinsics(R_t, C_t);
    Q = K * R'; % Remember to transpose rotation matrix (R)
    q = K * t';
    C = (-Q^(-1) * q);
    dir = Q^(-1) * m_homo;
    
    scatter_points = [];
    descriptor_points = [];
    
    % Iterate through points. If point intersects the triangle, store
    % coordinates
    for i = 1:size(f,2)
        [INTERSECT, T,U,V, XCOOR] = TriangleRayIntersection(C,dir(1:3,i),vertex(faces_pick(1),:),vertex(faces_pick(2),:),vertex(faces_pick(3),:));
        if INTERSECT
            scatter_points = [scatter_points;XCOOR];
            descriptor_points = [descriptor_points,d(1:128,i)];
        end
    end
end