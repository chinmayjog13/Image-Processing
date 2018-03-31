/* Key point Detection using SIFT and SURF
   Open CV 3.3.1
   Arguments must be passed as follows- Image(1) Image(2)
   Number of images to extract features from can be changed by making few changes in code
   To use SIFT uncomment SIFT pointer creation line
   To use SURF uncomment SURF pointer creation line
   Use only SIFT or SURF at a time
   Author- Chinmay Jog
*/

#include<stdio.h>
#include<iostream>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

int main (int argc, char** argv)
{
//    if (argc != 2)
//    {
//        std::cout << "Arguments should be passed as image1 image 2 " << std::endl;
//        return -1;
//    }

    Mat image1 = imread(argv[1], IMREAD_COLOR);
    Mat image2 = imread(argv[2], IMREAD_COLOR);

    if (!image1.data || !image2.data)
        std::cout << "Error reading images " << std::endl;

    int distance = 400;
    int features = 100;
    int octave_layers = 3;
    double contrast_threshold = 0.04;
    double edge_threshold = 10;
    double sigma = 2;
    //double sigma = sqrt(2);

    Ptr<SIFT> detector = SIFT::create(features, octave_layers, contrast_threshold, edge_threshold, sigma);
    //Ptr<SURF> detector = SURF::create(distance);
    std::vector <KeyPoint> keypoints1, keypoints2;

    detector->detect(image1, keypoints1);
    detector->detect(image2, keypoints2);

    Mat image_keypoints1;
    Mat image_keypoints2;

    drawKeypoints(image1, keypoints1, image_keypoints1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    drawKeypoints(image2, keypoints2, image_keypoints2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    imshow("keypoints 1", image_keypoints1);
    imshow("keypoints 2", image_keypoints2);

    imwrite("Optimus_Prime_keypoints_Sift_sigma2.jpg", image_keypoints1);
    imwrite("Bumblebee_keypoints_Sift_sigma2.jpg", image_keypoints2);

    waitKey(0);

    return 0;
}
