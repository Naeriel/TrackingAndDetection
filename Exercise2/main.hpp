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
#include <glob.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include "hog_visualization.hpp"
#include "readimages.hpp"

using namespace cv;
using namespace cv::ml;
using namespace std;

vector<float> compute_descriptors(Mat image, HOGDescriptor hog);
#endif /* main_hpp */
