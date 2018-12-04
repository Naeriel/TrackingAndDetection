%% Divides the first and second term of a vector by the third
function v = normalize(v)
   v = v(:, 1:2) ./ repmat(v(:, 3), [1 2]); 
end