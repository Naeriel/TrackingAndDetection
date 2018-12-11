function plotCameras(orient_matrix,loc_matrix)
    plotCamera('Size', 0.05, 'Orientation', orient_matrix, 'Location',...
         loc_matrix);
     xlim([-1,1])
     ylim([-1, 1])
end

