/* Image Matching using SIFT and SURF
   Open CV 3.3.1
   Arguments must be passed as follows- Image(1) Image(2)
   To use SIFT uncomment SIFT pointer creation line
   To use SURF uncomment SURF pointer creation line
   Use only SIFT or SURF at a time
   Author- Chinmay Jog
*/



#include<stdio.h>
#include<iostream>
#include <vector>
#include <math.h>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

int main (int argc, char** argv)
{

    Mat image1 = imread(argv[1], IMREAD_GRAYSCALE);
    Mat image2 = imread(argv[2], IMREAD_GRAYSCALE);

    int features = 50;
    int octave_layers = 3;
    double contrast_threshold = 0.04;
    double edge_threshold = 10;
    double sigma = sqrt(2);

    int distance = 4000;
    //Ptr<SURF> detector = SURF::create(distance);
    Ptr<SIFT> detector = SIFT::create(features, octave_layers, contrast_threshold, edge_threshold, sigma);
    std::vector <KeyPoint> keypoints1, keypoints2;

    detector->detect(image1, keypoints1);
    detector->detect(image2, keypoints2);

    Mat descriptor1, descriptor2;

    detector->compute(image1, keypoints1, descriptor1);
    detector->compute(image2, keypoints2, descriptor2);

    BFMatcher matcher;
    std::vector<DMatch> matches;
    matcher.match(descriptor1, descriptor2, matches);

    Mat image_matched;
    drawMatches(image1, keypoints1, image2, keypoints2, matches, image_matched);

    imshow("matched image", image_matched);

    imwrite("Matched_Image_Sift_grey.jpg", image_matched);

    waitKey(0);

    return 0;
}
