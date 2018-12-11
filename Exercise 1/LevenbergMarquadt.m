function [Rt, Tt] = LevenbergMarquadt(R0, T0, A, M, m,  N, thresh)
    
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
        
        J = [];
        for j=1:size(M)
            dM_dp = [dR_dv1*M(j,:)', dR_dv2*M(j,:)', dR_dv3*M(j,:)',eye(3)];
            dmt_dM = A;
            mt = A*(R*M(j,:)'+T'); %feature point backprojected on image
            U = mt(1);
            V = mt(2);
            W = mt(3);
            
            dm_dmt = [1/W, 0, -U/W^2; 0 , 1/W, -V/W^2];
            
            J_new = dm_dmt * dmt_dM * dM_dp;
            J = [J;J_new];
        end

        
        e = energyFunction(R, T, A, M, m);

        delta = -(J'*J + lambda*eye(6))^(-1)*(J'*e);
        
        theta_new = theta + delta;
        v_new = theta_new (1:3);
        R_new = rotationVectorToMatrix (v_new);
        T_new = theta_new (4:6);
        e_new = energyFunction(R_new, T_new, A, M, m);
        
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
