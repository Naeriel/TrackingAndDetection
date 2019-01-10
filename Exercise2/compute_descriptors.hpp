//
//  compute_descriptors.hpp
//  Exercise2
//
//  Created by Cristina Serrano González on 10.01.19.
//  Copyright © 2019 Cristina Serrano González. All rights reserved.
//

#ifndef compute_descriptors_h
#define compute_descriptors_h
std::vector<float> compute_descriptors(cv::Mat image, cv::HOGDescriptor hog, cv::Size winSize);

std::vector<float> compute_descriptors(cv::Mat image, cv::HOGDescriptor hog, cv::Size winSize){
    cv::Mat resImg;
    static std::vector<float> descriptor;
    // resize original image to 640x640
    resize(image, resImg, winSize,0,0,CV_INTER_LINEAR);
    // Compute HOG descriptors
    hog.compute(resImg, descriptor);
    return descriptor;
}
#endif /* compute_descriptors_h */
