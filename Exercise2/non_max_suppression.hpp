#include <iostream>
#include<bits/stdc++.h> 
#include <algorithm>

using namespace std;

int *non_max_suppression(int[] boxes, float overlapThreshold);
    
int *non_max_suppression(int[] boxes, float overlapThreshold){
    //if there are no boxes, return an empty list
	if (sizeof(boxes) == 0)
		return {};
	
	//initialize the list of picked indexes
	vector<int> pick;
	
	//grab the coordinates of the bounding boxes
	int x1[sizeof(boxes)] = {};
	int y1[sizeof(boxes)] = {};
	int x2[sizeof(boxes)] = {};
	int y2[sizeof(boxes)] = {};
	for (int i=0; i<sizeof(boxes); i++){
	    x1[i] = boxes[i][0];
	    y1[i] = boxes[i][1];
	    x2[i] = boxes[i][2];
	    y2[i] = boxes[i][3];
	}
	
	// compute the area of the bounding boxes and sort the bounding
	// boxes by the bottom-right y-coordinate of the bounding box
	int area[sizeof(boxes)] = {};
	for(int i=0; i<sizeof(boxes); i++){
	    area[i] = (x2[i] - x1[i] + 1) * (y2[i] - y1[i] + 1);
	}
	
	//idxs = np.argsort(y2)
	//the indexes of the sorted coordinates of the bottom-right y-coordinate of the bounding boxes.
	// we need it to compute the overlap ratio of other bounding boxes.
	int sortedy2[sizeof(boxes)];
	std::copy(y2, y2+sizeof(boxes), sortedy2);
	int n = sizeof(sortedy2)/sizeof(sortedy2[0]); 
	sort(sortedy2, sortedy2+n); 
	vector<int> idxs; 
	for (int i=0; i<sizeof(boxes); i++){
	    for (int j=0; j<sizeof(boxes); j++){
	        if (sortedy2[i] == y2[j]){
	            sortedy2[i] = -1;   //in order to prevent duplication
	            idxs.push_back(j); 
	        }
	    }
	}
	
	//keep looping while some indexes still remain in the indexes array
	while (idxs.size() > 0){
	    int last = idxs.size() - 1;
	    int i =  idxs.at(last);
	    pick.push_back(i); 
	    vector<int> suppress = {last};
	    
	    //loop over all indexes in the indexes list
	    for (int pos=0; pos<last; pos++){
	        //grab the current index
	        int j = idxs.at(pos);
	        
	        // find the largest (x, y) coordinates for the start of
			// the bounding box and the smallest (x, y) coordinates
			// for the end of the bounding box
	        int xx1 = std::max(x1[i], x1[j]);
	        int yy1 = std::max(y1[i], y1[j]);
	        int xx2 = std::min(x2[i], x2[j]);
	        int yy2 = std::min(y2[i], y2[j]);
	        
	        // compute the width and height of the bounding box
	        int w = std::max(0, xx2 - xx1 + 1);
	        int h = std::max(0, yy2 - yy1 + 1);
	        
	        //compute the ratio of overlap between the computed bounding box and the bounding box in the area list
	        float overlap = float(w * h) / float(area[j]);
	        
	        //if there is sufficient overlap, suppress the current bounding box
			if overlap > overlapThreshold
				suppress.push_back(pos);
	    }
	    // delete all indexes from the index list that are in the suppression list
	    for (int i=0; i<suppress.size(); i++){
	        std::vector<int>::iterator position = std::find(idxs.begin(), idxs.end(), suppress.at(i));
            if (position != idxs.end()) // == myVector.end() means the element was not found
                idxs.erase(position);
	    }
	}

    // return only the bounding boxes that were picked
    int nms[pick.size][4] = {};
    for (int i=0; i<pick.size(); i++){
        nms[i] = boxes[pick.at(i)][0];
	    nms[i] = boxes[pick.at(i)][1];
	    nms[i] = boxes[pick.at(i)][2];
	    nms[i] = boxes[pick.at(i)][3];
    }
    
    return nms;
}
