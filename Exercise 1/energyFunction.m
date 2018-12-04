%% Energy function F to compute camera pose
function E = energyFunction(R, T, A, M, m, angle_fun)
    
    M = (M * angle_fun(R)' + repmat(T', [size(M, 1), 1])) * A'; %compute Matrix M   
    dist = M - m; % difference between 3D coordinates (M) and image points (m)
    E = sum(sum(dist.^2));
end

