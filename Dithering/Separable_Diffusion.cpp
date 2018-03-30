/* Separable Colour Error Diffusion
   Floyd Steinberg serpentine diffusion is used
   Pass arguments in following order- Input_Image.raw Output_Image.raw BytesPerPixel Width Height
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
	unsigned char* Imagedata = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* result = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* result_c = new unsigned char [xSize*ySize];
	unsigned char* result_m = new unsigned char [xSize*ySize];
	unsigned char* result_y = new unsigned char [xSize*ySize];
	double* resultint_c = new double [xSize*ySize];
	double* resultint_m = new double [xSize*ySize];
	double* resultint_y = new double [xSize*ySize];


	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

	//Error Diffusion
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

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(result, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

	delete[] Imagedata;
	delete[] result;
	delete[] result_c;
	delete[] result_m;
	delete[] result_y;
	delete[] resultint_c;
	delete[] resultint_m;
	delete[] resultint_y;

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
