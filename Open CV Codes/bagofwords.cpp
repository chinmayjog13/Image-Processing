#include<stdio.h>
#include<iostream>
#include <vector>
#include <math.h>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include "matplotlibcpp.h"

using namespace cv;
using namespace cv::xfeatures2d;
namespace plt = matplotlibcpp;

int main (int argc, char** argv)
{

    Mat image1 = imread(argv[1], IMREAD_COLOR);
    Mat image2 = imread(argv[2], IMREAD_COLOR);
    Mat image3 = imread(argv[3], IMREAD_COLOR);
    Mat image4 = imread(argv[4], IMREAD_COLOR);

    int features = 0;
    int octave_layers = 3;
    double contrast_threshold = 0.04;
    double edge_threshold = 10;
    double sigma = sqrt(2);

    Mat descriptor;
    Mat features_orig;

    //Ptr<SURF> detector = SURF::create(distance);
    Ptr<SIFT> detector = SIFT::create(features, octave_layers, contrast_threshold, edge_threshold, sigma);
    std::vector <KeyPoint> keypoints;

    detector->detect(image1, keypoints);
    detector->compute(image1, keypoints, descriptor);
    features_orig.push_back(descriptor);

    detector->detect(image2, keypoints);
    detector->compute(image2, keypoints, descriptor);
    features_orig.push_back(descriptor);

    detector->detect(image3, keypoints);
    detector->compute(image3, keypoints, descriptor);
    features_orig.push_back(descriptor);

    int dictionarySize = 8;
    //define Term Criteria
    TermCriteria tc((CV_TERMCRIT_ITER || CV_TERMCRIT_EPS),100,0.001);
    //retries number
    int attempts = 3;
    //necessary flags
    int flags=KMEANS_PP_CENTERS;
    //Create the BoW (or BoF) trainer
    BOWKMeansTrainer bowTrainer(dictionarySize,tc,attempts,flags);
    //cluster the feature vectors
    Mat dictionary = bowTrainer.cluster(features_orig);
    //store the vocabulary
    FileStorage fs("dictionary.yml", FileStorage::WRITE);
    fs << "vocabulary" << dictionary;
    fs.release();

    // Testing ferrari 2
    Ptr<FlannBasedMatcher> Fbmatcher = FlannBasedMatcher::create();
    BOWImgDescriptorExtractor bowDE(detector,Fbmatcher);
    bowDE.setVocabulary(dictionary);
    char * imageTag = new char[10];

    FileStorage fs1("descriptor.yml", FileStorage::WRITE);
    detector->detect(image4, keypoints);
    Mat bowDescriptor_optimus, bowDescriptor_bumblebee, bowDescriptor_ferrari_1, bowDescriptor_ferrari_2;

    bowDE.compute2(image1, keypoints, bowDescriptor_optimus);
    bowDE.compute2(image2, keypoints, bowDescriptor_bumblebee);
    bowDE.compute2(image3, keypoints, bowDescriptor_ferrari_1);
    bowDE.compute2(image4, keypoints, bowDescriptor_ferrari_2);

    //prepare the yml (some what similar to xml) file
    sprintf(imageTag, "img");
    fs1 << imageTag << bowDescriptor_ferrari_2;
    fs1.release();

    std::vector<float> optimus; std::vector<float> bumblebee; std::vector<float> ferrari_1; std::vector<float> ferrari_2;
    std::vector<float> x_axis;
    for (int i = 0; i < 8; i++)
    {
        optimus.push_back(bowDescriptor_optimus.at<float>(0,i));
        bumblebee.push_back(bowDescriptor_bumblebee.at<float>(0,i));
        ferrari_1.push_back(bowDescriptor_ferrari_1.at<float>(0,i));
        ferrari_2.push_back(bowDescriptor_ferrari_2.at<float>(0,i));
        x_axis.push_back(i+1);
    }

    double dist_optimus = 0; double dist_bumblebee = 0; double dist_ferrari = 0;
    for (int j = 0; j < 8; j++)
    {
        dist_optimus += pow((bowDescriptor_optimus.at<float>(0,j) - bowDescriptor_ferrari_2.at<float>(0,j)), 2);
        dist_optimus += pow((bowDescriptor_bumblebee.at<float>(0,j) - bowDescriptor_ferrari_2.at<float>(0,j)), 2);
        dist_optimus += pow((bowDescriptor_ferrari_1.at<float>(0,j) - bowDescriptor_ferrari_2.at<float>(0,j)), 2);
    }
    dist_optimus = sqrt(dist_optimus);
    dist_bumblebee = sqrt(dist_bumblebee);
    dist_ferrari = sqrt(dist_ferrari);

    double match = min(dist_ferrari, min(dist_bumblebee, dist_optimus));
    if (match == dist_ferrari)
        std::cout << "Ferrari 1 is closest match to Ferrari 2 " << std::endl;
    else if (match == dist_bumblebee)
        std::cout << "Bumblebee is closest match to Ferrari 2 " << std::endl;
    else
        std::cout << "Optimus Prime is closest match to Ferrari 2 " << std::endl;

    const std::string f = "ferrari_2";

    bool res = plt::named_hist("bumblebee", bumblebee, 8, "b", 1.0);
    plt::title("Bumblebee");
    plt::show(res);
    plt::figure();
//    plt::figure();
//    plt::named_plot("ferrari 2", x_axis, ferrari_2);
//    plt::named_plot("ferrari 1", x_axis, ferrari_1);
//    plt::named_plot("optimus", x_axis, optimus);
//    plt::named_plot("bumblebee", x_axis, bumblebee);
//    plt::legend();
//    plt::show(true);

    return 0;
}
