function [Rt, Tt] = LevenbergMarquadt(R0, T0, A, M, m,  N, thresh)
    cameraParams = cameraParameters('IntrinsicMat',A'); % Store intrinsics matrix 
    c = 4.685; % weight

    R = R0;
    T = T0;
    
    lambda = 0.001;
    u = thresh + 1;
    
    for i = 1:N
      if (u > thresh)
          
        v = rotationMatrixToVector(R); %rotationVector
        
        
        theta = [v, T]';
        
        vx = skewSymmetric (v);

       tmp = v .* (eye(3)-R);
        
        dR_dv1 = (v(1)*vx+tmp(1))*R/sum(v.^2);
        dR_dv2 = (v(2)*vx+tmp(2))*R/sum(v.^2);
        dR_dv3 = (v(3)*vx+tmp(3))*R/sum(v.^2);
   %     dR_dv = (cross(v,vx)+cross(v, eye(3)-R))*R/sum(v.^2);
        dR_dv = [dR_dv1, dR_dv2, dR_dv3];
        
        
        e = energyFunction(R, T, A, M, m);
        
        M_img = worldToImage(cameraParams, R, T, M);
        dist_root = (m - M_img');
        dist = (m - M_img').^2;
        e = [dist(1, :)'; dist(2, :)'];
        
        sigma = 1.48257968 * median(e);
        W_matrix = diag( w_i_weight(e / sigma, c) );
        
        e_old = sum( m_estimator(e / sigma, c) );
        
        J = [];
        Ju = zeros(length(M), 6);
        Jv = zeros(length(M), 6);
        for j=1:length(M)
            dM_dp = [dR_dv1*M(j,:)', dR_dv2*M(j,:)', dR_dv3*M(j,:)',eye(3)];
            dmt_dM = A;
            mt = A*(R*M(j,:)'+T'); %feature point backprojected on image
            U = mt(1);
            V = mt(2);
            W = mt(3);
            
            dm_dmt = [1/W, 0, -U/W^2; 0 , 1/W, -V/W^2];
            
            J_new = dm_dmt * dmt_dM * dM_dp;
            Ju(j, :) = J_new(1,:) * 2 * dist_root(1, j);
            Jv(j, :) = J_new(2,:) * 2 * dist_root(2, j);
        end
        
        J = vertcat(Ju, Jv);

        delta = -(J'*W_matrix*J + lambda*eye(6))^(-1)*(J'*W_matrix*e);
        
        theta_new = theta + delta;
        v_new = theta_new (1:3);
        R_new = rotationVectorToMatrix (v_new);
        T_new = theta_new (4:6);
        M_img_new = worldToImage(cameraParams, R_new, T_new, M);
        dist_new = (m - M_img_new').^2;
        e_new = [dist_new(1, :)'; dist_new(2, :)'];
        
        %e_new = energyFunction(R_new, T_new, A, M, m);
        
        if e_new > e
            lambda = 10 * lambda;
        else
            lambda = 10 / lambda;
            R = R_new;
            T = T_new;
        end                  
        u = norm(delta);
      
      else 
          break;
      end
      
      Rt = R;
      Tt = T;
      
    end
end

function w_i = w_i_weight(e_i, c)
    len_e_i = length(e_i);
    w_i = zeros(len_e_i, 1);
    for e_index=1:len_e_i
        if abs(e_i(e_index)) < c
            w_i(e_index) = (1 - e_i(e_index)^2 / c^2)^2;
        end
    end
end

function p_i = m_estimator(e_i, c)
    len_e_i = length(e_i);
    p_i = c^2 / 6 *  ones(len_e_i, 1);

    for e_index=1:len_e_i
        if abs(e_i(e_index)) <= c
            p_i(e_index) = c^2 / 6 * (1 - (1 - (e_i(e_index) / c)^2)^3);
        end
    end
end