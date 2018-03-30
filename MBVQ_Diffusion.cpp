/* MBVQ diffusion for colour images
   Pass arguments in following order- InputImage.raw OutputImage.raw BytesPerPixel Width Height
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

double*** create_3D_array(int xSize, int ySize, int BytesPerPixel);
void cmyw(double*** tempData, int i, int j);
void krgb(double*** tempData, int i, int j);
void mygc(double*** tempData, int i, int j);
void rgmy(double*** tempData, int i, int j);
void rgbm(double*** tempData, int i, int j);
void cmgb(double*** tempData, int i, int j);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int xSize = 256;
	int ySize = 256;

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
	unsigned char* ImageData = new unsigned char [xSize*ySize*BytesPerPixel];
	double*** tempData = create_3D_array(xSize, ySize, BytesPerPixel);
	unsigned char* result = new unsigned char [xSize*ySize*BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(ImageData, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	//fread(Imagedata2, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				tempData[i][j][k] = ImageData[i*xSize*3 + j*3 + k];
			}
		}
	}

	// decide quadrant
	for (int i = 0; i < ySize-1; i++)
	{
		for (int j = 1; j < xSize-1; j++)
		{
			if (tempData[i][j][0] + tempData[i][j][1] > 255)
			{
				if (tempData[i][j][1] + tempData[i][j][2] > 255)
				{
					if (tempData[i][j][0] + tempData[i][j][1] + tempData[i][j][2] > 510)
					{
						cmyw(tempData, i, j);
					}
					else
					{
						mygc(tempData, i, j);
					}
				}
				else
				{
					rgmy(tempData, i, j);
				}
			}
			else
			{
				if (!(tempData[i][j][1] + tempData[i][j][2] > 255))
				{
					if (! (tempData[i][j][0] + tempData[i][j][1] + tempData[i][j][2] > 255))
					{
						krgb(tempData, i, j);
					}
					else
					{
						rgbm(tempData, i, j);
					}
				}
				else
				{
					cmgb(tempData, i, j);
				}
			}
		}
	}

	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				result[i*xSize*3 + j*3 + k] = tempData[i][j][k];
			}
		}
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(result, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

	delete[] ImageData;
	delete[] result;
	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			delete[] tempData[i][j];
		}
			delete[] tempData[i];
	}
	delete[] tempData;

	return 0;
}


double*** create_3D_array(int xSize, int ySize, int BytesPerPixel)
{
	double*** newArray = new double**[ySize];
    for (int i = 0; i < ySize; i++)
    {
        newArray[i] = new double*[xSize];
        for (int j = 0; j < xSize; j++)
		{
			newArray[i][j] = new double[BytesPerPixel];
		}
    }
    return newArray;
}


void cmyw(double*** tempData, int i, int j)
{
	double closest[3] = {0};
	double black[3] = {255, 255, 255};	//white
	double red[3] = {0, 255, 255};		//cyan
	double green[3] = {255, 0, 255};	//magenta
	double blue[3] = {255, 255, 0};		//yellow
	double dist_k, dist_r, dist_g, dist_b;
	double error[3] = {0};
	double oldval[3] = {0};
	//unsigned char cyan[3] = {0, 255, 255};
	//unsigned char magenta[3] = {255, 0, 255};
	//unsigned char yellow[3] = {255, 255, 0};
	//unsigned char white[3] = {255, 255, 255};

	dist_k = sqrt((tempData[i][j][0] - black[0])*(tempData[i][j][0] - black[0]) + (tempData[i][j][1] - black[1])*(tempData[i][j][1] - black[1]) + (tempData[i][j][2] - black[2])*(tempData[i][j][2] - black[2]));
	dist_r = sqrt((tempData[i][j][0] - red[0])*(tempData[i][j][0] - red[0]) + (tempData[i][j][1] - red[1])*(tempData[i][j][1] - red[1]) + (tempData[i][j][2] - red[2])*(tempData[i][j][2] - red[2]));
	dist_g = sqrt((tempData[i][j][0] - green[0])*(tempData[i][j][0] - green[0]) + (tempData[i][j][1] - green[1])*(tempData[i][j][1] - green[1]) + (tempData[i][j][2] - green[2])*(tempData[i][j][2] - green[2]));
	dist_b = sqrt((tempData[i][j][0] - blue[0])*(tempData[i][j][0] - blue[0]) + (tempData[i][j][1] - blue[1])*(tempData[i][j][1] - blue[1]) + (tempData[i][j][2] - blue[2])*(tempData[i][j][2] - blue[2]));

	double close = min(min(min(dist_k, dist_r), dist_g), dist_b);
	if (close == dist_k)
	{
		closest[0] = black[0];
		closest[1] = black[1];
		closest[2] = black[2];
	}
	else if (close == dist_r)
	{
		closest[0] = red[0];
		closest[1] = red[1];
		closest[2] = red[2];
	}
	else if (close == dist_g)
	{
		closest[0] = green[0];
		closest[1] = green[1];
		closest[2] = green[2];
	}
	else
	{
		closest[0] = blue[0];
		closest[1] = blue[1];
		closest[2] = blue[2];
	}

	for (int k = 0; k < 3; k++)
	{
		oldval[k] = tempData[i][j][k];
		tempData[i][j][k] = closest[k];
		error[k] = oldval[k] - tempData[i][j][k];
	}
	for (int k = 0; k < 3; k++)
	{
		tempData[i][j+1][k] = tempData[i][j+1][k] + (error[k]*0.4375);
		tempData[i+1][j+1][k] = tempData[i+1][j+1][k] + (error[k]*0.0625);
		tempData[i+1][j][k] = tempData[i+1][j][k] + (error[k]*0.3125);
		tempData[i+1][j-1][k] = tempData[i+1][j-1][k] + (error[k]*0.1875);
	}

}

void krgb(double*** tempData, int i, int j)
{
	double closest[3] = {0};
	double black[3] = {0, 0, 0};
	double red[3] = {255, 0, 0};
	double green[3] = {0, 255, 0};
	double blue[3] = {0, 0, 255};
	double dist_k, dist_r, dist_g, dist_b;
	double error[3] = {0};
	double oldval[3] = {0};
	//unsigned char cyan[3] = {0, 255, 255};
	//unsigned char magenta[3] = {255, 0, 255};
	//unsigned char yellow[3] = {255, 255, 0};
	//unsigned char white[3] = {255, 255, 255};

	dist_k = sqrt((tempData[i][j][0] - black[0])*(tempData[i][j][0] - black[0]) + (tempData[i][j][1] - black[1])*(tempData[i][j][1] - black[1]) + (tempData[i][j][2] - black[2])*(tempData[i][j][2] - black[2]));
	dist_r = sqrt((tempData[i][j][0] - red[0])*(tempData[i][j][0] - red[0]) + (tempData[i][j][1] - red[1])*(tempData[i][j][1] - red[1]) + (tempData[i][j][2] - red[2])*(tempData[i][j][2] - red[2]));
	dist_g = sqrt((tempData[i][j][0] - green[0])*(tempData[i][j][0] - green[0]) + (tempData[i][j][1] - green[1])*(tempData[i][j][1] - green[1]) + (tempData[i][j][2] - green[2])*(tempData[i][j][2] - green[2]));
	dist_b = sqrt((tempData[i][j][0] - blue[0])*(tempData[i][j][0] - blue[0]) + (tempData[i][j][1] - blue[1])*(tempData[i][j][1] - blue[1]) + (tempData[i][j][2] - blue[2])*(tempData[i][j][2] - blue[2]));

	double close = min(min(min(dist_k, dist_r), dist_g), dist_b);
	if (close == dist_k)
	{
		closest[0] = black[0];
		closest[1] = black[1];
		closest[2] = black[2];
	}
	else if (close == dist_r)
	{
		closest[0] = red[0];
		closest[1] = red[1];
		closest[2] = red[2];
	}
	else if (close == dist_g)
	{
		closest[0] = green[0];
		closest[1] = green[1];
		closest[2] = green[2];
	}
	else
	{
		closest[0] = blue[0];
		closest[1] = blue[1];
		closest[2] = blue[2];
	}

	for (int k = 0; k < 3; k++)
	{
		oldval[k] = tempData[i][j][k];
		tempData[i][j][k] = closest[k];
		error[k] = oldval[k] - tempData[i][j][k];
	}
	for (int k = 0; k < 3; k++)
	{
		tempData[i][j+1][k] = tempData[i][j+1][k] + (error[k]*0.4375);
		tempData[i+1][j+1][k] = tempData[i+1][j+1][k] + (error[k]*0.0625);
		tempData[i+1][j][k] = tempData[i+1][j][k] + (error[k]*0.3125);
		tempData[i+1][j-1][k] = tempData[i+1][j-1][k] + (error[k]*0.1875);
	}

}

void mygc(double*** tempData, int i, int j)
{
	double closest[3] = {0};
	double black[3] = {255, 0, 255};	// magenta
	double red[3] = {255, 255, 0};		// yellow
	double green[3] = {0, 255, 0};		// green
	double blue[3] = {0, 255, 255};		// cyan
	double dist_k, dist_r, dist_g, dist_b;
	double error[3] = {0};
	double oldval[3] = {0};
	//unsigned char cyan[3] = {0, 255, 255};
	//unsigned char magenta[3] = {255, 0, 255};
	//unsigned char yellow[3] = {255, 255, 0};
	//unsigned char white[3] = {255, 255, 255};

	dist_k = sqrt((tempData[i][j][0] - black[0])*(tempData[i][j][0] - black[0]) + (tempData[i][j][1] - black[1])*(tempData[i][j][1] - black[1]) + (tempData[i][j][2] - black[2])*(tempData[i][j][2] - black[2]));
	dist_r = sqrt((tempData[i][j][0] - red[0])*(tempData[i][j][0] - red[0]) + (tempData[i][j][1] - red[1])*(tempData[i][j][1] - red[1]) + (tempData[i][j][2] - red[2])*(tempData[i][j][2] - red[2]));
	dist_g = sqrt((tempData[i][j][0] - green[0])*(tempData[i][j][0] - green[0]) + (tempData[i][j][1] - green[1])*(tempData[i][j][1] - green[1]) + (tempData[i][j][2] - green[2])*(tempData[i][j][2] - green[2]));
	dist_b = sqrt((tempData[i][j][0] - blue[0])*(tempData[i][j][0] - blue[0]) + (tempData[i][j][1] - blue[1])*(tempData[i][j][1] - blue[1]) + (tempData[i][j][2] - blue[2])*(tempData[i][j][2] - blue[2]));

	double close = min(min(min(dist_k, dist_r), dist_g), dist_b);
	if (close == dist_k)
	{
		closest[0] = black[0];
		closest[1] = black[1];
		closest[2] = black[2];
	}
	else if (close == dist_r)
	{
		closest[0] = red[0];
		closest[1] = red[1];
		closest[2] = red[2];
	}
	else if (close == dist_g)
	{
		closest[0] = green[0];
		closest[1] = green[1];
		closest[2] = green[2];
	}
	else
	{
		closest[0] = blue[0];
		closest[1] = blue[1];
		closest[2] = blue[2];
	}

	for (int k = 0; k < 3; k++)
	{
		oldval[k] = tempData[i][j][k];
		tempData[i][j][k] = closest[k];
		error[k] = oldval[k] - tempData[i][j][k];
	}
	for (int k = 0; k < 3; k++)
	{
		tempData[i][j+1][k] = tempData[i][j+1][k] + (error[k]*0.4375);
		tempData[i+1][j+1][k] = tempData[i+1][j+1][k] + (error[k]*0.0625);
		tempData[i+1][j][k] = tempData[i+1][j][k] + (error[k]*0.3125);
		tempData[i+1][j-1][k] = tempData[i+1][j-1][k] + (error[k]*0.1875);
	}

}

void rgmy(double*** tempData, int i, int j)
{
	double closest[3] = {0};
	double black[3] = {255, 255, 0};	// yellow
	double red[3] = {255, 0, 0};
	double green[3] = {0, 255, 0};
	double blue[3] = {255, 0, 255};	// magenta
	double dist_k, dist_r, dist_g, dist_b;
	double error[3] = {0};
	double oldval[3] = {0};
	//unsigned char cyan[3] = {0, 255, 255};
	//unsigned char magenta[3] = {255, 0, 255};
	//unsigned char yellow[3] = {255, 255, 0};
	//unsigned char white[3] = {255, 255, 255};

	dist_k = sqrt((tempData[i][j][0] - black[0])*(tempData[i][j][0] - black[0]) + (tempData[i][j][1] - black[1])*(tempData[i][j][1] - black[1]) + (tempData[i][j][2] - black[2])*(tempData[i][j][2] - black[2]));
	dist_r = sqrt((tempData[i][j][0] - red[0])*(tempData[i][j][0] - red[0]) + (tempData[i][j][1] - red[1])*(tempData[i][j][1] - red[1]) + (tempData[i][j][2] - red[2])*(tempData[i][j][2] - red[2]));
	dist_g = sqrt((tempData[i][j][0] - green[0])*(tempData[i][j][0] - green[0]) + (tempData[i][j][1] - green[1])*(tempData[i][j][1] - green[1]) + (tempData[i][j][2] - green[2])*(tempData[i][j][2] - green[2]));
	dist_b = sqrt((tempData[i][j][0] - blue[0])*(tempData[i][j][0] - blue[0]) + (tempData[i][j][1] - blue[1])*(tempData[i][j][1] - blue[1]) + (tempData[i][j][2] - blue[2])*(tempData[i][j][2] - blue[2]));

	double close = min(min(min(dist_k, dist_r), dist_g), dist_b);
	if (close == dist_k)
	{
		closest[0] = black[0];
		closest[1] = black[1];
		closest[2] = black[2];
	}
	else if (close == dist_r)
	{
		closest[0] = red[0];
		closest[1] = red[1];
		closest[2] = red[2];
	}
	else if (close == dist_g)
	{
		closest[0] = green[0];
		closest[1] = green[1];
		closest[2] = green[2];
	}
	else
	{
		closest[0] = blue[0];
		closest[1] = blue[1];
		closest[2] = blue[2];
	}

	for (int k = 0; k < 3; k++)
	{
		oldval[k] = tempData[i][j][k];
		tempData[i][j][k] = closest[k];
		error[k] = oldval[k] - tempData[i][j][k];
	}
	for (int k = 0; k < 3; k++)
	{
		tempData[i][j+1][k] = tempData[i][j+1][k] + (error[k]*0.4375);
		tempData[i+1][j+1][k] = tempData[i+1][j+1][k] + (error[k]*0.0625);
		tempData[i+1][j][k] = tempData[i+1][j][k] + (error[k]*0.3125);
		tempData[i+1][j-1][k] = tempData[i+1][j-1][k] + (error[k]*0.1875);
	}

}

void rgbm(double*** tempData, int i, int j)
{
	double closest[3] = {0};
	double black[3] = {255, 0, 255};	// magenta
	double red[3] = {255, 0, 0};
	double green[3] = {0, 255, 0};
	double blue[3] = {0, 0, 255};
	double dist_k, dist_r, dist_g, dist_b;
	double error[3] = {0};
	double oldval[3] = {0};
	//unsigned char cyan[3] = {0, 255, 255};
	//unsigned char magenta[3] = {255, 0, 255};
	//unsigned char yellow[3] = {255, 255, 0};
	//unsigned char white[3] = {255, 255, 255};

	dist_k = sqrt((tempData[i][j][0] - black[0])*(tempData[i][j][0] - black[0]) + (tempData[i][j][1] - black[1])*(tempData[i][j][1] - black[1]) + (tempData[i][j][2] - black[2])*(tempData[i][j][2] - black[2]));
	dist_r = sqrt((tempData[i][j][0] - red[0])*(tempData[i][j][0] - red[0]) + (tempData[i][j][1] - red[1])*(tempData[i][j][1] - red[1]) + (tempData[i][j][2] - red[2])*(tempData[i][j][2] - red[2]));
	dist_g = sqrt((tempData[i][j][0] - green[0])*(tempData[i][j][0] - green[0]) + (tempData[i][j][1] - green[1])*(tempData[i][j][1] - green[1]) + (tempData[i][j][2] - green[2])*(tempData[i][j][2] - green[2]));
	dist_b = sqrt((tempData[i][j][0] - blue[0])*(tempData[i][j][0] - blue[0]) + (tempData[i][j][1] - blue[1])*(tempData[i][j][1] - blue[1]) + (tempData[i][j][2] - blue[2])*(tempData[i][j][2] - blue[2]));

	double close = min(min(min(dist_k, dist_r), dist_g), dist_b);
	if (close == dist_k)
	{
		closest[0] = black[0];
		closest[1] = black[1];
		closest[2] = black[2];
	}
	else if (close == dist_r)
	{
		closest[0] = red[0];
		closest[1] = red[1];
		closest[2] = red[2];
	}
	else if (close == dist_g)
	{
		closest[0] = green[0];
		closest[1] = green[1];
		closest[2] = green[2];
	}
	else
	{
		closest[0] = blue[0];
		closest[1] = blue[1];
		closest[2] = blue[2];
	}

	for (int k = 0; k < 3; k++)
	{
		oldval[k] = tempData[i][j][k];
		tempData[i][j][k] = closest[k];
		error[k] = oldval[k] - tempData[i][j][k];
	}
	for (int k = 0; k < 3; k++)
	{
		tempData[i][j+1][k] = tempData[i][j+1][k] + (error[k]*0.4375);
		tempData[i+1][j+1][k] = tempData[i+1][j+1][k] + (error[k]*0.0625);
		tempData[i+1][j][k] = tempData[i+1][j][k] + (error[k]*0.3125);
		tempData[i+1][j-1][k] = tempData[i+1][j-1][k] + (error[k]*0.1875);
	}

}

void cmgb(double*** tempData, int i, int j)
{
	double closest[3] = {0};
	double black[3] = {0, 0, 255};	//blue
	double red[3] = {0, 255, 255};		//cyan
	double green[3] = {255, 0, 255};	//magenta
	double blue[3] = {0, 255, 0};		//green
	double dist_k, dist_r, dist_g, dist_b;
	double error[3] = {0};
	double oldval[3] = {0};
	//unsigned char cyan[3] = {0, 255, 255};
	//unsigned char magenta[3] = {255, 0, 255};
	//unsigned char yellow[3] = {255, 255, 0};
	//unsigned char white[3] = {255, 255, 255};

	dist_k = sqrt((tempData[i][j][0] - black[0])*(tempData[i][j][0] - black[0]) + (tempData[i][j][1] - black[1])*(tempData[i][j][1] - black[1]) + (tempData[i][j][2] - black[2])*(tempData[i][j][2] - black[2]));
	dist_r = sqrt((tempData[i][j][0] - red[0])*(tempData[i][j][0] - red[0]) + (tempData[i][j][1] - red[1])*(tempData[i][j][1] - red[1]) + (tempData[i][j][2] - red[2])*(tempData[i][j][2] - red[2]));
	dist_g = sqrt((tempData[i][j][0] - green[0])*(tempData[i][j][0] - green[0]) + (tempData[i][j][1] - green[1])*(tempData[i][j][1] - green[1]) + (tempData[i][j][2] - green[2])*(tempData[i][j][2] - green[2]));
	dist_b = sqrt((tempData[i][j][0] - blue[0])*(tempData[i][j][0] - blue[0]) + (tempData[i][j][1] - blue[1])*(tempData[i][j][1] - blue[1]) + (tempData[i][j][2] - blue[2])*(tempData[i][j][2] - blue[2]));

	double close = min(min(min(dist_k, dist_r), dist_g), dist_b);
	if (close == dist_k)
	{
		closest[0] = black[0];
		closest[1] = black[1];
		closest[2] = black[2];
	}
	else if (close == dist_r)
	{
		closest[0] = red[0];
		closest[1] = red[1];
		closest[2] = red[2];
	}
	else if (close == dist_g)
	{
		closest[0] = green[0];
		closest[1] = green[1];
		closest[2] = green[2];
	}
	else
	{
		closest[0] = blue[0];
		closest[1] = blue[1];
		closest[2] = blue[2];
	}

	for (int k = 0; k < 3; k++)
	{
		oldval[k] = tempData[i][j][k];
		tempData[i][j][k] = closest[k];
		error[k] = oldval[k] - tempData[i][j][k];
	}
	for (int k = 0; k < 3; k++)
	{
		tempData[i][j+1][k] = tempData[i][j+1][k] + (error[k]*0.4375);
		tempData[i+1][j+1][k] = tempData[i+1][j+1][k] + (error[k]*0.0625);
		tempData[i+1][j][k] = tempData[i+1][j][k] + (error[k]*0.3125);
		tempData[i+1][j-1][k] = tempData[i+1][j-1][k] + (error[k]*0.1875);
	}

}
