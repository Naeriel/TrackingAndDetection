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
#define    test_samples    10

// Compute HOG descriptor
vector<float> compute_descriptors(Mat image, HOGDescriptor hog){
    Mat hogImg;
    static vector<float> descriptor;
    // resize original imageto 640x640
    resize(image, hogImg, HOGwinSize,0,0,CV_INTER_LINEAR);
    
    // Compute HOG descriptors
    hog.compute(hogImg, descriptor);
    
    return descriptor;
}


int main( int argc, char** argv ){
  /*  // TASK 1: OPEN CV AND HOG DESCRIPTORS
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
    visualizeHOG(hogImg, descriptors, hog, 1);*/
    
    //TASK 2: DECISION TREES
    
    HOGDescriptor hogTrain(HOGwinSize, HOGblockSize, HOGblockStride, HOGcellSize, HOGnbins);
    
    // Prepare the labels for training (matrix of ones)
    int  train_samples_number[6] = {49,67,42,53,67,110} ; //图片个数
    Mat train_labels;
    
    for(int i = 0; i < 6; i++){
        train_labels.push_back(Mat::ones(train_samples_number[i],1,CV_32S) * i);
    }
    
    // Prepare the data for training with HOG Descriptors
    vector<float> train_descriptor;
    Mat train_descriptors;
 
    // Iterate over folders
    for(int i = 0; i < num_of_folders; i++){
        String train_path = "//Users/zojja/TUM/Exercise2/Exercise2/data/task2/train/0"+std::to_string(i)+"/";
        vector<String> fn;
        glob(train_path, fn, false);
        
        // Iterate over images (with random filenames)
        for(int j=0;j<train_samples_number[i];j++){
            Mat image = imread(fn[j]);
            train_descriptor = compute_descriptors(image, hogTrain); // Compute descriptors for Training images
            Mat descriptor_mat = Mat(train_descriptor).t(); // Add to descriptor Mat
            train_descriptors.push_back(descriptor_mat);
        }
    }
    
    // Training of Decision Tree
    Ptr<TrainData> tData = TrainData::create(train_descriptors, ROW_SAMPLE, train_labels);
    
    // Create and set parameters Decision Tree
    Ptr<DTrees> dtree = DTrees::create();
    
    dtree->setMaxDepth(10);
    dtree->setMinSampleCount(10);
    dtree->setMaxCategories(6);
    dtree->setCVFolds(0);
    dtree->train(tData);
    cout<<"decision tree training done."<<endl<<endl;
    dtree->save("/Users/zojja/TUM/Exercise2/Exercise2/data/task2/train/trained_dtree.xml");

    // Predict labels
    cout<<"predict decision tree starts."<<endl;
    cout<<endl;
    int counter = 0;
    for(int i=0;i<6;i++){
        String test_path = "/Users/zojja/TUM/Exercise2/Exercise2/data/task2/test/0"+std::to_string(i)+"/";
        
        //prepare the data for testing
        vector<float> test_descriptor;
        Mat test_descriptors;
        
        vector<String> fn;
        glob(test_path, fn, false);
        
        // Compute descriptors of test images
        for(int j = 0; j < test_samples; j++){
            Mat image = imread(fn[j]);
            test_descriptor = compute_descriptors(image, hogTrain);
            Mat descriptor_mat = cv::Mat(test_descriptor).t();
            test_descriptors.push_back(descriptor_mat);
        }
        
        // Prepare labels and results using Dtree->predict
        Mat test_result;
        Mat test_labels;
        
        Ptr<DTrees> Dtree = DTrees::load("/Users/zojja/TUM/Exercise2/Exercise2/data/task2/train/trained_dtree.xml");
        Dtree->predict(test_descriptors,test_result);
        test_labels.push_back(Mat::ones(test_samples,1,CV_32FC1) * i);
        
        // Show results
        for(int j = 0; j <test_samples; j++) {
            float value1 = ((float *) test_labels.data)[j];
            float value2 = ((float *) test_result.data)[j];
            fprintf(stdout, "actual class: %f, expected class: %f\n", value1, value2);
            if (int(value1) == int(value2)) {
                ++counter;
            }
        }
    }
    cout<<endl;
    fprintf(stdout, "total classification accuracy using decision tree: %f\n", counter * 1.f/60);
    cout<<endl;

  /*  // task 2 ///////////////random forest classifier///////////////
    
    cout << endl;
    cout << "random forest starts." << endl;
    cout << endl;

    randomForest Forest[num_of_trees];
    int correct_class = 15;
    
    for (int t = 0; t < num_of_trees; t++) {
        srand((unsigned) time(nullptr));
        // size of subset of training data. each class use 1/3 data to train
        int subset_train[6] = {17, 22, 14, 17, 22, 36};
        int range_train[6] = {49, 67, 42, 53, 67, 110};
        int *rand_index;
        
        std::vector<float> train_descriptor;
        cv::Mat train_descriptors;
        cv::Mat train_resize_image;
        
        //prepare labels
        cv::Mat train_labels;
        
        for (int i = 0; i < 6; i++) {
            rand_index = randperm(subset_train[i], 0, range_train[i] - 1);
            
            train_labels.push_back(cv::Mat::ones(subset_train[i], 1, CV_32S) * i);
            
            cv::String train_path = "/Users/zojja/TUM/Exercise2/Exercise2/data/task2/train/0" + std::to_string(i) + "/";
            vector<String> fn;
            glob(train_path, fn, false);
            for (int j = 0; j < subset_train[i]; j++) {
                cv::Mat sub_train;
                sub_train = imread(fn[i]);
                
                cv::Mat resize_image;
                cv::resize(sub_train, resize_image, cv::Size(640, 640), 0, 0, CV_INTER_LINEAR);
                // 计算HOG descriptor
                cv::HOGDescriptor hog(cv::Size(640, 640), cv::Size(80, 80), cv::Size(80, 80), cv::Size(80, 80), 9);
                hog.compute(resize_image, train_descriptor);
                cv::Mat descriptor_mat = cv::Mat(train_descriptor).t();
                train_descriptors.push_back(descriptor_mat);
            }
            delete[] rand_index;
        }
        
        cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(train_descriptors, cv::ml::ROW_SAMPLE,
                                                                     train_labels);
        cv::Ptr<cv::ml::DTrees> random_forest = Forest[t].creatRF();
        Forest[t].trainRF(random_forest, tData);
        cout << "training for " + std::to_string(t) + ".th tree is done." << endl;
        
        //prepare the data for testing
        std::vector<float> test_descriptor;
        cv::Mat test_descriptors;
        cv::Mat test_resize_image;
        int test_samples_number = 10;
        cv::Mat test_labels;
        
        for (int i = 0; i < 6; i++) {
            test_labels.push_back(cv::Mat::ones(test_samples_number, 1, CV_32FC1) * i);
            cv::String test_path = "/Users/zojja/TUM/Exercise2/Exercise2/data/task2/test/0" + std::to_string(i) + "/";
            vector<String> fn;
            glob(test_path, fn, false);
            for (int j = 0; j < test_samples_number; j++) {
                
               cv::Mat image = imread(fn[i]);
                // resize to 640*640
                cv::resize(image, test_resize_image, cv::Size(640, 640), 0, 0, CV_INTER_LINEAR);
                // 计算HOG descriptor
                cv::HOGDescriptor hog(cv::Size(640, 640), cv::Size(80, 80), cv::Size(80, 80), cv::Size(80, 80), 9);
                hog.compute(test_resize_image, test_descriptor);
                cv::Mat descriptor_mat = cv::Mat(test_descriptor).t();
                test_descriptors.push_back(descriptor_mat);
            }
        }
        
        // predict
        cv::Mat test_result;
        random_forest->predict(test_descriptors, test_result);
        cout << "predicting for " + std::to_string(t) + ".th tree is done." << endl;
        
        std::ofstream outFile;
        outFile.open("/Users/zojja/TUM/Exercise2/Exercise2/data/task2/rftree_" + std::to_string(t) + "_result.txt");
        
        for (int i = 0; i < test_samples_number * 6; i++) {
            outFile << ((float *) test_result.data)[i] << endl;
        }
        outFile.close();
        
        int counter = 0;
        
        for (int k = 0; k < test_samples_number * 6 - 1; ++k) {
            float value1 = ((float *) test_labels.data)[k];
            float value2 = ((float *) test_result.data)[k];
            fprintf(stdout, "actual class: %f, expected class: %f\n", value1, value2);
            if (int(value1) == int(value2)) {
                ++counter;
            }
        }
        
        fprintf(stdout, "total classification accuracy: %f\n", counter * 1.f / (test_samples_number * 6));
        cout << endl;
    }
    
    
    int test_samples_number = 10;
    float vote_all_trees[test_samples_number * 6][num_of_trees];
    vote_all_trees[test_samples_number * 6][num_of_trees] = {0};
    int tree_index = 0;
    int image_index = 0;
    
    for (int i = 0; i < num_of_trees; i++) {
        std::ifstream rftree;
        rftree.open("/Users/zojja/TUM/Exercise2/Exercise2/data/task2/rftree_" + std::to_string(i) + "_result.txt");
        if (!rftree) {
            cout << "tree not found." << endl;
            exit(1);
        }
        
        while (!rftree.eof()) {
            rftree >> vote_all_trees[image_index][tree_index];
            if (image_index >= test_samples_number * 6 || tree_index >= num_of_trees) {
                break;
            }
            image_index = image_index + 1;
        }
        tree_index = tree_index + 1;
        image_index = 0;
        rftree.close();
    }
    
    // vote
    
    cv::Mat vote_labels;
    for (int i = 0; i < num_of_trees; i++) {
        vote_labels.push_back(cv::Mat::ones(test_samples_number, 1, CV_32FC1) * i);
    }
    
    // vote_one_tree 100棵树的结果两两比较,统计每一棵树的预测结果在100棵树里出现了多少次
    float vote_one_tree[num_of_trees];
    
    for (int i = 0; i < test_samples_number * 6; i++) {
        for (int j = 0; j < num_of_trees; j++) {
            vote_one_tree[j] = vote_all_trees[i][j];
        }
        
        
        int vote_index = vote_for_result(vote_one_tree);
        //cout << "the classifaction of " + std::to_string(i) + ".th image is " +std::to_string(vote_one_tree[vote_index]);
        
        if (((float *) vote_labels.data)[i] == vote_one_tree[vote_index]) {
            correct_class++;
        }
        
    }
    cout << endl;
    fprintf(stdout, "total classification accuracy using random forest: %f\n", correct_class * 1.f / (test_samples_number * 6));*/
    return 0;
}

int vote_for_result(const float a[]){
    float vote[num_of_trees];
    vote[num_of_trees] = {0};
    for(int i=0;i<num_of_trees;i++){
        for(int j=0;j<num_of_trees;j++){
            if(a[i]==a[j]){
                vote[i] = vote[i] + 1;
            }
        }
    }
    
    //两两比较后输出最大的那个
    float vote_max = 0;
    int vote_max_index=0;
    for(int i=0;i<num_of_trees;i++){
        if(vote[i]>vote_max){
            vote_max = vote[i];
            vote_max_index = i;
        }
    }
    return vote_max_index;
}
