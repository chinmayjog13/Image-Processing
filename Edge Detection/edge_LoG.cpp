/* Edge Detection using LoG filter
   Input is colour
   Output_Image(1) is ternary edge map
   Output_Image(2) binary edge map
   Arguments must be passed as follows- Input_Image.raw Output_Image(1).raw BytesPerPixel Width Height Output_Image(2).raw
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include<fstream>

using namespace std;

unsigned char** create_2D_array(int xSize, int ySize);
int** create_2D_array_int(int xSize, int ySize);
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
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			xSize = atoi(argv[4]);
			ySize = atoi(argv[5]);
		}
	}

	// Allocate image data array
	unsigned char* Imagedata = new unsigned char [xSize*ySize*BytesPerPixel];
	double* grey_image = new double [xSize*ySize];
	int* result_int = new int [xSize*ySize];
	int** result_bin = new int* [ySize];
	for (int i = 0; i < ySize; i++)
    {
        result_bin[i] = new int [xSize];
    }
	unsigned char* result = new unsigned char [xSize*ySize];
	unsigned char* result2 = new unsigned char [xSize*ySize];
	unsigned char** image = create_2D_array(xSize, ySize);
	unsigned char** image_int = create_2D_array(xSize+2, ySize+2);

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

    //Sobel filter

    int count1 = 0;
    for (int i = 0; i < xSize*ySize*BytesPerPixel; i+=3)
    {
        grey_image[count1] = Imagedata[i]*0.21 + Imagedata[i+1]*0.72 + Imagedata[i+2]*0.07;
        count1++;
    }
    cout << "Converted to gray " << endl;
    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            image[i][j] = grey_image[i*xSize + j];
            result[i*xSize+j] = grey_image[i*xSize + j];
        }
    }

    image_int = pad(image, xSize, ySize, 3);

    int** filter = create_2D_array_int(3, 3);

    // LoG filter
    filter[0][0] = 0; filter[0][1] = -1; filter[0][2] = 0;
    filter[1][0] = -1; filter[1][1] = 4; filter[1][2] = -1;
    filter[2][0] = 0; filter[2][1] = -1; filter[2][2] = 0;

    for (int i = 1; i < ySize+1; i++)
    {
        for (int j = 1; j < xSize+1; j++)
        {
            result_int[(i-1)*xSize+(j-1)] = feature(image_int, filter, i, j, 3);
        }
    }
    cout << "Filters applied " << endl;

    double counter[512] = {0};
    int countval;

    for (int j = 0; j < xSize*ySize; j++)
    {
        countval = result_int[j]%255;
        int index = countval+255;
        counter[index]++;
    }
    cout << "histogram done " << endl;

    ofstream histr_original("hist_log.csv");
    for (int i =0; i < 512; i++)
    {
        histr_original << i << "," << counter[i] << endl;
    }
    histr_original.close();

    int kneePoint1;
    int kneePoint2;
    for (int i = 1; i < 512; i++)
    {
        if (counter[i-1] + 40 < counter[i])
        {
            kneePoint1 = i-255;
            i = 512;
        }
    }
    for (int i = 512; i > 1; i--)
    {
        if (counter[i] + 40 < counter[i-1])
        {
            kneePoint2 = i-255;
            i = 0;
        }
    }

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            if (result_int[i*xSize+j] <= kneePoint1)
            {
                result[i*xSize+j] = 64;
                result2[i*xSize+j] = 0;
                result_bin[i][j] = -1;
            }
            else if (result_int[i*xSize+j] > kneePoint1 && result_int[i*xSize+j] <= kneePoint2)
            {
                result[i*xSize+j] = 128;
                result2[i*xSize+j] = 0;
                result_bin[i][j] = 0;
            }
            else
            {
                result[i*xSize+j] = 192;
                result2[i*xSize+j] = 255;
                result_bin[i][j] = 1;
            }
        }
    }
    cout << "pt1 = " << kneePoint1 << " pt2 = " << kneePoint2 << endl;

//    for (int i = 1; i < ySize-1; i++)
//    {
//        for (int j = 1; j < xSize-1; j++)
//        {
//            if (result_bin[i][j] == 1)
//            {
//                if (result_bin[i-1][j] == 0 || result_bin[i+1][j] == 0 || result_bin[i][j-1] == 0 || result_bin[i][j+1] == 0 ||
//                    result_bin[i-1][j-1] == 0 || result_bin[i-1][j+1] == 0 || result_bin[i+1][j-1] == 0 || result_bin[i+1][j+1] == 0)
//                {
//                    result2[i*xSize+j] = 255;
//                }
//                else
//                {
//                    result2[i*xSize+j] = 0;
//                }
//            }
//            else
//            {
//                result2[i*xSize+j] = 0;
//            }
//        }
//    }



	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(result, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	if (!(file=fopen(argv[6],"wb"))) {
		cout << "Cannot open file: " << argv[6] << endl;
		exit(1);
	}
	fwrite(result2, sizeof(unsigned char), xSize*ySize, file);
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

int** create_2D_array_int(int xSize, int ySize)
{
    int** newArray = new int*[ySize];
    for (int i = 0; i < ySize; i++)
    {
        newArray[i] = new int[xSize];
    }
    return newArray;
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
    return sum;
}
