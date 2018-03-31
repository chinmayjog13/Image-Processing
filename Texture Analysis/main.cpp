/*
/ Author Chinmay Jog
/ Id 6171795819
/ Email cjog@usc.edu
/ Date March 25, 2018
/ EE569 Assignment 3
/ Question 1- Texture segmentation using Law's filter and pca
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

unsigned char** create_2D_array(int xSize, int ySize);
int** create_filter(int f1[3], int f2[3]);
unsigned char** pad(unsigned char** rgb, int xSize, int ySize, int window_size);
double feature(unsigned char** image, int** filter, int x, int y, int window);
double avg_filter(double** rgb, int x, int y, int window);
double** double_pad(double** rgb, int xSize, int ySize, int window_size);
double** create_2D_array_double(int xSize, int ySize);
Mat compressPCA_vector(Mat& pcaset, int maxComponents);
Mat compressPCA_value(Mat& pcaset, int maxComponents);
unsigned char local_max(double** rgb, int x, int y, int window);

int main(int argc, char *argv[])

{
    cout << "starting main " << endl;
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int xSize = 512;
	int ySize = 512;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 512]" << endl;
		return 0;
	}

	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	}
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			xSize = atoi(argv[4]);
			ySize = atoi(argv[5]);
		}
	}

	// Allocate image data array
	cout << "starting allocation " << endl;
	unsigned char* Imagedata = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* result = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char** image = create_2D_array(xSize, ySize);
	unsigned char** image_int1 = create_2D_array(xSize+2, ySize+2);

	double** image_ll = create_2D_array_double(xSize+12, ySize+12);
	double** image_le = create_2D_array_double(xSize+12, ySize+12);
	double** image_ls = create_2D_array_double(xSize+12, ySize+12);
	double** image_lw = create_2D_array_double(xSize+12, ySize+12);
	double** image_lr = create_2D_array_double(xSize+12, ySize+12);

	double** image_el = create_2D_array_double(xSize+12, ySize+12);
	double** image_ee = create_2D_array_double(xSize+12, ySize+12);
	double** image_es = create_2D_array_double(xSize+12, ySize+12);
	double** image_ew = create_2D_array_double(xSize+12, ySize+12);
	double** image_er = create_2D_array_double(xSize+12, ySize+12);

	double** image_sl = create_2D_array_double(xSize+12, ySize+12);
	double** image_se = create_2D_array_double(xSize+12, ySize+12);
	double** image_ss = create_2D_array_double(xSize+12, ySize+12);
	double** image_sw = create_2D_array_double(xSize+12, ySize+12);
	double** image_sr = create_2D_array_double(xSize+12, ySize+12);

	double** image_wl = create_2D_array_double(xSize+12, ySize+12);
	double** image_we = create_2D_array_double(xSize+12, ySize+12);
	double** image_ws = create_2D_array_double(xSize+12, ySize+12);
	double** image_ww = create_2D_array_double(xSize+12, ySize+12);
	double** image_wr = create_2D_array_double(xSize+12, ySize+12);

	double** image_rl = create_2D_array_double(xSize+12, ySize+12);
	double** image_re = create_2D_array_double(xSize+12, ySize+12);
	double** image_rs = create_2D_array_double(xSize+12, ySize+12);
	double** image_rw = create_2D_array_double(xSize+12, ySize+12);
	double** image_rr = create_2D_array_double(xSize+12, ySize+12);

	double** feature_ll = create_2D_array_double(600, 450);
	double** feature_le = create_2D_array_double(600, 450);
	double** feature_ls = create_2D_array_double(600, 450);
	double** feature_lw = create_2D_array_double(600, 450);
	double** feature_lr = create_2D_array_double(600, 450);

	double** feature_el = create_2D_array_double(600, 450);
	double** feature_ee = create_2D_array_double(600, 450);
	double** feature_es = create_2D_array_double(600, 450);
	double** feature_ew = create_2D_array_double(600, 450);
	double** feature_er = create_2D_array_double(600, 450);

	double** feature_sl = create_2D_array_double(600, 450);
	double** feature_se = create_2D_array_double(600, 450);
	double** feature_ss = create_2D_array_double(600, 450);
	double** feature_sw = create_2D_array_double(600, 450);
	double** feature_sr = create_2D_array_double(600, 450);

	double** feature_wl = create_2D_array_double(600, 450);
	double** feature_we = create_2D_array_double(600, 450);
	double** feature_ws = create_2D_array_double(600, 450);
	double** feature_ww = create_2D_array_double(600, 450);
	double** feature_wr = create_2D_array_double(600, 450);

	double** feature_rl = create_2D_array_double(600, 450);
	double** feature_re = create_2D_array_double(600, 450);
	double** feature_rs = create_2D_array_double(600, 450);
	double** feature_rw = create_2D_array_double(600, 450);
	double** feature_rr = create_2D_array_double(600, 450);

    double** centroid_vector = new double* [6];
    double** old_vector = new double* [6];
    for (int i = 0; i < 6; i++)
    {
        centroid_vector[i] = new double[25];
        old_vector[i] = new double[25];
    }

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

    //Law's filter
    cout << "starting" << endl;
    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            image[i][j] = Imagedata[i*xSize + j];
        }
    }

    image_int1 = pad(image, xSize, ySize, 3);

    int L[5] = {1, 4, 6, 4, 1};
    int E[5] = {-1, -2, 0, 2, 1};
    int S[5] = {-1, 0, 2, 0, -1};
    int W[5] = {-1, 2, 0, -2, 1};
    int R[5] = {1, -4, 6, -4, 1};


    int** LL = create_filter(L, L);
    int** LE = create_filter(L, E);
    int** LS = create_filter(L, S);
    int** LW = create_filter(L, W);
    int** LR = create_filter(L, R);
    int** EL = create_filter(E, L);
    int** EE = create_filter(E, E);
    int** ES = create_filter(E, S);
    int** EW = create_filter(E, W);
    int** ER = create_filter(E, R);
    int** SL = create_filter(S, L);
    int** SE = create_filter(S, E);
    int** SS = create_filter(S, S);
    int** SW = create_filter(S, W);
    int** SR = create_filter(S, R);
    int** WL = create_filter(W, L);
    int** WE = create_filter(W, E);
    int** WS = create_filter(W, S);
    int** WW = create_filter(W, W);
    int** WR = create_filter(W, R);
    int** RL = create_filter(R, L);
    int** RE = create_filter(R, E);
    int** RS = create_filter(R, S);
    int** RW = create_filter(R, W);
    int** RR = create_filter(R, R);



    for (int i = 1; i < ySize+1; i++)
    {
        for (int j = 1; j < xSize+1; j++)
        {
            feature_ll[i-1][j-1] = feature(image_int1, LL, i, j, 3)/double(256*256);
            feature_le[i-1][j-1] = feature(image_int1, LE, i, j, 3)/double(96*96);
            feature_ls[i-1][j-1] = feature(image_int1, LS, i, j, 3)/double(64*64);
            feature_lw[i-1][j-1] = feature(image_int1, LW, i, j, 3)/double(96*96);
            feature_lr[i-1][j-1] = feature(image_int1, LR, i, j, 3)/double(256*256);

            feature_el[i-1][j-1] = feature(image_int1, EL, i, j, 3)/double(96*96);
            feature_ee[i-1][j-1] = feature(image_int1, EE, i, j, 3)/double(36*36);
            feature_es[i-1][j-1] = feature(image_int1, ES, i, j, 3)/double(24*24);
            feature_ew[i-1][j-1] = feature(image_int1, EW, i, j, 3)/double(36*36);
            feature_er[i-1][j-1] = feature(image_int1, ER, i, j, 3)/double(96*96);

            feature_sl[i-1][j-1] = feature(image_int1, SL, i, j, 3)/double(64*64);
            feature_se[i-1][j-1] = feature(image_int1, SE, i, j, 3)/double(24*24);
            feature_ss[i-1][j-1] = feature(image_int1, SS, i, j, 3)/double(16*16);
            feature_sw[i-1][j-1] = feature(image_int1, SW, i, j, 3)/double(24*24);
            feature_sr[i-1][j-1] = feature(image_int1, SR, i, j, 3)/double(64*64);

            feature_wl[i-1][j-1] = feature(image_int1, WL, i, j, 3)/double(96*96);
            feature_we[i-1][j-1] = feature(image_int1, WE, i, j, 3)/double(36*36);
            feature_ws[i-1][j-1] = feature(image_int1, WS, i, j, 3)/double(24*24);
            feature_ww[i-1][j-1] = feature(image_int1, WW, i, j, 3)/double(36*36);
            feature_wr[i-1][j-1] = feature(image_int1, WR, i, j, 3)/double(96*96);

            feature_rl[i-1][j-1] = feature(image_int1, RL, i, j, 3)/double(256*256);
            feature_re[i-1][j-1] = feature(image_int1, RE, i, j, 3)/double(96*96);
            feature_rs[i-1][j-1] = feature(image_int1, RS, i, j, 3)/double(64*64);
            feature_rw[i-1][j-1] = feature(image_int1, RW, i, j, 3)/double(96*96);
            feature_rr[i-1][j-1] = feature(image_int1, RR, i, j, 3)/double(256*256);
        }
    }
    cout << "filters done " << endl;

    image_ll = double_pad(feature_ll, xSize, ySize, 13);
    image_le = double_pad(feature_le, xSize, ySize, 13);
    image_ls = double_pad(feature_ls, xSize, ySize, 13);
    image_lw = double_pad(feature_lw, xSize, ySize, 13);
    image_lr = double_pad(feature_lr, xSize, ySize, 13);

    image_el = double_pad(feature_el, xSize, ySize, 13);
    image_ee = double_pad(feature_ee, xSize, ySize, 13);
    image_es = double_pad(feature_es, xSize, ySize, 13);
    image_ew = double_pad(feature_ew, xSize, ySize, 13);
    image_er = double_pad(feature_er, xSize, ySize, 13);

    image_sl = double_pad(feature_sl, xSize, ySize, 13);
    image_se = double_pad(feature_se, xSize, ySize, 13);
    image_ss = double_pad(feature_ss, xSize, ySize, 13);
    image_sw = double_pad(feature_sw, xSize, ySize, 13);
    image_sr = double_pad(feature_sr, xSize, ySize, 13);

    image_wl = double_pad(feature_wl, xSize, ySize, 13);
    image_we = double_pad(feature_we, xSize, ySize, 13);
    image_ws = double_pad(feature_ws, xSize, ySize, 13);
    image_ww = double_pad(feature_ww, xSize, ySize, 13);
    image_wr = double_pad(feature_wr, xSize, ySize, 13);

    image_rl = double_pad(feature_rl, xSize, ySize, 13);
    image_re = double_pad(feature_re, xSize, ySize, 13);
    image_rs = double_pad(feature_rs, xSize, ySize, 13);
    image_rw = double_pad(feature_rw, xSize, ySize, 13);
    image_rr = double_pad(feature_rr, xSize, ySize, 13);

    cout << "starting vectors " << endl;
    Mat feature_vector;
    feature_vector.create(xSize*ySize, 25, CV_64F);
    for (int i = 6; i < ySize+6; i++)
    {
        for (int j = 6; j < xSize+6; j++)
        {
            feature_vector.at<double>((i-6)*xSize+(j-6), 0) = avg_filter(image_ll, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 1) = avg_filter(image_le, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 2) = avg_filter(image_ls, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 3) = avg_filter(image_lw, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 4) = avg_filter(image_lr, i, j, 13);

            feature_vector.at<double>((i-6)*xSize+(j-6), 5) = avg_filter(image_el, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 6) = avg_filter(image_ee, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 7) = avg_filter(image_es, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 8) = avg_filter(image_ew, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 9) = avg_filter(image_er, i, j, 13);

            feature_vector.at<double>((i-6)*xSize+(j-6), 10) = avg_filter(image_sl, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 11) = avg_filter(image_se, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 12) = avg_filter(image_ss, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 13) = avg_filter(image_sw, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 14) = avg_filter(image_sr, i, j, 13);

            feature_vector.at<double>((i-6)*xSize+(j-6), 15) = avg_filter(image_wl, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 16) = avg_filter(image_we, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 17) = avg_filter(image_ws, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 18) = avg_filter(image_ww, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 19) = avg_filter(image_wr, i, j, 13);

            feature_vector.at<double>((i-6)*xSize+(j-6), 20) = avg_filter(image_rl, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 21) = avg_filter(image_re, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 22) = avg_filter(image_rs, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 23) = avg_filter(image_rw, i, j, 13);
            feature_vector.at<double>((i-6)*xSize+(j-6), 24) = avg_filter(image_rr, i, j, 13);
        }
    }
    cout << "feature vector stored " << endl;


    //PCA

    //Mat eigen_vector = compressPCA(feature_vector, 16);
    Mat eigen_vector = compressPCA_vector(feature_vector, 16);
    Mat eigen_value = compressPCA_value(feature_vector, 16);
    cout << "pca reduced " << endl;

    Mat output_vector;
    output_vector.create(xSize*ySize, 16, CV_64F);
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < xSize*ySize; j++)
        {
            output_vector.at<double>(j, i) = eigen_vector.at<double>(i,j)*eigen_value.at<double>(i,0);
        }
    }

    FileStorage fs1("EigenValues.yml", FileStorage::WRITE);
    fs1 << "Eigen Values" << eigen_value;
    fs1.release();
    cout << "Eigen Values saved" << endl;

    for (int i = 0; i < 16; i++)
    {
        centroid_vector[0][i] = output_vector.at<double>(0, i);//feat_max;
        centroid_vector[1][i] = output_vector.at<double>(10000, i);//(feature_vector[2][i] + feature_vector[8][i] + feature_vector[10][i])/double(3);//double(2)/double(3)*(feat_min);
        centroid_vector[2][i] = output_vector.at<double>(25000, i);//(feature_vector[1][i] + feature_vector[4][i] + feature_vector[11][i])/double(3);//double(1)/double(3)*(feat_max - feat_min);
        centroid_vector[3][i] = output_vector.at<double>(450*160, i);
        centroid_vector[4][i] = output_vector.at<double>(450*320, i);
        centroid_vector[5][i] = output_vector.at<double>(450*600-1, i);
    }

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            cout << centroid_vector[i][j] << "\t";
        }
        cout << "\n";
    }

//    double class_1[12][9] = {0};
//    double class_2[12][9] = {0};
//    double class_3[12][9] = {0};
//    double class_4[12][9] = {0};
//    double dist1 = 0;
//    double dist2 = 0;
//    double dist3 = 0;
//    double dist4 = 0;
    int t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0;
//    double mean1[9] = {0}, mean2[9] = {0}, mean3[9] = {0}, mean4[9] = {0};
    double** class_1 = create_2D_array_double(16, 450*600);
    double** class_2 = create_2D_array_double(16, 450*600);
    double** class_3 = create_2D_array_double(16, 450*600);
    double** class_4 = create_2D_array_double(16, 450*600);
    double** class_5 = create_2D_array_double(16, 450*600);
    double** class_6 = create_2D_array_double(16, 450*600);
    cout << "starting k means " << endl;
    for (int iter = 0; iter < 100; iter++)
    {
        double mean1[16] = {0}, mean2[16] = {0}, mean3[16] = {0}, mean4[16] = {0}, mean5[16] = {0}, mean6[16] = {0};
        double dist1, dist2, dist3, dist4, dist5, dist6;

        t1 = 0; t2 = 0; t3 = 0; t4 = 0; t5 = 0; t6 = 0;
        cout << "iter is " << iter << endl;
        for (int i = 0; i < 600*450; i++)
        {
            //cout << i << endl;
            dist1 = 0; dist2 = 0; dist3 = 0; dist4 = 0; dist5 = 0; dist6 = 0;
            for (int j = 0; j < 16; j++)
            {
                dist1 = dist1 + pow((centroid_vector[0][j] - output_vector.at<double>(i, j)), 2);
                dist2 = dist2 + pow((centroid_vector[1][j] - output_vector.at<double>(i, j)), 2);
                dist3 = dist3 + pow((centroid_vector[2][j] - output_vector.at<double>(i, j)), 2);
                dist4 = dist4 + pow((centroid_vector[3][j] - output_vector.at<double>(i, j)), 2);
                dist5 = dist5 + pow((centroid_vector[4][j] - output_vector.at<double>(i, j)), 2);
                dist6 = dist6 + pow((centroid_vector[5][j] - output_vector.at<double>(i, j)), 2);

                old_vector[0][j] = centroid_vector[0][j];
                old_vector[1][j] = centroid_vector[1][j];
                old_vector[2][j] = centroid_vector[2][j];
                old_vector[3][j] = centroid_vector[3][j];
                old_vector[4][j] = centroid_vector[4][j];
                old_vector[5][j] = centroid_vector[5][j];

                mean1[j] = 0; mean2[j] = 0; mean3[j] = 0; mean4[j] = 0; mean5[j] = 0; mean6[j] = 0;
//                class_1[i][j] = 0;
//                class_2[i][j] = 0;
//                class_3[i][j] = 0;
//                class_4[i][j] = 0;
//                class_5[i][j] = 0;
//                class_6[i][j] = 0;
                //cout << "distances calculated for i = " << i << " and j = " << j << endl;
            }
            dist1 = sqrt(dist1);
            dist2 = sqrt(dist2);
            dist3 = sqrt(dist3);
            dist4 = sqrt(dist4);
            dist5 = sqrt(dist5);
            dist6 = sqrt(dist6);
            //cout << "Distances " << dist1 << " " << dist2 << " " << dist3 << " " << dist4 << " " << dist5 << " " << dist6 << endl;
//            cout << "distances calculated for i = " << i << " and j = " << j << endl;
            //cout << "finding min " << endl;
            double temp;
            temp = min(min(min(min(min(dist1,dist2), dist3), dist4), dist5), dist6);
            //cout << "min found" << endl;
            if (temp == dist1)
            {
                for (int j = 0; j < 16; j++)
                {
                    class_1[t1][j] = output_vector.at<double>(i, j);
                }
                t1++;
                //cout << "crashed in temp = dist1, t1 =  " << t1 << endl;
            }
            else if (temp == dist2)
            {
                for (int j = 0; j < 16; j++)
                {
                    class_2[t2][j] = output_vector.at<double>(i, j);
                }
                t2++;
                //cout << "crashed in temp = dist2, t2 =  " << t2 << endl;
            }
            else if (temp == dist3)
            {
                for (int j = 0; j < 16; j++)
                {
                    class_3[t3][j] = output_vector.at<double>(i, j);
                }
                t3++;
                //cout << "crashed in temp = dist3, t3 =  " << t3 << endl;
            }
            else if (temp == dist4)
            {
                for (int j = 0; j < 16; j++)
                {
                    class_4[t4][j] = output_vector.at<double>(i, j);
                }
                t4++;
                //cout << "crashed in temp = dist3, t3 =  " << t3 << endl;
            }
            else if (temp == dist5)
            {
                for (int j = 0; j < 16; j++)
                {
                    class_5[t5][j] = output_vector.at<double>(i, j);
                }
                t5++;
                //cout << "crashed in temp = dist5, t5 =  " << t5 << endl;
            }
            else //if (temp == dist4)
            {
                for (int j = 0; j < 16; j++)
                {
                    class_6[t6][j] = output_vector.at<double>(i, j);
                }
                t6++;
                //cout << "crashed in temp = dist6, t6 =  " << t6 << endl;
            }
        }
        cout << "classes allocated " << endl;

        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < t1; j++)
            {
                mean1[i] = mean1[i] + class_1[j][i];
            }
            mean1[i] = mean1[i]/double(t1);

            for (int j = 0; j < t2; j++)
            {
                mean2[i] = mean2[i] + class_2[j][i];
            }
            mean2[i] = mean2[i]/double(t2);

            for (int j = 0; j < t3; j++)
            {
                mean3[i] = mean3[i] + class_3[j][i];
            }
            mean3[i] = mean3[i]/double(t3);

            for (int j = 0; j < t4; j++)
            {
                mean4[i] = mean4[i] + class_4[j][i];
            }
            mean4[i] = mean4[i]/double(t4);

            for (int j = 0; j < t5; j++)
            {
                mean5[i] = mean5[i] + class_5[j][i];
            }
            mean5[i] = mean5[i]/double(t5);

            for (int j = 0; j < t6; j++)
            {
                mean6[i] = mean6[i] + class_6[j][i];
            }
            mean6[i] = mean6[i]/double(t6);
        }
        for (int i = 0; i < 16; i++)
        {
            centroid_vector[0][i] = mean1[i];
            centroid_vector[1][i] = mean2[i];
            centroid_vector[2][i] = mean3[i];
            centroid_vector[3][i] = mean4[i];
            centroid_vector[4][i] = mean5[i];
            centroid_vector[5][i] = mean6[i];
        }
        cout << "means calculated " << endl;
        int counter = 0;
        double eps = 1;
        for (int i = 0; i < 16; i ++)
        {
            //cout << mean1[i] << "\t";
            if ((abs(centroid_vector[0][i] - old_vector[0][i]) <= eps && abs(centroid_vector[1][i] - old_vector[1][i]) <= eps && abs(centroid_vector[2][i] == old_vector[2][i]) <= eps && abs(centroid_vector[3][i] == old_vector[3][i]) <= eps && abs(centroid_vector[4][i] - old_vector[4][i]) <= eps && abs(centroid_vector[5][i] - old_vector[5][i]) <= eps) || iter == 99)
            {
                counter++;
            }
        }
        if (counter >= 11)
        {
            for (int i = 0; i < 450*600; i++)
                {
                    dist1 = 0; dist2 = 0; dist3 = 0; dist4 = 0; dist5 = 0; dist6 = 0;
                    for (int j = 0; j < 16; j++)
                    {
//                        cout << centroid_vector[0][j] << "\t";
                        dist1 = dist1 + pow((centroid_vector[0][j] - output_vector.at<double>(i, j)), 2);
                        dist2 = dist2 + pow((centroid_vector[1][j] - output_vector.at<double>(i, j)), 2);
                        dist3 = dist3 + pow((centroid_vector[2][j] - output_vector.at<double>(i, j)), 2);
                        dist4 = dist4 + pow((centroid_vector[3][j] - output_vector.at<double>(i, j)), 2);
                        dist5 = dist5 + pow((centroid_vector[4][j] - output_vector.at<double>(i, j)), 2);
                        dist6 = dist6 + pow((centroid_vector[5][j] - output_vector.at<double>(i, j)), 2);
                    }
                    dist1 = sqrt(dist1);
                    dist2 = sqrt(dist2);
                    dist3 = sqrt(dist3);
                    dist4 = sqrt(dist4);
                    dist5 = sqrt(dist5);
                    dist6 = sqrt(dist6);

                    double temp;
                    temp = min(min(min(min(min(dist1,dist2), dist3), dist4), dist5), dist6);
                    if (temp == dist1)
                    {
                        result[i] = 0;
                        //cout << " Image " << i+1 << " has texture " << 1 << endl;
                        iter = 100;
                    }
                    else if (temp == dist2)
                    {
                        result[i] = 51;
                        //cout << " Image " << i+1 << " has texture " << 2 << endl;
                        iter = 100;
                    }
                    else if (temp == dist3)
                    {
                        result[i] = 102;
                        //cout << " Image " << i+1 << " has texture " << 3 << endl;
                        iter = 100;
                    }
                    else if (temp == dist4)
                    {
                        result[i] = 153;
                        //cout << " Image " << i+1 << " has texture " << 4 << endl;
                        iter = 100;
                    }
                    else if (temp == dist5)
                    {
                        result[i] = 204;
                        //cout << " Image " << i+1 << " has texture " << 4 << endl;
                        iter = 100;
                    }
                    else if (temp == dist6)
                    {
                        result[i] = 255;
                        //cout << " Image " << i+1 << " has texture " << 4 << endl;
                        iter = 100;
                    }
                }
        }
    }

    double** img_temp = create_2D_array_double(xSize, ySize);
    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            img_temp[i][j] = result[i*xSize+j];
        }
    }

    double** img_temp2 = double_pad(img_temp, xSize, ySize, 13);

    for (int i = 6; i < ySize+6; i++)
    {
        for (int j = 6; j < xSize+6; j++)
        {
            result[(i-6)*xSize+(j-6)] = local_max(img_temp2, i, j, 13);
        }
    }

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(result, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

	delete[] result;
	return 0;
}

unsigned char** create_2D_array(int xSize, int ySize)
{
    unsigned char** newArray = new unsigned char*[ySize];
    for (int i = 0; i < ySize; i++)
    {
        newArray[i] = new unsigned char[xSize];
    }
    return newArray;
}

double** create_2D_array_double(int xSize, int ySize)
{
    double** newArray = new double*[ySize];
    for (int i = 0; i < ySize; i++)
    {
        newArray[i] = new double[xSize];
    }
    return newArray;
}

int** create_filter(int f1[3], int f2[3])
{
    int** filter = new int*[3];
    for (int i = 0; i < 3; i++)
    {
        filter[i] = new int[3];
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            filter[i][j] = f1[i]*f2[j];
        }
    }
    return filter;
}

unsigned char** pad(unsigned char** rgb, int xSize, int ySize, int window_size)
{
    unsigned char** rgbnew = new unsigned char*[ySize+window_size-1];
    for (int i = 0; i < ySize+window_size-1; i++)
    {
        rgbnew[i] = new unsigned char[xSize+window_size-1];
    }

    if (window_size == 3)
    {
        for (int row = 0; row < ySize; row++)
        {
            for (int col = 0; col < xSize; col++)
            {
                rgbnew[0][col+1] = rgb[0][col];
                rgbnew[row+1][col+1] = rgb[row][col];
                rgbnew[ySize+1][col+1] = rgb[ySize-1][col];
            }
        }

        for (int row = 0; row < ySize+2; row++)
        {
            rgbnew[row][0] = rgbnew[row][1];
            rgbnew[row][xSize+1] = rgbnew[row][xSize];
        }
    }

    else if (window_size == 13)
    {
        for (int row = 0; row < ySize; row++)
        {
            for (int col = 0; col < xSize; col++)
            {
                rgbnew[5][col+6] = rgb[0][col];
                rgbnew[4][col+6] = rgb[1][col];
                rgbnew[3][col+6] = rgb[2][col];
                rgbnew[2][col+6] = rgb[3][col];
                rgbnew[1][col+6] = rgb[4][col];
                rgbnew[0][col+6] = rgb[5][col];
                rgbnew[row+6][col+6] = rgb[row][col];
                rgbnew[ySize+6][col+6] = rgb[ySize-1][col];
                rgbnew[ySize+7][col+6] = rgb[ySize-2][col];
                rgbnew[ySize+8][col+6] = rgb[ySize-3][col];
                rgbnew[ySize+9][col+6] = rgb[ySize-4][col];
                rgbnew[ySize+10][col+6] = rgb[ySize-5][col];
                rgbnew[ySize+11][col+6] = rgb[ySize-6][col];
            }
        }

        for (int row = 0; row < ySize+12; row++)
        {
            rgbnew[row][0] = rgbnew[row][11];
            rgbnew[row][1] = rgbnew[row][10];
            rgbnew[row][2] = rgbnew[row][9];
            rgbnew[row][3] = rgbnew[row][8];
            rgbnew[row][4] = rgbnew[row][7];
            rgbnew[row][5] = rgbnew[row][6];
            rgbnew[row][xSize+6] = rgbnew[row][xSize+5];
            rgbnew[row][xSize+7] = rgbnew[row][xSize+4];
            rgbnew[row][xSize+8] = rgbnew[row][xSize+3];
            rgbnew[row][xSize+9] = rgbnew[row][xSize+2];
            rgbnew[row][xSize+10] = rgbnew[row][xSize+1];
            rgbnew[row][xSize+11] = rgbnew[row][xSize];
        }
    }

    return rgbnew;
}

double** double_pad(double** rgb, int xSize, int ySize, int window_size)
{
    double** rgbnew = new double*[ySize+window_size-1];
    for (int i = 0; i < ySize+window_size-1; i++)
    {
        rgbnew[i] = new double[xSize+window_size-1];
    }

    if (window_size == 3)
    {
        for (int row = 0; row < ySize; row++)
        {
            for (int col = 0; col < xSize; col++)
            {
                rgbnew[0][col+1] = rgb[0][col];
                rgbnew[row+1][col+1] = rgb[row][col];
                rgbnew[ySize+1][col+1] = rgb[ySize-1][col];
            }
        }

        for (int row = 0; row < ySize+2; row++)
        {
            rgbnew[row][0] = rgbnew[row][1];
            rgbnew[row][xSize+1] = rgbnew[row][xSize];
        }
    }

    else if (window_size == 13)
    {
        for (int row = 0; row < ySize; row++)
        {
            for (int col = 0; col < xSize; col++)
            {
                rgbnew[5][col+6] = rgb[0][col];
                rgbnew[4][col+6] = rgb[1][col];
                rgbnew[3][col+6] = rgb[2][col];
                rgbnew[2][col+6] = rgb[3][col];
                rgbnew[1][col+6] = rgb[4][col];
                rgbnew[0][col+6] = rgb[5][col];
                rgbnew[row+6][col+6] = rgb[row][col];
                rgbnew[ySize+6][col+6] = rgb[ySize-1][col];
                rgbnew[ySize+7][col+6] = rgb[ySize-2][col];
                rgbnew[ySize+8][col+6] = rgb[ySize-3][col];
                rgbnew[ySize+9][col+6] = rgb[ySize-4][col];
                rgbnew[ySize+10][col+6] = rgb[ySize-5][col];
                rgbnew[ySize+11][col+6] = rgb[ySize-6][col];
            }
        }

        for (int row = 0; row < ySize+12; row++)
        {
            rgbnew[row][0] = rgbnew[row][11];
            rgbnew[row][1] = rgbnew[row][10];
            rgbnew[row][2] = rgbnew[row][9];
            rgbnew[row][3] = rgbnew[row][8];
            rgbnew[row][4] = rgbnew[row][7];
            rgbnew[row][5] = rgbnew[row][6];
            rgbnew[row][xSize+6] = rgbnew[row][xSize+5];
            rgbnew[row][xSize+7] = rgbnew[row][xSize+4];
            rgbnew[row][xSize+8] = rgbnew[row][xSize+3];
            rgbnew[row][xSize+9] = rgbnew[row][xSize+2];
            rgbnew[row][xSize+10] = rgbnew[row][xSize+1];
            rgbnew[row][xSize+11] = rgbnew[row][xSize];
        }
    }

    return rgbnew;
}

double feature(unsigned char** image, int** filter, int x, int y, int window)
{
    double sum = 0;
    for (int i = 0; i < window; i++)
    {
        for (int j = 0; j < window; j++)
        {
            sum = sum + (image[x + i - (window/2)][y + j - (window/2)])*(filter[i][j]);
        }
    }
    return (sum*sum);
}

double avg_filter(double** rgb, int x, int y, int window)
{
    int w2 = window*window;
    double sum = 0;

    for (int i = 0; i < window; i++)
    {
        for (int j = 0; j < window; j++)
        {
            sum += (rgb[x + i - (window/2)][y + j - (window/2)]);
        }
    }

    double avg = sum /double(w2);
    //int avg = 9/sum;
    //cout << "sum is " << sum << endl;
    //cout << "avg is " << avg << endl;
    return avg;
}

Mat compressPCA_vector(Mat& pcaset, int maxComponents)
{
    PCA pca;
    pca.operator()(pcaset, // pass the data
            Mat(), // we do not have a pre-computed mean vector,
                   // so let the PCA engine to compute it
            PCA::DATA_AS_COL, // indicate that the vectors
                                // are stored as matrix rows
                                // (use PCA::DATA_AS_COL if the vectors are
                                // the matrix columns)
            maxComponents // specify, how many principal components to retain
            );
    cout << "pca done " << endl;

    return pca.eigenvectors;
}

Mat compressPCA_value(Mat& pcaset, int maxComponents)
{
    PCA pca;
    pca.operator()(pcaset, // pass the data
            Mat(), // we do not have a pre-computed mean vector,
                   // so let the PCA engine to compute it
            PCA::DATA_AS_COL, // indicate that the vectors
                                // are stored as matrix rows
                                // (use PCA::DATA_AS_COL if the vectors are
                                // the matrix columns)
            maxComponents // specify, how many principal components to retain
            );
    cout << "pca done " << endl;

    return pca.eigenvalues;
}

unsigned char local_max(double** rgb, int x, int y, int window)
{
    int counter0 = 0; int counter51 = 0; int counter102 = 0; int counter153 = 0; int counter204 = 0; int counter255 = 0;
    for (int i = 0; i < window; i++)
    {
        for (int j = 0; j < window; j++)
        {
            if (rgb[x + i - (window/2)][y + j - (window/2)] == 0)
                counter0++;
            else if (rgb[x + i - (window/2)][y + j - (window/2)] == 51)
                counter51++;
            else if (rgb[x + i - (window/2)][y + j - (window/2)] == 102)
                counter102++;
            else if (rgb[x + i - (window/2)][y + j - (window/2)] == 153)
                counter153++;
            else if (rgb[x + i - (window/2)][y + j - (window/2)] == 204)
                counter204++;
            else
                counter255++;
        }
    }

    int max_val = max(counter0, max(counter51, max(counter102, max(counter153, max(counter204, counter255)))));
    unsigned char lc_max = 0;
    if (max_val == counter0)
        lc_max = 0;
    else if (max_val == counter51)
        lc_max = 51;
    else if (max_val == counter102)
        lc_max = 102;
    else if (max_val == counter153)
        lc_max = 153;
    else if (max_val == counter204)
        lc_max = 204;
    else
        lc_max = 255;

    return lc_max;
}
