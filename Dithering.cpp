/* Dithering
   Output_Image(1) Dithering using fixed thresholding
   Output_Image(2) Dithering using random thresholding
   Output_Image(3) Dithering using Bayer index matrix of size 2
   Output_Image(4) Dithering using Bayer index matrix of size 4
   Output_Image(5) Dithering using Bayer index matrix of size 8
   Output_Image(6) Dithering using Bayer index matrix of size 2 but 4 intensity values in output instead of 2
   Pass arguments in following order- Input_Image.raw Output_Image(1).raw BytesPerPixel Width Height Output_Image(2).raw Output_Image(3).raw Output_Image(4).raw Output_Image(5).raw Output_Image(6).raw
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

int** dither(int order);

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
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
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
	unsigned char* Imagedata = new unsigned char [xSize*ySize];
	unsigned char* result_fixed = new unsigned char [xSize*ySize];
	unsigned char* result_random = new unsigned char [xSize*ySize];
	unsigned char* result_w2 = new unsigned char [xSize*ySize];
	unsigned char* result_w4 = new unsigned char [xSize*ySize];
	unsigned char* result_w8 = new unsigned char [xSize*ySize];
	unsigned char* result_4val = new unsigned char [xSize*ySize];


	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	//Thresholding

	for (int i = 0; i < xSize*ySize; i++)
    {
        //Fixed
        if (Imagedata[i] <= 127) //127 intuitive, 81 for histogram method
            result_fixed[i] = 0;
        else
            result_fixed[i] = 255;

        //Random
        int r = rand() % 256;
        if (Imagedata[i] <= r)
            result_random[i] = 0;
        else
            result_random[i] = 255;
    }
    cout << "fixed and random result success " << endl;

    int** w2 = dither(2);
    int** w4 = dither(4);
    int** w8 = dither(8);

    unsigned char img[xSize][ySize];

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            img[i][j] = Imagedata[i*xSize + j];
        }
    }

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            if (img[i][j] > ((w2[i%2][j%2] + 0.5)/4)*255)
                result_w2[i*xSize + j] = 255;
            else
                result_w2[i*xSize + j] = 0;

            if (img[i][j] > ((w4[i%4][j%4] + 0.5)/16)*255)
                result_w4[i*xSize + j] = 255;
            else
                result_w4[i*xSize + j] = 0;

            if (img[i][j] > ((w8[i%8][j%8] + 0.5)/64)*255)
                result_w8[i*xSize + j] = 255;
            else
                result_w8[i*xSize + j] = 0;

            if (img[i][j] > ((w2[i%2][j%2] + 0.5)/4)*255)
                result_4val[i*xSize + j] = 255;
            else if (img[i][j] > (((w2[i%2][j%2] + 0.5)/4)*255*2)/3)
                result_4val[i*xSize + j] = 170;
            else if (img[i][j] > (((w2[i%2][j%2] + 0.5)/4)*255)/3)
                result_4val[i*xSize + j] = 85;
            else
                result_4val[i*xSize + j] = 0;
        }
    }
    cout << "matrix result success " << endl;

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(result_fixed, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	if (!(file=fopen(argv[6],"wb"))) {
		cout << "Cannot open file: " << argv[6] << endl;
		exit(1);
	}
	fwrite(result_random, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	if (!(file=fopen(argv[7],"wb"))) {
		cout << "Cannot open file: " << argv[7] << endl;
		exit(1);
	}
	fwrite(result_w2, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	if (!(file=fopen(argv[8],"wb"))) {
		cout << "Cannot open file: " << argv[8] << endl;
		exit(1);
	}
	fwrite(result_w4, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	if (!(file=fopen(argv[9],"wb"))) {
		cout << "Cannot open file: " << argv[9] << endl;
		exit(1);
	}
	fwrite(result_w8, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	if (!(file=fopen(argv[10],"wb"))) {
		cout << "Cannot open file: " << argv[10] << endl;
		exit(1);
	}
	fwrite(result_4val, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	delete[] Imagedata;
	delete[] result_fixed;
	delete[] result_random;
	delete[] result_w2;
	delete[] result_w4;
	delete[] result_w8;
	delete[] result_4val;
	for (int i = 0; i < 2; i++)
        delete[] w2[i];
    delete[] w2;

    for (int i = 0; i < 4; i++)
        delete[] w4[i];
    delete[] w4;

    for (int i = 0; i < 8; i++)
        delete[] w8[i];
    delete[] w8;

	return 0;
}


int** dither(int order)
{
    int n = order/2;
    int temp;

    int** w = new int*[order];
        for (int i = 0; i < order; i++)
        {
            w[i] = new int[order];
        }

    if (order == 2)
    {
        for (int i = 0; i < order; i ++)
        {
            for (int j = 0; j < order; j++)
            {
                 if (i < n && j < n)
                    temp = 1;
                else if (i < n && j >= n)
                    temp = 2;
                else if (i >= n && j < n)
                    temp = 3;
                else
                    temp = 0;

                w[i][j] = temp;
            }
        }
    }
    if (order == 4)
    {
        for (int i = 0; i < order; i++)
        {
            for (int j = 0; j < order; j++)
            {
                 if (i < n && j < n)
                    temp = 1;
                else if (i < n && j >= n)
                    temp = 2;
                else if (i >= n && j < n)
                    temp = 3;
                else
                    temp = 0;

                int** w1 = dither(2);
                w[i][j] = 4*w1[i%2][j%2] + temp;
            }
        }
    }


    if (order == 8)
    {
        for (int i = 0; i < order; i++)
        {
            for (int j = 0; j < order; j++)
            {
                 if (i < n && j < n)
                    temp = 1;
                else if (i < n && j >= n)
                    temp = 2;
                else if (i >= n && j < n)
                    temp = 3;
                else
                    temp = 0;

                int** w1 = dither(4);
                w[i][j] = 4*w1[i%4][j%4] + temp;
                //w[i][j] = ((w[i][j] + 0.5)/64);
            }
        }
    }
    return w;
}
