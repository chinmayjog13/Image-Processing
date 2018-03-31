/* Edge Detection using Sobel filter
   Input is colour
   Output_Image(1) is binary edge map
   Output_Image(2) is normalized x gradient filter output
   Output_Image(3) is normalized y gradient filter output
   Arguments must be passed as follows- Input_Image.raw Output_Image(1).raw BytesPerPixel Width Height Output_Image(2).raw Output_Image(3).raw
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>

using namespace std;

unsigned char** create_2D_array(int xSize, int ySize);
int** create_filter(int f1[3], int f2[3]);
unsigned char** pad(unsigned char** rgb, int xSize, int ySize, int window_size);
double feature(unsigned char** image, int** filter, int x, int y, int window);
double avg_filter(double** rgb, int x, int y, int window);
double** double_pad(double** rgb, int xSize, int ySize, int window_size);
double** create_2D_array_double(int xSize, int ySize);

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
	unsigned char* grey_image = new unsigned char [xSize*ySize];
	unsigned char* result = new unsigned char [xSize*ySize];
	unsigned char* xresult = new unsigned char [xSize*ySize];
	unsigned char* yresult = new unsigned char [xSize*ySize];
	unsigned char** image = create_2D_array(xSize, ySize);
	unsigned char** image_int = create_2D_array(xSize+2, ySize+2);

	double** xImage = create_2D_array_double(xSize, ySize);
	double** yImage = create_2D_array_double(xSize, ySize);
	double** magnitudeImage = create_2D_array_double(xSize, ySize);

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
        result[count1] = grey_image[count1];
        count1++;
    }
    cout << "Converted to gray " << endl;
    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            image[i][j] = grey_image[i*xSize + j];
        }
    }

    image_int = pad(image, xSize, ySize, 3);

    int sx1[3] = {1, 2, 1};
    int sx2[3] = {-1, 0, 1};
    int sy1[3] = {1, 0, -1};
    int sy2[3] = {1, 2, 1};

    int** xFilt = create_filter(sx1, sx2);
    int** yFilt = create_filter(sy1, sy2);

    double largest = 0; double smallest = 0;
    double largestx = 0; double largesty = 0;
    double smallestx = 0; double smallesty = 0;

    double** xnorm = create_2D_array_double(xSize, ySize);
    double** ynorm = create_2D_array_double(xSize, ySize);
    for (int i = 1; i < ySize+1; i++)
    {
        for (int j = 1; j < xSize+1; j++)
        {
            xImage[i-1][j-1] = pow(feature(image_int, xFilt, i, j, 3),2);
            yImage[i-1][j-1] = pow(feature(image_int, yFilt, i, j, 3),2);
            xnorm[i-1][j-1] = feature(image_int, xFilt, i, j, 3);
            ynorm[i-1][j-1] = feature(image_int, yFilt, i, j, 3);
            magnitudeImage[i-1][j-1] = sqrt(xImage[i-1][j-1] + yImage[i-1][j-1]);
            if (magnitudeImage[i-1][j-1] > largest)
                largest = magnitudeImage[i-1][j-1];
            else if (magnitudeImage[i-1][j-1] < smallest)
                smallest = magnitudeImage[i-1][j-1];

            if (xnorm[i-1][j-1] > largestx)
                largestx = xnorm[i-1][j-1];
            else if (xnorm[i-1][j-1] < smallestx)
                smallestx = xnorm[i-1][j-1];

            if (ynorm[i-1][j-1] > largesty)
                largesty = ynorm[i-1][j-1];
            else if (ynorm[i-1][j-1] < smallesty)
                smallesty = ynorm[i-1][j-1];

//            xresult[(i-1)*xSize + (j-1)] = sqrt(xImage[i-1][j-1]);
//            yresult[(i-1)*xSize + (j-1)] = sqrt(yImage[i-1][j-1]);
        }
    }
    cout << "Filters applied and magnitude image created " << endl;

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            xresult[i*xSize+j] = ((xnorm[i][j] - smallestx)/(largestx-smallestx))*255;
            yresult[i*xSize+j] = ((ynorm[i][j] - smallesty)/(largesty-smallesty))*255;
            //result[i*xSize+j] = ((magnitudeImage[i][j] - smallest)/(largest-smallest))*255;
        }
    }

    double counter[int(largest)] = {0};
    int countval;

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            countval = magnitudeImage[i][j];
            int index = countval;
            counter[index]++;
        }
    }
    cout << "histogram done " << endl;

    for (int i = 1; i < int(largest); i++)
    {
        counter[i] = counter[i-1] + counter[i];
    }
    cout << "cdf done " << endl;

    ofstream transferr("cdf_magnitude.csv");
    for (int i =0; i < int(largest); i++)
    {
        transferr << i << "," << counter[i] << endl;
    }
    transferr.close();

    int threshold;
    for (int i = 0; i < int(largest); i++)
    {
        if (counter[i] <= 0.90*(xSize*ySize))
        {
            threshold = i;
        }
    }
    cout << "Threshold is " << threshold << endl;
    //double threshold = 0.2*largest;

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            if (magnitudeImage[i][j] > threshold)
                result[i*xSize+j] = 255;
            else
                result[i*xSize+j] = 0;
        }
    }
    cout << "Thresholded magnitude image \nResult done " << endl;


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
	fwrite(xresult, sizeof(unsigned char), xSize*ySize, file);
	fclose(file);

	if (!(file=fopen(argv[7],"wb"))) {
		cout << "Cannot open file: " << argv[7] << endl;
		exit(1);
	}
	fwrite(yresult, sizeof(unsigned char), xSize*ySize, file);
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
    return (sum);
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
