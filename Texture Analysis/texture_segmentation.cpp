/* Texture Segmentation using Laws filters and k means clustering
   Input image is grayscale
   Arguments must be passed as follows- Input_Image.raw Output.raw BytesPerPixel Width Height
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

unsigned char** create_2D_array(int xSize, int ySize);
int** create_filter(int f1[3], int f2[3]);
unsigned char** pad(unsigned char** rgb, int xSize, int ySize, int window_size);
double feature(double** image, int** filter, int x, int y, int window);
double avg_filter(double** rgb, int x, int y, int window);
double** double_pad(double** rgb, int xSize, int ySize, int window_size);
double** create_2D_array_double(int xSize, int ySize);
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
	double** image = create_2D_array_double(xSize, ySize);
	double** image2 = create_2D_array_double(xSize, ySize);
	double** image_padded = create_2D_array_double(xSize+14, ySize+14);
	double** image_int1 = create_2D_array_double(xSize+2, ySize+2);
	double** image_ee = create_2D_array_double(xSize+12, ySize+12);
	double** image_es = create_2D_array_double(xSize+12, ySize+12);
	double** image_el = create_2D_array_double(xSize+12, ySize+12);
	double** image_se = create_2D_array_double(xSize+12, ySize+12);
	double** image_ss = create_2D_array_double(xSize+12, ySize+12);
	double** image_sl = create_2D_array_double(xSize+12, ySize+12);
	double** image_le = create_2D_array_double(xSize+12, ySize+12);
	double** image_ls = create_2D_array_double(xSize+12, ySize+12);
	double** image_ll = create_2D_array_double(xSize+12, ySize+12);
	double** feature_ee = new double* [450];
	double** feature_es = new double* [450];
	double** feature_el = new double* [450];
	double** feature_se = new double* [450];
	double** feature_ss = new double* [450];
	double** feature_sl = new double* [450];
	double** feature_le = new double* [450];
	double** feature_ls = new double* [450];
	double** feature_ll = new double* [450];
    for (int i = 0; i < 450; i++)
    {
        feature_ee[i] = new double[600];
        feature_es[i] = new double[600];
        feature_el[i] = new double[600];
        feature_se[i] = new double[600];
        feature_ss[i] = new double[600];
        feature_sl[i] = new double[600];
        feature_le[i] = new double[600];
        feature_ls[i] = new double[600];
        feature_ll[i] = new double[600];
    }
    double** feature_vector = new double* [450*600];
    for (int i = 0; i < 450*600; i++)
    {
        feature_vector[i] = new double[9];
    }
    double** centroid_vector = new double* [6];
    double** old_vector = new double* [6];
    for (int i = 0; i < 6; i++)
    {
        centroid_vector[i] = new double[9];
        old_vector[i] = new double[9];
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
    image_padded = double_pad(image, xSize, ySize, 15);

    for (int i = 7; i < ySize+7; i++)
    {
        for (int j = 7; j < xSize+7; j++)
        {
            image2[i-7][j-7] = avg_filter(image_padded, i, j, 15);
        }
    }
    image_int1 = double_pad(image2, xSize, ySize, 3);

    int L[3] = {1, 2, 1};
    int E[3] = {-1, 0, 1};
    int S[3] = {-1, 2, -1};

    int** EE = create_filter(E, E);
    int** SS = create_filter(S, S);
    int** LL = create_filter(L, L);
    int** ES = create_filter(E, S);
    int** EL = create_filter(E, L);
    int** SE = create_filter(S, E);
    int** SL = create_filter(S, L);
    int** LE = create_filter(L, E);
    int** LS = create_filter(L, S);

    for (int i = 1; i < ySize+1; i++)
    {
        for (int j = 1; j < xSize+1; j++)
        {
            feature_ee[i-1][j-1] = feature(image_int1, EE, i, j, 3)/double(16);
            feature_es[i-1][j-1] = feature(image_int1, ES, i, j, 3)/double(16);
            feature_el[i-1][j-1] = feature(image_int1, EL, i, j, 3)/double(144);
            feature_se[i-1][j-1] = feature(image_int1, SE, i, j, 3)/double(16);
            feature_ss[i-1][j-1] = feature(image_int1, SS, i, j, 3)/double(16);
            feature_sl[i-1][j-1] = feature(image_int1, SL, i, j, 3)/double(144);
            feature_le[i-1][j-1] = feature(image_int1, LE, i, j, 3)/double(144);
            feature_ls[i-1][j-1] = feature(image_int1, LS, i, j, 3)/double(144);
            feature_ll[i-1][j-1] = feature(image_int1, LL, i, j, 3)/double(36*36);
        }
    }
    cout << feature_ll[0][0]*36*36 << " " << feature_ll[0][1]*36*36 << " " << feature_ll[0][2]*36*36 << endl;

    cout << "filters done " << endl;
    image_ee = double_pad(feature_ee, xSize, ySize, 13);
    image_es = double_pad(feature_es, xSize, ySize, 13);
    image_el = double_pad(feature_el, xSize, ySize, 13);
    image_se = double_pad(feature_se, xSize, ySize, 13);
    image_ss = double_pad(feature_ss, xSize, ySize, 13);
    image_sl = double_pad(feature_sl, xSize, ySize, 13);
    image_le = double_pad(feature_le, xSize, ySize, 13);
    image_ls = double_pad(feature_ls, xSize, ySize, 13);
    image_ll = double_pad(feature_ll, xSize, ySize, 13);
    cout << "starting vectors " << endl;
    for (int i = 6; i < ySize+6; i++)
    {
        for (int j = 6; j < xSize+6; j++)
        {
            feature_vector[(i-6)*xSize+(j-6)][0] = avg_filter(image_ee, i, j, 13); // /avg_filter(image_ll, i, j, 13);
            feature_vector[(i-6)*xSize+(j-6)][1] = avg_filter(image_es, i, j, 13); // /avg_filter(image_ll, i, j, 13);
            feature_vector[(i-6)*xSize+(j-6)][2] = avg_filter(image_el, i, j, 13); // /avg_filter(image_ll, i, j, 13);
            feature_vector[(i-6)*xSize+(j-6)][3] = avg_filter(image_se, i, j, 13); // /avg_filter(image_ll, i, j, 13);
            feature_vector[(i-6)*xSize+(j-6)][4] = avg_filter(image_ss, i, j, 13); // /avg_filter(image_ll, i, j, 13);
            feature_vector[(i-6)*xSize+(j-6)][5] = avg_filter(image_sl, i, j, 13); // /avg_filter(image_ll, i, j, 13);
            feature_vector[(i-6)*xSize+(j-6)][6] = avg_filter(image_le, i, j, 13); // /avg_filter(image_ll, i, j, 13);
            feature_vector[(i-6)*xSize+(j-6)][7] = avg_filter(image_ls, i, j, 13); // /avg_filter(image_ll, i, j, 13);
            feature_vector[(i-6)*xSize+(j-6)][8] = avg_filter(image_ll, i, j, 13);
        }
    }

    for (int i = 0; i < 9; i++) //i<9 for unnormalized
    {
        centroid_vector[0][i] = feature_vector[0][i];//feat_max;
        centroid_vector[1][i] = feature_vector[75*xSize+300][i];//(feature_vector[2][i] + feature_vector[8][i] + feature_vector[10][i])/double(3);//double(2)/double(3)*(feat_min);
        centroid_vector[2][i] = feature_vector[200*xSize+300][i];//(feature_vector[1][i] + feature_vector[4][i] + feature_vector[11][i])/double(3);//double(1)/double(3)*(feat_max - feat_min);
        centroid_vector[3][i] = feature_vector[50*xSize+500][i];
        centroid_vector[4][i] = feature_vector[400*xSize+100][i];
        centroid_vector[5][i] = feature_vector[450*600-1][i];
    }

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 9; j++)
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
    double** class_1 = create_2D_array_double(9, 450*600);
    double** class_2 = create_2D_array_double(9, 450*600);
    double** class_3 = create_2D_array_double(9, 450*600);
    double** class_4 = create_2D_array_double(9, 450*600);
    double** class_5 = create_2D_array_double(9, 450*600);
    double** class_6 = create_2D_array_double(9, 450*600);
    cout << "starting k means " << endl;
    for (int iter = 0; iter < 100; iter++)
    {
        double mean1[9] = {0}, mean2[9] = {0}, mean3[9] = {0}, mean4[9] = {0}, mean5[9] = {0}, mean6[9] = {0};
        double dist1, dist2, dist3, dist4, dist5, dist6;

        t1 = 0; t2 = 0; t3 = 0; t4 = 0; t5 = 0; t6 = 0;
        cout << "iter is " << iter << endl;
        for (int i = 0; i < 600*450; i++)
        {
            //cout << i << endl;
            dist1 = 0; dist2 = 0; dist3 = 0; dist4 = 0; dist5 = 0; dist6 = 0;
            for (int j = 0; j < 9; j++)
            {
                dist1 = dist1 + pow((centroid_vector[0][j] - feature_vector[i][j]), 2);
                dist2 = dist2 + pow((centroid_vector[1][j] - feature_vector[i][j]), 2);
                dist3 = dist3 + pow((centroid_vector[2][j] - feature_vector[i][j]), 2);
                dist4 = dist4 + pow((centroid_vector[3][j] - feature_vector[i][j]), 2);
                dist5 = dist5 + pow((centroid_vector[4][j] - feature_vector[i][j]), 2);
                dist6 = dist6 + pow((centroid_vector[5][j] - feature_vector[i][j]), 2);

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
            else if (temp == dist4)
            {
                for (int j = 0; j < 9; j++)
                {
                    class_4[t4][j] = feature_vector[i][j];
                }
                t4++;
                //cout << "crashed in temp = dist3, t3 =  " << t3 << endl;
            }
            else if (temp == dist5)
            {
                for (int j = 0; j < 9; j++)
                {
                    class_5[t5][j] = feature_vector[i][j];
                }
                t5++;
                //cout << "crashed in temp = dist5, t5 =  " << t5 << endl;
            }
            else //if (temp == dist4)
            {
                for (int j = 0; j < 9; j++)
                {
                    class_6[t6][j] = feature_vector[i][j];
                }
                t6++;
                //cout << "crashed in temp = dist6, t6 =  " << t6 << endl;
            }
        }
        cout << "classes allocated " << endl;

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
        for (int i = 0; i < 9; i++)
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
            for (int i = 0; i < 450*600; i++)
                {
                    dist1 = 0; dist2 = 0; dist3 = 0; dist4 = 0; dist5 = 0; dist6 = 0;
                    for (int j = 0; j < 9; j++)
                    {
//                        cout << centroid_vector[0][j] << "\t";
                        dist1 = dist1 + pow((centroid_vector[0][j] - feature_vector[i][j]), 2);
                        dist2 = dist2 + pow((centroid_vector[1][j] - feature_vector[i][j]), 2);
                        dist3 = dist3 + pow((centroid_vector[2][j] - feature_vector[i][j]), 2);
                        dist4 = dist4 + pow((centroid_vector[3][j] - feature_vector[i][j]), 2);
                        dist5 = dist5 + pow((centroid_vector[4][j] - feature_vector[i][j]), 2);
                        dist6 = dist6 + pow((centroid_vector[5][j] - feature_vector[i][j]), 2);
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
//    double** img_temp = create_2D_array_double(xSize, ySize);
//    for (int i = 0; i < ySize; i++)
//    {
//        for (int j = 0; j < xSize; j++)
//        {
//            img_temp[i][j] = result[i*xSize+j];
//        }
//    }
//
//    double** img_temp2 = double_pad(img_temp, xSize, ySize, 15);
//
//    for (int i = 7; i < ySize+7; i++)
//    {
//        for (int j = 7; j < xSize+7; j++)
//        {
//            result[(i-7)*xSize+(j-7)] = local_max(img_temp2, i, j, 15);
//        }
//    }

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

    else if (window_size == 15)
    {
        for (int row = 0; row < ySize; row++)
        {
            for (int col = 0; col < xSize; col++)
            {
                rgbnew[6][col+7] = rgb[0][col];
                rgbnew[5][col+7] = rgb[1][col];
                rgbnew[4][col+7] = rgb[2][col];
                rgbnew[3][col+7] = rgb[3][col];
                rgbnew[2][col+7] = rgb[4][col];
                rgbnew[1][col+7] = rgb[5][col];
                rgbnew[0][col+7] = rgb[6][col];
                rgbnew[row+7][col+7] = rgb[row][col];
                rgbnew[ySize+7][col+7] = rgb[ySize-1][col];
                rgbnew[ySize+8][col+7] = rgb[ySize-2][col];
                rgbnew[ySize+9][col+7] = rgb[ySize-3][col];
                rgbnew[ySize+10][col+7] = rgb[ySize-4][col];
                rgbnew[ySize+11][col+7] = rgb[ySize-5][col];
                rgbnew[ySize+12][col+7] = rgb[ySize-6][col];
                rgbnew[ySize+13][col+7] = rgb[ySize-7][col];
            }
        }

        for (int row = 0; row < ySize+14; row++)
        {
            rgbnew[row][0] = rgbnew[row][13];
            rgbnew[row][1] = rgbnew[row][12];
            rgbnew[row][2] = rgbnew[row][11];
            rgbnew[row][3] = rgbnew[row][10];
            rgbnew[row][4] = rgbnew[row][9];
            rgbnew[row][5] = rgbnew[row][8];
            rgbnew[row][6] = rgbnew[row][7];
            rgbnew[row][xSize+7] = rgbnew[row][xSize+6];
            rgbnew[row][xSize+8] = rgbnew[row][xSize+5];
            rgbnew[row][xSize+9] = rgbnew[row][xSize+4];
            rgbnew[row][xSize+10] = rgbnew[row][xSize+3];
            rgbnew[row][xSize+11] = rgbnew[row][xSize+2];
            rgbnew[row][xSize+12] = rgbnew[row][xSize+1];
            rgbnew[row][xSize+13] = rgbnew[row][xSize];
        }
    }

    return rgbnew;
}

double feature(double** image, int** filter, int x, int y, int window)
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
    return avg;
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
