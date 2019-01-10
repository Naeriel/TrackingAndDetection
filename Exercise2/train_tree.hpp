//
//  train_tree.hpp
//  Exercise2
//
//  Created by Cristina Serrano González on 10.01.19.
//  Copyright © 2019 Cristina Serrano González. All rights reserved.
//

#ifndef train_tree_h
#define train_tree_h
#define    HOGwinSize      cv::Size(640,640)

void train_tree(cv::String path, int num_folders, cv::HOGDescriptor hog_train, cv::Size winSize);
std::vector<float> compute_descriptors(cv::Mat image, cv::HOGDescriptor hog, cv::Size winSize);

void train_tree(cv::String path, int num_folders, cv::HOGDescriptor hog_train, cv::Size winSize){
    std::vector<float> train_descriptor;
    cv::Mat train_descriptors;
    cv::Mat train_labels;
    
    for(int i = 0; i < num_folders; i++){
        cv::String train_path = path +"0" + std::to_string(i) + "/";
        std::vector<cv::String> fn;
        glob(train_path, fn, false);
        size_t count = fn.size(); //number of files per folder
        
        train_labels.push_back(cv::Mat::ones((int)count, 1, CV_32S) * i);
        // Iterate over images (with random filenames)
        for(int j = 0; j < count; j++){
            cv::Mat image = imread(fn[j]);
            train_descriptor = compute_descriptors(image, hog_train, winSize); // Compute descriptors for Training images
            cv::Mat descriptor_mat = cv::Mat(train_descriptor).t(); // Add to descriptor Mat
            train_descriptors.push_back(descriptor_mat);
        }
    }
    
    // Training of Decision Tree
    cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(train_descriptors, cv::ml::ROW_SAMPLE, train_labels);
    
    // Create and set parameters Decision Tree
    cv::Ptr<cv::ml::DTrees> dtree = cv::ml::DTrees::create();
    
    dtree->setMaxDepth(10);
    dtree->setMinSampleCount(10);
    dtree->setMaxCategories(6);
    dtree->setCVFolds(0);
    dtree->train(tData);
    std::cout<<"decision tree training done."<<std::endl;
    cv:: String save_path = path + "trained_dtree.xml";
    dtree->save(save_path);
}

#endif /* train_tree_h */
