/*
// Author Chinmay Jog
// Id: 6171795819
// Email: cjog@usc.edu
// Date: Feb 25, 2018
// Assignment 2
// Question 2- Colour Error Diffusion
*/
#include <cstdlib>
#include <cmath>
#include <math.h>

double* diffusion_fs(double* resultint_fs, int xSize, int ySize);
double* diffusion_jjn(double* resultint_jjn, int xSize, int ySize);
double* diffusion_st(double* resultint_st, int xSize, int ySize);

	int ySize = 512;
			ySize = atoi(argv[5]);
	unsigned char* result = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* result_c = new unsigned char [xSize*ySize];
	unsigned char* result_m = new unsigned char [xSize*ySize];
	unsigned char* result_y = new unsigned char [xSize*ySize];
	double* resultint_c = new double [xSize*ySize];
	double* resultint_m = new double [xSize*ySize];
	double* resultint_y = new double [xSize*ySize];

	int count1 = 0;
	for (int i = 0; i < xSize*ySize*BytesPerPixel; i+=3)
    {
        resultint_c[count1] = 255 - Imagedata[i];
        resultint_m[count1] = 255 - Imagedata[i+1];
        resultint_y[count1] = 255 - Imagedata[i+2];
        count1++;
    }

    double* temp_c = diffusion_fs(resultint_c, xSize, ySize);
    double* temp_m = diffusion_fs(resultint_m, xSize, ySize);
    double* temp_y = diffusion_fs(resultint_y, xSize, ySize);
    for (int i = 0; i < xSize*ySize; i++)
    {
        result_c[i] = 255 - temp_c[i];
        result_m[i] = 255 - temp_m[i];
        result_y[i] = 255 - temp_y[i];
    }
    count1 = 0;
    for (int i = 0; i < xSize*ySize*BytesPerPixel; i+=3)
    {
        result[i] = result_c[count1];
        result[i+1] = result_m[count1];
        result[i+2] = result_y[count1];
        count1++;
    }
    cout << "Diffusion done " << endl;


	delete[] Imagedata;
	delete[] result;
	delete[] result_c;
	delete[] result_m;
	delete[] result_y;
	delete[] resultint_c;
	delete[] resultint_m;
	delete[] resultint_y;



double* diffusion_fs(double* resultint, int xSize, int ySize)
{
    int count1 = 0;
    double oldval = 0;
    double newval = 0;
    double err = 0;
    for (int i = 0; i < ySize-1; i++)
    {
        if (count1%2 == 0)
        {
            for (int j = 0; j < xSize; j++)
            {

                oldval = resultint[(i*xSize)+j];
                if (resultint[(i*xSize+j)] > 127)
                    newval = 255;
                else
                    newval = 0;
                resultint[(i*xSize+j)] = newval;

                err = (oldval - newval);
                resultint[(i*xSize)+j+1] = resultint[(i*xSize)+j+1] + (err*0.4375);
                resultint[((i+1)*xSize)+j + 1] = resultint[((i+1)*xSize)+j +1] + (err*0.0625);
                resultint[((i+1)*xSize)+j] = resultint[((i+1)*xSize)+j] + (err*0.3125);
                resultint[((i+1)*xSize)+j-1] = resultint[((i+1)*xSize)+j-1] + (err*0.1875);
                count1++;
                //cout << "error is " << err << endl;
            }
        }

        if (count1%2 == 1)
        {
            for(int j = xSize; j > 0; j--)
            {
                oldval = resultint[i*xSize+j];
                if (resultint[(i*xSize)+j] > 127)
                    newval = 255;
                else
                    newval = 0;
                resultint[(i*xSize)+j] = newval;

                err = (oldval - newval);
                resultint[(i*xSize)+j-1] = resultint[(i*xSize)+j-1] + (err*0.4375);
                resultint[((i+1)*xSize)+j-1] = resultint[((i+1)*xSize)+j-1] + (err*0.0625);
                resultint[((i+1)*xSize)+j] = resultint[((i+1)*xSize)+j] + (err*0.3125);
                resultint[((i+1)*xSize)+j+1] = resultint[((i+1)*xSize)+j+1] + (err*0.1875);
                count1++;
            }
        }
    }
    int i = ySize - 1;
    for (int j = 0; j < xSize; j++)
    {
        if (resultint[i*xSize + j] > 127)
            resultint[i*xSize + j] = 255;
        else
            resultint[i*xSize + j] = 0;
    }
    return resultint;
}