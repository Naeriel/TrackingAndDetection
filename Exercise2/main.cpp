//
//  main.cpp
//  
//
//  Created by Cristina Serrano González on 27.12.18.
//

#include "main.hpp"

#define    HOGwinSize      Size(640,640)
#define    HOGblockSize    Size(80, 80)
#define    HOGblockStride  Size(80, 80)
#define    HOGcellSize     Size(80, 80)
#define    HOGnbins        9
#define    num_of_trees    100
#define    num_of_folders  6

// Compute HOG descriptor
vector<float> compute_descriptors(Mat image, HOGDescriptor hog){
    Mat hogImg;
    vector<float> descriptor;
    // resize original imageto 640x640
    resize(image, hogImg, Size(640,640),0,0, INTER_NEAREST);
    
    // Compute HOG descriptors
    hog.compute(hogImg, descriptor);
    
    return descriptor;
}


int main( int argc, char** argv ){
    // TASK 1: OPEN CV AND HOG DESCRIPTORS
    // Load image
    Mat image;
    image = imread("/Users/zojja/TUM/Exercise2/Exercise2/data/task1/obj1000.jpg",IMREAD_COLOR); // change image path
    
    // Change image to grayscale
    Mat grayImg;
    cvtColor(image, grayImg, COLOR_BGR2GRAY );
    imshow( "Grayscale", grayImg);
    imwrite("/Users/zojja/TUM/Exercise2/Exercise2/grayscale.jpg", grayImg);
    
    // Flip mirror ( code 1 )
    Mat flipImg;
    flip(image, flipImg, 1);
    imshow("Flipped image", flipImg);
    imwrite("/Users/zojja/TUM/Exercise2/Exercise2/flip.jpg", flipImg);
    
    // Resize
    Mat resImg;
    resize(image, resImg, Size(256,256),0,0, INTER_NEAREST);
    imshow("Resized image", resImg);
    imwrite("/Users/zojja/TUM/Exercise2/Exercise2/resize.jpg", resImg);
    
    // Rotate 90 clockwise
    Mat rotImg;
    rotate(image, rotImg, ROTATE_90_CLOCKWISE);
    imshow("Rotated image", rotImg);
    imwrite("/Users/zojja/TUM/Exercise2/Exercise2/resize.jpg", rotImg);
    
    // Padding 10px left and bottom with replicate border
    Mat paddImg;
    int border = 10;
    copyMakeBorder(image, paddImg, 0, border, border, 0, BORDER_REPLICATE);
    imshow("Paddded image", paddImg);
    imwrite("/Users/zojja/TUM/Exercise2/Exercise2/padding.jpg", paddImg);
    
   // Create HOG and modify paramenters
    Mat hogImg;
    resize(image, hogImg, Size(640,640),0,0, INTER_NEAREST);
    HOGDescriptor hog;
    hog.winSize = HOGwinSize;
    hog.blockSize = HOGblockSize;
    hog.blockStride = HOGblockStride;
    hog.cellSize = HOGcellSize;
    hog.nbins = HOGnbins;
    
    // Compute HOG descriptors
    vector<float> descriptors;
    
    descriptors = compute_descriptors(image,hog);
    visualizeHOG(hogImg, descriptors, hog, 1);
    
    //TASK 2: DECISION TREES
    
    HOGDescriptor hogTrain;
    hogTrain.winSize = HOGwinSize;
    hogTrain.blockSize = HOGblockSize;
    hogTrain.blockStride = HOGblockStride;
    hogTrain.cellSize = HOGcellSize;
    hogTrain.nbins = HOGnbins;
    
    vector<float> train_descriptor;
    Mat train_descriptors;
    
    // Iterate over folder
    for(int i = 0; i < num_of_folders; i++){
        String train_path = "/Users/zojja/TUM/Exercise2/Exercise2/data/task2/train/0" + to_string(i) + "/";
        vector<String> fn;
        glob(train_path, fn, false);
        
        size_t count = fn.size(); // number of jpg files in images folder
        
        // Iterate over images
        for (int j = 0; j < count; j++){
            Mat imageTrain = imread(fn[i]); //read image
            train_descriptor = compute_descriptors(imageTrain, hogTrain);
            Mat descriptor_mat = Mat(train_descriptor).t();
            train_descriptors.push_back(descriptor_mat);
        }
    }
    cout<<train_descriptors.size()<<endl;
    
    waitKey(0);
    return 0;
}
