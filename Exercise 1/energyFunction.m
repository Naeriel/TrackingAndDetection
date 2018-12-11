%% Energy function F to compute camera pose
function E = energyFunction(R, T, A, M, m)
    
    E = sum(sum(dot(A,R.*M+T)-m)^2);
end