%% Compute LM algorithm
function theta = LevenbergMarquadt(x,theta0, N, thresh)
    theta = theta0;
    lambda = 0.001;
    u = thresh + 1;
    
    for ((i = 1:N) && (u > thresh))
        J = jacobian(f, v);
        e = energy(x, theta);
        
        delta = -(J'*J + lambda*J)^(-1)*(J'*e)
        en = energy(x, theta + delta);
        
        if en > e
            lambda = 10 * lambda;
        else
            lambda = 10 / lambda;
            theta = theta + delta;
        end
        
        u = norm(delta);
    end
end

