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
#include <fstream>
#include <vector>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include "hog_visualization.hpp"
#include "train_tree.hpp"
#include "compute_descriptors.hpp"

using namespace cv;
using namespace cv::ml;
using namespace std;

#define    HOGwinSize      Size(640,640)
#define    HOGblockSize    Size(80, 80)
#define    HOGblockStride  Size(80, 80)
#define    HOGcellSize     Size(80, 80)
#define    HOGnbins        9
#define    num_of_trees    100
#define    num_of_folders  6
#define    test_samples    10

unsigned int GetRandomInteger(int low, int up);
int* randperm(int Num, int low, int up);
int vote_for_result(const float a[]);


class randomForest{
public:
    
    randomForest(){
        cv::Ptr<cv::ml::DTrees> dtrees;
        dtrees = cv::ml::DTrees::create();
        dtrees->setMaxDepth(10);
        dtrees->setMinSampleCount(10);
        dtrees->setMaxCategories(2);
        dtrees->setCVFolds(0);
    }
    
    cv::Ptr<cv::ml::DTrees> creatRF(){
        cv::Ptr<cv::ml::DTrees> dtrees;
        dtrees = cv::ml::DTrees::create();
        dtrees->setMaxDepth(10);
        dtrees->setMinSampleCount(10);
        dtrees->setMaxCategories(2);
        dtrees->setCVFolds(0);
        return dtrees;
    }
    
    void trainRF(const cv::Ptr<cv::ml::DTrees> &tree, const cv::Ptr<cv::ml::TrainData> &data){
        tree->train(data);
    }   
};

// GetRandomInteger() and randperm are used to generate random index
unsigned int GetRandomInteger(int low, int up)
{
    unsigned int uiResult;
    
    if (low > up)
    {
        int temp = low;
        low = up;
        up = temp;
    }
    
    uiResult = static_cast<unsigned int>((rand() % (up - low + 1)) + low);
    
    return uiResult;
}
 // Random permutation of index
int* randperm(int Num, int low, int up) {
    
    int *temp = new int[Num];
    for(int i = 0; i < Num; ++i){
        temp[i]=GetRandomInteger(low,up);
    }
    return temp;
}
#endif /* main_hpp */
