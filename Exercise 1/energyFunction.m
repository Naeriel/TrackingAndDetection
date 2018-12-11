%% Energy function F to compute camera pose
function E = energyFunction(R, T, A, M, m)
    %tmp1 = R*M';
    %tmp2 = tmp1 + T';
    %tmp3 = A * tmp2;
    %tmp3 = normalize(tmp3);
    tmp1 = worldToImage(A, R, T, M);
    tmp2 = tmp1 - m;
    E = sum(sum( (tmp2)^2 ) );
end