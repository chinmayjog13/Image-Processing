/* Dithering by Error diffusion techniques
   Output_Image(1) is output by Floyd Steinberg diffusion
   Output_Image(2) is output by JJN diffusion
   Output_Image(3) is output by Stucki diffusion
   Pass arguments in following order- Input_Image.raw Output_Image(1).raw BytesPerPixel Width Height Output_Image(2).raw Output_Image(3).raw
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <math.h>

using namespace std;

double* diffusion_fs(double* resultint_fs, int xSize, int ySize);
double* diffusion_jjn(double* resultint_jjn, int xSize, int ySize);
double* diffusion_st(double* resultint_st, int xSize, int ySize);

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
	unsigned char* result_fs = new unsigned char [xSize*ySize];
	unsigned char* result_jjn = new unsigned char [xSize*ySize];
	unsigned char* result_st = new unsigned char [xSize*ySize];
	double* resultint_fs = new double [xSize*ySize];
	double* resultint_jjn = new double [xSize*ySize];
	double* resultint_st = new double [xSize*ySize];


	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	//Error Diffusion
	for (int i = 0; i < xSize*ySize; i++)
    {
        resultint_fs[i] = Imagedata[i];
        resultint_jjn[i] = Imagedata[i];
        resultint_st[i] = Imagedata[i];
    }

    double* temp_fs = diffusion_fs(resultint_fs, xSize, ySize);
    double* temp_jjn = diffusion_jjn(resultint_jjn, xSize, ySize);
    double* temp_st = diffusion_st(resultint_st, xSize, ySize);
    for (int i = 0; i < xSize*ySize; i++)
    {
        result_fs[i] = temp_fs[i];
        result_jjn[i] = temp_jjn[i];
        result_st[i] = temp_st[i];
    }
    cout << "Diffusion done " << endl;

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(result_fs, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	if (!(file=fopen(argv[6],"wb"))) {
		cout << "Cannot open file: " << argv[6] << endl;
		exit(1);
	}
	fwrite(result_jjn, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	if (!(file=fopen(argv[7],"wb"))) {
		cout << "Cannot open file: " << argv[7] << endl;
		exit(1);
	}
	fwrite(result_st, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	delete[] Imagedata;
	delete[] result_fs;
	delete[] result_jjn;
	delete[] result_st;
	delete[] resultint_fs;
	delete[] resultint_jjn;
	delete[] resultint_st;

	return 0;
}


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
                cout << "error is " << err << endl;
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

double* diffusion_jjn(double* resultint, int xSize, int ySize)
{
    double oldval = 0;
    double newval = 0;
    double err = 0;
    for (int i = 0; i < ySize-2; i++)
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
            resultint[(i*xSize)+j+1] = resultint[(i*xSize)+j+1] + (err*0.1458);
            resultint[(i*xSize)+j+2] = resultint[(i*xSize)+j+2] + (err*0.1041);

            resultint[((i+1)*xSize)+j + 1] = resultint[((i+1)*xSize)+j +1] + (err*0.1041);
            resultint[((i+1)*xSize)+j + 2] = resultint[((i+1)*xSize)+j +2] + (err*0.0625);
            resultint[((i+1)*xSize)+j]     = resultint[((i+1)*xSize)+j]    + (err*0.1458);
            resultint[((i+1)*xSize)+j - 1] = resultint[((i+1)*xSize)+j -1] + (err*0.1041);
            resultint[((i+1)*xSize)+j - 2] = resultint[((i+1)*xSize)+j -2] + (err*0.0625);

            resultint[((i+2)*xSize)+j + 1] = resultint[((i+2)*xSize)+j +1] + (err*0.0625);
            resultint[((i+2)*xSize)+j + 2] = resultint[((i+2)*xSize)+j +2] + (err*0.0208);
            resultint[((i+2)*xSize)+j]     = resultint[((i+2)*xSize)+j]    + (err*0.1041);
            resultint[((i+2)*xSize)+j - 1] = resultint[((i+2)*xSize)+j -1] + (err*0.0625);
            resultint[((i+2)*xSize)+j - 2] = resultint[((i+2)*xSize)+j -2] + (err*0.0208);

            //cout << "error is " << err << endl;
            }
    }
    for (int i = ySize-2; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            if (resultint[i*xSize + j] > 127)
                resultint[i*xSize + j] = 255;
            else
                resultint[i*xSize + j] = 0;
        }
    }

    return resultint;
}

double* diffusion_st(double* resultint, int xSize, int ySize)
{
    double oldval = 0;
    double newval = 0;
    double err = 0;
    for (int i = 0; i < ySize-2; i++)
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
            resultint[(i*xSize)+j+1] = resultint[(i*xSize)+j+1] + (err*0.1904);
            resultint[(i*xSize)+j+2] = resultint[(i*xSize)+j+2] + (err*0.0952);

            resultint[((i+1)*xSize)+j + 1] = resultint[((i+1)*xSize)+j +1] + (err*0.0952);
            resultint[((i+1)*xSize)+j + 2] = resultint[((i+1)*xSize)+j +2] + (err*0.0476);
            resultint[((i+1)*xSize)+j]     = resultint[((i+1)*xSize)+j]    + (err*0.1904);
            resultint[((i+1)*xSize)+j - 1] = resultint[((i+1)*xSize)+j -1] + (err*0.0952);
            resultint[((i+1)*xSize)+j - 2] = resultint[((i+1)*xSize)+j -2] + (err*0.0476);

            resultint[((i+2)*xSize)+j + 1] = resultint[((i+2)*xSize)+j +1] + (err*0.0476);
            resultint[((i+2)*xSize)+j + 2] = resultint[((i+2)*xSize)+j +2] + (err*0.0238);
            resultint[((i+2)*xSize)+j]     = resultint[((i+2)*xSize)+j]    + (err*0.0952);
            resultint[((i+2)*xSize)+j - 1] = resultint[((i+2)*xSize)+j -1] + (err*0.0476);
            resultint[((i+2)*xSize)+j - 2] = resultint[((i+2)*xSize)+j -2] + (err*0.0238);

            //cout << "error is " << err << endl;
            }
    }
    for (int i = ySize-2; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            if (resultint[i*xSize + j] > 127)
                resultint[i*xSize + j] = 255;
            else
                resultint[i*xSize + j] = 0;
        }
    }

    return resultint;
}
