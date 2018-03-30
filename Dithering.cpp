/*
// Author Chinmay Jog
// Id: 6171795819
// Email: cjog@usc.edu
// Date: Feb 25, 2018
// Assignment 2
// Question 2- Dithering
*/
#include <cstdlib>

int** dither(int order);

	int ySize = 512;
			ySize = atoi(argv[5]);
	unsigned char* result_fixed = new unsigned char [xSize*ySize];
	unsigned char* result_random = new unsigned char [xSize*ySize];
	unsigned char* result_w2 = new unsigned char [xSize*ySize];
	unsigned char* result_w4 = new unsigned char [xSize*ySize];
	unsigned char* result_w8 = new unsigned char [xSize*ySize];
	unsigned char* result_4val = new unsigned char [xSize*ySize];


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


	if (!(file=fopen(argv[6],"wb"))) {

	if (!(file=fopen(argv[7],"wb"))) {

	if (!(file=fopen(argv[8],"wb"))) {

	if (!(file=fopen(argv[9],"wb"))) {

	if (!(file=fopen(argv[10],"wb"))) {

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