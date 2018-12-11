%% Energy function F to compute camera pose
function E = energyFunction(R, T, A, M, m)

%    tmp1 = R*M';
%    tmp2 = tmp1 + T';
%    tmp3 = A * tmp2;
%    tmp3 = normalize(tmp3');
%   %tmp1 = worldToImage(A, R, T, M);
%    tmp4 = tmp3 - m';
    
%    E = sum(sum( tmp4.^2 ) );
    
    E = sum(sum( ((normalize((A * ((R*M') + T'))')) - m').^2 ) );

end