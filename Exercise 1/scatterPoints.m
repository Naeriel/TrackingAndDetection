function scatter_points = scatterPoints(filename, R_t,C,K,vertex,face,faces_pick)
    I = imread(filename);
    I = single(rgb2gray(I));
    [f,d] = vl_sift(I) ;
    perm = randperm(size(f,2)) ;
    sel = perm(1:5000) ;
    f = f(:,sel);
    R= R_t';
    
    m_nonhomo = f(1:2,1:size(f,2));
    homogenize_ones = ones(1,size(f,2));
    m_homo = [m_nonhomo;homogenize_ones];
    dir = R*inv(K)*m_homo;
    
    face_m = face + 1;
    scatter_points = [];
    
    for i = 1:size(f,2)
        [INTERSECT, T,U,V, XCOOR] = TriangleRayIntersection(C,dir(1:3,i),vertex(face_m(faces_pick,1),[1:3]),vertex(face_m(faces_pick,2),[1:3]),vertex(face_m(faces_pick,3),[1:3]));
        if sum(INTERSECT) > 0
            for j = 1:size(INTERSECT,1)
                if INTERSECT(j)
                    scatter_points = [scatter_points;XCOOR(j,[1:3])];
                end
            end
        end
    end
end