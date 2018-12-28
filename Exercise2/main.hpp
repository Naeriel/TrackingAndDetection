//
//  main.hpp
//  
//
//  Created by Cristina Serrano González on 27.12.18.
//

#ifndef main_hpp
#define main_hpp

#include <stdio.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

void visualizeHOG(cv::Mat img, std::vector<float> &feats, cv::HOGDescriptor hog_detector, int scale_factor = 3);
#endif /* main_hpp */
