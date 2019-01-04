//
//  main.cpp
//  
//
//  Created by Cristina Serrano González on 27.12.18.
//

#include "main.hpp"

int main( int argc, char** argv ){
    
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
    hog.winSize = hogImg.size();
    hog.blockSize = Size(80, 80);
    hog.blockStride = Size(80, 80);
    hog.cellSize = Size(80, 80);
    hog.nbins = 9;
    
    // Compute HOG descriptors
    vector<Point> positions;
    vector<float> descriptor;
    hog.compute(hogImg, descriptor);
    visualizeHOG(hogImg, descriptor, hog, 1);
    waitKey(0);                                         
    return 0;
}
