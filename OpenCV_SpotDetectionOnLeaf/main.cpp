//
//  main.cpp
//  OpenCV_SpotDetectionOnLeaf
//
//  Created by Minhaz on 6/16/15.
//  Copyright (c) 2015 Minhaz. All rights reserved.
//

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat img_src = imread("/Users/minhaz/Leaf-Spot.jpg", CV_8S);
    
    Mat img_gray;
    cvtColor(img_src, img_gray, CV_BGR2GRAY);
    blur(img_gray, img_gray, Size(3,3));
    
    Mat img_canny;
    Canny(img_gray, img_canny, 100, 200, 3);
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(img_canny, contours, hierarchy,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    
    const int spots_min = 10;
    int spots = 0;
    
    Mat img_result = img_src.clone();
    for(unsigned int i = 0; i< contours.size(); i++)
    {
        if(contours.at(i).size() > 10)
        {
            RotatedRect rect = fitEllipse(contours.at(i));
            if(rect.boundingRect().area() < 500)
            {
                spots++;
                
                Scalar color(0, 255, 255);
                circle(img_result, rect.center, 5, color, 5, CV_AA);
            }
        }
    }
    
    stringstream str;
    Scalar fontColor;
    
    if(spots >= spots_min)
    {
        fontColor = Scalar(0,0,255);
        str << "Infected " << spots << " spots";
    }
    else
    {
        fontColor = Scalar(0,255,0);
        str << "Safe " << spots << " spots";
    }
    
    putText(img_result, str.str(), Point(10,30), CV_FONT_NORMAL, 1, fontColor, 2, CV_AA);
    
    /*
     string name_src("Source");
     namedWindow(name_src, CV_WINDOW_AUTOSIZE);
     imshow(name_src, img_src);
     
     string name_result("Result");
     namedWindow(name_result, CV_WINDOW_AUTOSIZE);
     imshow(name_result, img_result);
     */
    
    // Merge both images into one
    Mat img_merged(400, 600, CV_8UC3);
    Mat left(img_merged, Rect(0, 0, 300, 400));
    img_src.copyTo(left);
    Mat right(img_merged, Rect(300, 0, 300, 400));
    img_result.copyTo(right);
    
    string name_merged("Spot Detection on Leaf");
    namedWindow(name_merged, CV_WINDOW_AUTOSIZE);
    imshow(name_merged, img_merged);
    
    waitKey(0);
    
    return 0;
}
