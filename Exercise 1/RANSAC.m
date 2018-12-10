function [worldOrientation, worldLocation, inlierIdx] = RANSAC(f, d, cameraParams, SIFT, N, t)
  [matches, scores] = vl_ubcmatch(SIFT.descriptor_points_merged, d);
  
  numMatches = size(matches, 2)
  X1 = SIFT.scatter_points_merged(matches(1,:), 1:3);
  X2 = f(1:2, matches(2,:));
  X2 = X2.';
  imgCoord = [];
  worldCoord = [];

  bestImgCoord = [];
  bestWorldCoord = [];
  bestInlierNumber = 0;

  for j = 1:N
      %Randomly select a sample of 4 data points from S and estimate the pose using PnP.]
      idx = randperm(numMatches,4);

      %concatanate the random points.
      imgCoord = [X2([idx],:)];
      worldCoord = [X1([idx],:)];

      [dummy,dummy_,dummy__,status] = estimateWorldCameraPose(imgCoord,worldCoord,cameraParams,'MaxReprojectionError',t);
      if status == 0
          [worldOrientation, worldLocation, inlierIdx] = estimateWorldCameraPose(imgCoord, worldCoord, cameraParams,'MaxReprojectionError', t);
          inlierNumber = length(inlierIdx);

          %If the number of inliers is greater than we have seen so far, re-estimate the pose using Si and store it with the corresponding number of inliers.
          if inlierNumber > bestInlierNumber
              bestInlierNumber = inlierNumber;    
              bestImgCoord = imgCoord;
              bestWorldCoord = worldCoord;
          end
      end
  end
[worldOrientation, worldLocation, inlierIdx] = estimateWorldCameraPose(bestImgCoord, bestWorldCoord, cameraParams,'MaxReprojectionError', t);
end
