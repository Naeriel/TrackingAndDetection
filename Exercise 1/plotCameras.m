function plotCameras(orient_matrix,loc_matrix, color)
    plotCamera('Size', 0.01, 'Orientation', orient_matrix, 'Location',...
         loc_matrix, 'Color', color);
     xlim([-1,1])
     ylim([-1, 1])
end

