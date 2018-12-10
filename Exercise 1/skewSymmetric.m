%% Skew-symmetric rotation matrix
function R = skewSymmetric(v)
  
    R = zeros(3);
    R(1, 2) = -v(3);
    R(1, 3) = v(2);
    R(2, 1) = v(3);
    R(2, 3) = -v(1);
    R(3, 1) = -v(2);
    R(3, 2) = v(1);
end