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


int main( int argc, char** argv ){
    // TASK 1: OPEN CV AND HOG DESCRIPTORS
  /*  // Load image
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
    
 /*    //TASK 2: DECISION TREES

   // Prepare the labels and data for training with HOG Descriptors
    vector<float> train_descriptor;
    Mat train_descriptors;
    Mat train_labels;
    HOGDescriptor hogTrain(HOGwinSize, HOGblockSize, HOGblockStride, HOGcellSize, HOGnbins);
    
    train_tree("/Users/zojja/TUM/Exercise2/Exercise2/data/task2/train/", num_of_folders, hogTrain, HOGwinSize);

    // Predict labels
    cout<<"predict decision tree starts."<<endl;
    cout<<endl;
    int counter = 0;
    for(int i = 0; i < num_of_folders; i++){
        String test_path = "/Users/zojja/TUM/Exercise2/Exercise2/data/task2/test/0"+std::to_string(i)+"/";
        
        //prepare the data for testing
        vector<float> test_descriptor;
        Mat test_descriptors;
        
        vector<String> fn;
        glob(test_path, fn, false);
        
        // Compute descriptors of test images
        for(int j = 0; j < test_samples; j++){
            Mat image = imread(fn[j]);
            test_descriptor = compute_descriptors(image, hogTrain, HOGwinSize);
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

   // task 2 ///////////////random forest classifier///////////////
    
    cout << endl;
    cout << "Random Forest Classifier." << endl;
    cout << endl;

    randomForest Forest[num_of_trees];
    int correct_class = 15;
    
    // Random forest with 100 trees
    for (int t = 0; t < num_of_trees; t++) {
        //srand((unsigned) time(nullptr));
        
        // Train data
        vector<float> train_descriptor;
        Mat train_descriptors;
        Mat train_labels;
        int *rand_index;
        
        for (int i = 0; i < num_of_folders; i++) {
            String train_path = "/Users/zojja/TUM/Exercise2/Exercise2/data/task2/train/0" + std::to_string(i) + "/";
            vector<String> fn;
            glob(train_path, fn, false);
            size_t count = fn.size(); //number of files per folder
            
            rand_index = randperm((int)count / 3, 0, (int)count- 1);
            train_labels.push_back(cv::Mat::ones((int)count / 3, 1, CV_32S) * i);
            
            // Each tree has 1/3 of all training data in folder distributed randomly
            for (int j = 0; j < (int)count / 3; j++) {
                int k = rand_index[j];
                Mat image = imread(fn[k]);
                train_descriptor = compute_descriptors(image, hogTrain, HOGwinSize);
                Mat descriptor_mat = cv::Mat(train_descriptor).t();
                train_descriptors.push_back(descriptor_mat);
            }
            delete[] rand_index;
        }
        
        // Create and train decision tree for RF
        Ptr<TrainData> tData = TrainData::create(train_descriptors, ROW_SAMPLE, train_labels);
        Ptr<DTrees> random_forest = Forest[t].creatRF();
        Forest[t].trainRF(random_forest, tData);
        
        // Test data
        vector<float> test_descriptor;
        Mat test_descriptors;
        Mat test_labels;
        
        for (int i = 0; i < 6; i++) {
            cv::String test_path = "/Users/zojja/TUM/Exercise2/Exercise2/data/task2/test/0" + std::to_string(i) + "/";
            vector<String> fn;
            glob(test_path, fn, false);
            test_labels.push_back(cv::Mat::ones(test_samples, 1, CV_32FC1) * i);
            
            for (int j = 0; j < test_samples; j++) {
                Mat image = imread(fn[j]);
                test_descriptor = compute_descriptors(image, hogTrain, HOGwinSize);
                Mat descriptor_mat = cv::Mat(test_descriptor).t();
                test_descriptors.push_back(descriptor_mat);
            }
        }
        
        // Predict Classes
        Mat test_result;
        random_forest->predict(test_descriptors, test_result);
        cout << "predicting for " + std::to_string(t) + ".th tree is done." << endl;
        
        // Save DT
        ofstream outFile;
        outFile.open("/Users/zojja/TUM/Exercise2/Exercise2/data/task2/rftree_" + std::to_string(t) + "_result.txt");
        
        for (int i = 0; i < test_samples * num_of_folders; i++) {
            outFile << ((float *) test_result.data)[i] << endl;
        }
        outFile.close();
        
        int counter = 0;
        
        // Print predictions (if values are the same ,prediction is correct, add to counter)
        for (int k = 0; k < test_samples * num_of_folders - 1; ++k) {
            float value1 = ((float *) test_labels.data)[k];
            float value2 = ((float *) test_result.data)[k];
            fprintf(stdout, "actual class: %f, expected class: %f\n", value1, value2);
            if (int(value1) == int(value2)) {
                ++counter;
            }
        }
        
        fprintf(stdout, "total classification accuracy: %f\n", counter * 1.f / (test_samples * 6));
        cout << endl;
    }
    
    //
    float vote_all_trees[test_samples * num_of_folders][num_of_trees];
    vote_all_trees[test_samples * num_of_folders][num_of_trees] = {0};
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
            if (image_index >= test_samples * num_of_folders || tree_index >= num_of_trees) {
                break;
            }
            image_index = image_index + 1;
        }
        tree_index = tree_index + 1;
        image_index = 0;
        rftree.close();
    }
    
    // vote
    Mat vote_labels;
    for (int i = 0; i < num_of_trees; i++) {
        vote_labels.push_back(Mat::ones(test_samples, 1, CV_32FC1) * i);
    }
    
    float vote_one_tree[num_of_trees];
    
    for (int i = 0; i < test_samples * num_of_folders; i++) {
        for (int j = 0; j < num_of_trees; j++) {
            vote_one_tree[j] = vote_all_trees[i][j];
        }
        int vote_index = vote_for_result(vote_one_tree);
        
        if (((float *) vote_labels.data)[i] == vote_one_tree[vote_index]) {
            correct_class++;
        }
        
    }
    cout << endl;
    fprintf(stdout, "total classification accuracy using random forest: %f\n", correct_class * 1.f / (test_samples * 6));*/
    
    //task 3 - object detection
    
    
    cout << endl;
    cout << "Object detection" << endl;
    cout << endl;
    
    // 3.1 Build test data set using sliding window
    HOGDescriptor hogTrain(HOGwinSize, HOGblockSize, HOGblockStride, HOGcellSize, HOGnbins);
    
    Size window_size = Size(60,60);
    int window_step = 20;
    int scale = 1; // if need o be changed -> resize the image?
    
    
    // test images folder
    String test_path = "/Users/zojja/TUM/Exercise2/Exercise2/data/task3/test/";
    vector<String> fn;
    glob(test_path, fn, false);
    size_t count_test_images = fn.size();
    
    vector<Mat> test_images;
    vector<int> samples_per_image;
    vector<Rect> sample_rect;
    
    int total_samples_num = 0;
    
    randomForest Forest[num_of_trees];
    int correct_class = 15;
    Mat train_descriptors;
    Mat train_labels;
    vector<float> train_descriptor;
    
    // Augment and train images
    for (int i = 0; i < 4; i++) {
        String train_path = "/Users/zojja/TUM/Exercise2/Exercise2/data/task3/train/0" + std::to_string(i) + "/";
        vector<String> fn;
        glob(train_path, fn, false);
        size_t count = fn.size(); //number of files per folder
            
        train_labels.push_back(cv::Mat::ones((int)count, 1, CV_32S) * i);
            
        // Each
        for (int j = 0; j < count; j++) {
            Mat image = imread(fn[j]);
            train_descriptor = compute_descriptors(image, hogTrain, HOGwinSize);
            Mat descriptor_mat = cv::Mat(train_descriptor).t();
            train_descriptors.push_back(descriptor_mat);
            //Augment training data with flip, rotation, grayscale
            for(int k = -1; k <= 1; k++){
                Mat flipImg;
                flip(image, flipImg, k);
                Mat descriptor_mat_flip = cv::Mat(train_descriptor).t();
                train_descriptors.push_back(descriptor_mat_flip);
                train_labels.push_back(1);
            }
                
            for(int k = 0; k <= 2; k++){
                Mat rotImg;
                rotate(image, rotImg, k);
                Mat descriptor_mat_rot = cv::Mat(train_descriptor).t();
                train_descriptors.push_back(descriptor_mat_rot);
                train_labels.push_back(1);
            }
                
            Mat grayImg;
            cvtColor(image, grayImg, COLOR_BGR2GRAY );
            Mat descriptor_mat_gray = cv::Mat(train_descriptor).t();
            train_descriptors.push_back(descriptor_mat_gray);
            train_labels.push_back(1);
        }
    }
    
    train_labels.convertTo(train_labels,CV_32S);
    Ptr<TrainData> tData = TrainData::create(train_descriptors, ROW_SAMPLE, train_labels);
    
    // Create and train Random Forest with 40 trees
    for(int t = 0; t < 40; t++){
        Ptr<DTrees> random_forest = Forest[t].creatRF();
        Forest[t].trainRF(random_forest, tData);
        
    }
    
    // go through all test images
    for (int i = 0; i < count_test_images; i++) {
        
        Mat image = imread(fn[i]);
        
        // cut every test image into windows for testing
        
        int x = 0; // x of subimage corner
        int y = 0; // y of subimage corner
        int num_samples = 0;
        
        while ((y + window_size.height) <= image.size[1]){
            while ((x + window_size.width) <= image.size[0])
            {
                Rect subimg_rect = Rect(x, y, window_size.width, window_size.height);
                Mat subimage = image(subimg_rect);
                test_images.push_back(subimage);
                sample_rect.push_back(subimg_rect);
                num_samples ++;
                x += window_step;
            }
            y += window_step;
        }
        total_samples_num += num_samples;
        samples_per_image.push_back(num_samples);
    }
    
    // 3.2 Build decision trees using training data set
    // All the same as in task 2
    
   
    
    //
    float vote_all_trees[total_samples_num][num_of_trees];
    vote_all_trees[total_samples_num][num_of_trees] = { 0 };
    int tree_index = 0;
    int image_index = 0;
    
    for (int i = 0; i < num_of_trees; i++) {
        std::ifstream rftree;
        rftree.open("/Users/zojja/TUM/Exercise2/Exercise2/data/task3/rftree_" + std::to_string(i) + "_result.txt");
        if (!rftree) {
            cout << "tree not found." << endl;
            exit(1);
        }
        
        while (!rftree.eof()) {
            rftree >> vote_all_trees[image_index][tree_index];
            if (image_index >= total_samples_num || tree_index >= num_of_trees) {
                break;
            }
            image_index = image_index + 1;
        }
        tree_index = tree_index + 1;
        image_index = 0;
        rftree.close();
    }
    
    // vote
    Mat vote_labels;
    for (int i = 0; i < num_of_trees; i++) {
        vote_labels.push_back(Mat::ones(test_samples, 1, CV_32FC1) * i);
    }
    
    float vote_one_tree[num_of_trees];
    
    for (int i = 0; i < total_samples_num; i++) {
        for (int j = 0; j < num_of_trees; j++) {
            vote_one_tree[j] = vote_all_trees[i][j];
        }
        int vote_index = vote_for_result(vote_one_tree);
        
        if (((float *)vote_labels.data)[i] == vote_one_tree[vote_index]) {
            correct_class++;
        }
        
    }
    cout << endl;
    fprintf(stdout, "total classification accuracy using random forest: %f\n", correct_class * 1.f / (test_samples * 6));
    
    // 3.3 Go through all predictions - store only ones with value higher than threshold
	int *pick = non_max_suppression(boundingRect, threshold);
    
/*    // 3.4 Look for intersections inside one image
    
    for (int i = 0; i < test_samples; i++)
    {
        for (int j = 0; j < samples_per_image.at(i); j++)
        {
            for (int k = j; j < samples_per_image.at(i); k++)
            {
                //!!!
                //if sample_rect.at(i*samples_per_image.at(i)+j) intersects with sample_rect.at(i*samples_per_image.at(i) + j)
                
                // then save only the one with prediction with higher accuracy!
            }
        }
    }

    }*/
    
    //Plotting of the boxes after NMS
	for (int i = 0; i < sizeof(pick); i++) {
		cv::rectangle(image, (pick[i][0], pick[i][1]), (pick[i][2], pick[i][3]). (0, 255, 0), 2);
	}
	cv::imshow("After NMS", image)

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
