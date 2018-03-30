/* Histogram Equalization using bucket fill method
   Output has equal number of pixels for all intensity value
   Pass arguments in following order- InputImage.raw OutputImage.raw BytesPerPixel Width Height
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <fstream>

using namespace std;

unsigned char filter(unsigned char** rgb, int x, int y);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int xSize = 400;
	int ySize = 300;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel] [Width] [Height]" << endl;
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
	unsigned char* Imagedata = new unsigned char [ySize*xSize*BytesPerPixel];
	unsigned char* result = new unsigned char[ySize*xSize*BytesPerPixel];
	unsigned char* r = new unsigned char [ySize*xSize];
	unsigned char* g = new unsigned char [ySize*xSize];
	unsigned char* b = new unsigned char [ySize*xSize];



	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

    // Histogram equalization bucket fill
    int row1 = 0;
    for (int row = 0; row < ySize*xSize*BytesPerPixel; row+=3)
    {
            r[row1] = Imagedata[row];
            g[row1] = Imagedata[row + 1];
            b[row1] = Imagedata[row + 2];
            row1++;
    }

    unsigned int* newred = new unsigned int[xSize*ySize];
    unsigned int* newgreen = new unsigned int [xSize*ySize];
    unsigned int* newblue = new unsigned int [xSize*ySize];
    unsigned char* red = new unsigned char[xSize*ySize];
    unsigned char* green = new unsigned char[xSize*ySize];
    unsigned char* blue = new unsigned char[xSize*ySize];

    int equalpixels = (xSize*ySize)/256 + 1;
    int rk = 0;
    int gk = 0;
    int bk = 0;
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < xSize*ySize; j++)
        {
            if (r[j] == i)
            {
                newred[rk] = j;
                rk++;
            }
        }

        for (int j = 0; j < xSize*ySize; j++)
        {
            if (g[j] == i)
            {
                newgreen[gk] = j;
                gk++;
            }
        }

        for (int j = 0; j < xSize*ySize; j++)
        {
            if (b[j] == i)
            {
                newblue[bk] = j;
                bk++;
            }
        }
    }
    cout << " locations stored " << endl;

    int rtemp = 0;
    int gtemp = 0;
    int btemp = 0;
    int i = 0;

    for (int j = 0; j < xSize*ySize; j++)
    {
        rtemp = newred[j];
        red[rtemp] = i;
        gtemp = newgreen[j];
        green[gtemp] = i;
        btemp = newblue[j];
        blue[btemp] = i;
        if ((j+1)%equalpixels == 0)
            i++;
    }

    int counterr[256] = {0};
    int counterg[256] = {0};
    int counterb[256] = {0};
    unsigned char countvalr;
    unsigned char countvalg;
    unsigned char countvalb;

    for (int j = 0; j < xSize*ySize; j++)
    {
        countvalr = red[j];
        counterr[countvalr]++;
        countvalg = green[j];
        counterg[countvalg]++;
        countvalb = blue[j];
        counterb[countvalb]++;
    }

    cout << "histogram done " << endl;

    // Plotting histogram into csv file

    ofstream histr("hist_red.csv");
    for (int i =0; i < 256; i++)
    {
        histr << i << "," << counterr[i] << endl;
    }
    histr.close();

    ofstream histg("hist_green.csv");
    for (int i =0; i < 256; i++)
    {
        histg << i << "," << counterg[i] << endl;
    }
    histg.close();

    ofstream histb("hist_blue.csv");
    for (int i =0; i < 256; i++)
    {
        histb << i << "," << counterb[i] << endl;
    }
    histb.close();

    for (int i = 0; i < ySize*xSize; i++)
    {
        result[3*i] = red[i];
        result[3*i + 1] = green[i];
        result[3*i + 2] = blue[i];
    }

    cout << "result success" << endl;

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(result, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

	delete[] result;
	delete[] Imagedata;
	delete[] r;
	delete[] g;
	delete[] b;
	delete[] newred;
	delete[] newgreen;
	delete[] newblue;
	delete[] red;
	delete[] green;
	delete[] blue;
	return 0;
}
