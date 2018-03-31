/* Texture Classification using Laws filters and k means clustering
   Input should be grayscale
   Code is developed for 12 input images, this can be changed by making few simple changes
   Pass arguments in following order- Input_Image(i).raw Output_Image.raw BytesPerPixel Width Height
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

unsigned char** create_2D_array(int xSize, int ySize);
int** create_filter(int f1[5], int f2[5]);
unsigned char** pad(unsigned char** rgb, int xSize, int ySize, int window_size);
double feature(unsigned char** image, int** filter, int x, int y, int window);

int main(int argc, char *argv[])

{
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
		BytesPerPixel = atoi(argv[13]);
		// Check if size is specified
		if (argc >= 5){
			xSize = atoi(argv[14]);
			ySize = atoi(argv[15]);
		}
	}

	// Allocate image data array
	unsigned char* Imagedata = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* result = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char** image = create_2D_array(xSize, ySize);
	unsigned char** image_int = create_2D_array(xSize+4, ySize+4);
	double** feature_vector = new double* [12];
    for (int i = 0; i < 12; i++)
    {
        feature_vector[i] = new double[9];
    }

    double** centroid_vector = new double* [4];
    double** old_vector = new double* [4];
    for (int i = 0; i < 4; i++)
    {
        centroid_vector[i] = new double[9];
        old_vector[i] = new double[9];
    }

	// Read image (filename specified by first argument) into image data matrix
	for (int k = 0; k < 12; k++)
    {
        if (!(file=fopen(argv[k+1],"rb"))) {
		cout << "Cannot open file: " << argv[k+1] <<endl;
		exit(1);
        }
        fread(Imagedata, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
        fclose(file);

//	if (!(file=fopen(argv[1],"rb"))) {
//		cout << "Cannot open file: " << argv[1] <<endl;
//		exit(1);
//	}
//	fread(Imagedata, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
//	fclose(file);

    //Law's filter
    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            image[i][j] = Imagedata[i*xSize + j];
            result[i*xSize + j] = Imagedata[i*xSize + j];
        }
    }

    image_int = pad(image, xSize, ySize, 5);

    int E[5] = {-1, -2, 0, 2, 1};
    int S[5] = {-1, 0, 2, 0, -1};
    int W[5] = {-1, 2, 0, -2, 1};

    int** EE = create_filter(E, E);
    int** SS = create_filter(S, S);
    int** WW = create_filter(W, W);
    int** ES = create_filter(E, S);
    int** EW = create_filter(E, W);
    int** SE = create_filter(S, E);
    int** SW = create_filter(S, W);
    int** WE = create_filter(W, E);
    int** WS = create_filter(W, S);

    double sum_ee = 0;
    double sum_es = 0;
    double sum_ew = 0;
    double sum_se = 0;
    double sum_ss = 0;
    double sum_sw = 0;
    double sum_we = 0;
    double sum_ws = 0;
    double sum_ww = 0;

    for (int i = 2; i < ySize+2; i++)
    {
        for (int j = 2; j < xSize+2; j++)
        {
            sum_ee = sum_ee + feature(image_int, EE, i, j, 5);
            sum_es = sum_es + feature(image_int, ES, i, j, 5);
            sum_ew = sum_ew + feature(image_int, EW, i, j, 5);
            sum_se = sum_se + feature(image_int, SE, i, j, 5);
            sum_ss = sum_ss + feature(image_int, SS, i, j, 5);
            sum_sw = sum_sw + feature(image_int, SW, i, j, 5);
            sum_we = sum_we + feature(image_int, WE, i, j, 5);
            sum_ws = sum_ws + feature(image_int, WS, i, j, 5);
            sum_ww = sum_ww + feature(image_int, WW, i, j, 5);
        }
    }

    feature_vector[k][0] = sum_ee/double(xSize*ySize*36*36);
    feature_vector[k][1] = sum_es/double(xSize*ySize*24*24);
    feature_vector[k][2] = sum_ew/double(xSize*ySize*36*36);
    feature_vector[k][3] = sum_se/double(xSize*ySize*24*24);
    feature_vector[k][4] = sum_ss/double(xSize*ySize*16*16);
    feature_vector[k][5] = sum_sw/double(xSize*ySize*24*24);
    feature_vector[k][6] = sum_we/double(xSize*ySize*36*36);
    feature_vector[k][7] = sum_ws/double(xSize*ySize*24*24);
    feature_vector[k][8] = sum_ww/double(xSize*ySize*36*36);

    for (int i = 0; i < 9; i++)
    {
        cout << feature_vector[k][i] << "\t";
    }
    cout << "\n";
    }

    double mean[9] = {0};
    for (int k = 0; k < 9; k++)
    {
        for (int j = 0; j < 12; j++)
        {
            mean[k] += feature_vector[j][k]/double(12);
        }
    }

    double variance[9] = {0};
    for (int k = 0; k < 9; k++)
    {
        for (int j = 0; j < 12; j++)
        {
            variance[k] += pow((feature_vector[j][k] - mean[k]),2)/double(12);
        }
        cout << "Variance of feature " << k+1 << " is " << variance[k] << endl;
    }

    for (int i = 0; i < 9; i++)
    {
        centroid_vector[0][i] = feature_vector[0][i];//feat_max;
        centroid_vector[1][i] = feature_vector[2][i];//(feature_vector[2][i] + feature_vector[8][i] + feature_vector[10][i])/double(3);//double(2)/double(3)*(feat_min);
        centroid_vector[2][i] = feature_vector[1][i];//(feature_vector[1][i] + feature_vector[4][i] + feature_vector[11][i])/double(3);//double(1)/double(3)*(feat_max - feat_min);
        centroid_vector[3][i] = feature_vector[7][i];
    }

//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 9; j++)
//        {
//            cout << centroid_vector[i][j] << "\t";
//        }
//        cout << "\n";
//    }

//    double class_1[12][9] = {0};
//    double class_2[12][9] = {0};
//    double class_3[12][9] = {0};
//    double class_4[12][9] = {0};
//    double dist1 = 0;
//    double dist2 = 0;
//    double dist3 = 0;
//    double dist4 = 0;
    int t1 = 0, t2 = 0, t3 = 0, t4 = 0;
//    double mean1[9] = {0}, mean2[9] = {0}, mean3[9] = {0}, mean4[9] = {0};
    cout << "starting k means " << endl;
    for (int iter = 0; iter < 100; iter++)
    {
        double mean1[9] = {0}, mean2[9] = {0}, mean3[9] = {0}, mean4[9] = {0};
        double dist1, dist2, dist3, dist4;
        double class_1[12][9] = {0};
        double class_2[12][9] = {0};
        double class_3[12][9] = {0};
        double class_4[12][9] = {0};
        t1 = 0; t2 = 0; t3 = 0; t4 = 0;
        //cout << "iter is " << iter << endl;
        for (int i = 0; i < 12; i++)
        {
            dist1 = 0; dist2 = 0; dist3 = 0; dist4 = 0;
            for (int j = 0; j < 9; j++)
            {
                dist1 = dist1 + pow((centroid_vector[0][j] - feature_vector[i][j]), 2);
                dist2 = dist2 + pow((centroid_vector[1][j] - feature_vector[i][j]), 2);
                dist3 = dist3 + pow((centroid_vector[2][j] - feature_vector[i][j]), 2);
                dist4 = dist4 + pow((centroid_vector[3][j] - feature_vector[i][j]), 2);

                old_vector[0][j] = centroid_vector[0][j];
                old_vector[1][j] = centroid_vector[1][j];
                old_vector[2][j] = centroid_vector[2][j];
                old_vector[3][j] = centroid_vector[3][j];

                mean1[j] = 0; mean2[j] = 0; mean3[j] = 0; mean4[j] = 0;
                //cout << "distances calculated for i = " << i << " and j = " << j << endl;
            }
            dist1 = sqrt(dist1);
            dist2 = sqrt(dist2);
            dist3 = sqrt(dist3);
            dist4 = sqrt(dist4);
            //cout << "Distances " << dist1 << " " << dist2 << " " << dist3 << " " << dist4 << endl;
//            cout << "distances calculated for i = " << i << " and j = " << j << endl;

            double temp;
            temp = min(min(min(dist1,dist2), dist3), dist4);
            if (temp == dist1)
            {
                for (int j = 0; j < 9; j++)
                {
                    class_1[t1][j] = feature_vector[i][j];
                }
                t1++;
                //cout << "crashed in temp = dist1, t1 =  " << t1 << endl;
            }
            else if (temp == dist2)
            {
                for (int j = 0; j < 9; j++)
                {
                    class_2[t2][j] = feature_vector[i][j];
                }
                t2++;
                //cout << "crashed in temp = dist2, t2 =  " << t2 << endl;
            }
            else if (temp == dist3)
            {
                for (int j = 0; j < 9; j++)
                {
                    class_3[t3][j] = feature_vector[i][j];
                }
                t3++;
                //cout << "crashed in temp = dist3, t3 =  " << t3 << endl;
            }
            else //if (temp == dist4)
            {
                for (int j = 0; j < 9; j++)
                {
                    class_4[t4][j] = feature_vector[i][j];
                }
                t4++;
                //cout << "crashed in temp = dist4, t4 =  " << t4 << endl;
            }
        }
        //cout << "classes allocated " << endl;

        for (int i = 0; i < 9; i++)
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
        }
        for (int i = 0; i < 9; i++)
        {
            centroid_vector[0][i] = mean1[i];
            centroid_vector[1][i] = mean2[i];
            centroid_vector[2][i] = mean3[i];
            centroid_vector[3][i] = mean4[i];
        }
        //cout << "means calculated " << endl;
        int counter = 0;

        for (int i = 0; i < 9; i ++)
        {
            //cout << mean1[i] << "\t";
            if ((centroid_vector[0][i] == old_vector[0][i] && centroid_vector[1][i] == old_vector[1][i] && centroid_vector[2][i] == old_vector[2][i] && centroid_vector[3][i] == old_vector[3][i]) || iter == 99)
            {
                counter++;
            }
        }
        if (counter == 9)
        {
            for (int i = 0; i < 12; i++)
                {
                    dist1 = 0; dist2 = 0; dist3 = 0; dist4 = 0;
                    for (int j = 0; j < 9; j++)
                    {
//                        cout << centroid_vector[0][j] << "\t";
                        dist1 = dist1 + pow((centroid_vector[0][j] - feature_vector[i][j]), 2);
                        dist2 = dist2 + pow((centroid_vector[1][j] - feature_vector[i][j]), 2);
                        dist3 = dist3 + pow((centroid_vector[2][j] - feature_vector[i][j]), 2);
                        dist4 = dist4 + pow((centroid_vector[3][j] - feature_vector[i][j]), 2);
                    }
                    dist1 = sqrt(dist1);
                    dist2 = sqrt(dist2);
                    dist3 = sqrt(dist3);
                    dist4 = sqrt(dist4);

                    double temp;
                    temp = min(min(min(dist1,dist2), dist3), dist4);
                    if (temp == dist1)
                    {
                        cout << " Image " << i+1 << " has texture " << 1 << endl;
                        iter = 100;
                    }
                    else if (temp == dist2)
                    {
                        cout << " Image " << i+1 << " has texture " << 2 << endl;
                        iter = 100;
                    }
                    else if (temp == dist3)
                    {
                        cout << " Image " << i+1 << " has texture " << 3 << endl;
                        iter = 100;
                    }
                    else if (temp == dist4)
                    {
                        cout << " Image " << i+1 << " has texture " << 4 << endl;
                        iter = 100;
                    }
                }
        }
    }

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[16],"wb"))) {
		cout << "Cannot open file: " << argv[16] << endl;
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

int** create_filter(int f1[5], int f2[5])
{
    int** filter = new int*[5];
    for (int i = 0; i < 5; i++)
    {
        filter[i] = new int[5];
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
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

    for (int row = 0; row < ySize; row++)
    {
        for (int col = 0; col < xSize; col++)
        {
            rgbnew[1][col+2] = rgb[0][col];
            rgbnew[0][col+2] = rgb[1][col];
            rgbnew[row+2][col+2] = rgb[row][col];
            rgbnew[ySize+2][col+2] = rgb[ySize-1][col];
            rgbnew[ySize+3][col+2] = rgb[ySize-2][col];
        }
    }

    for (int row = 0; row < ySize+4; row++)
    {
        rgbnew[row][0] = rgbnew[row][3];
        rgbnew[row][1] = rgbnew[row][2];
        rgbnew[row][xSize+2] = rgbnew[row][xSize];
        rgbnew[row][xSize+3] = rgbnew[row][xSize+1];
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
