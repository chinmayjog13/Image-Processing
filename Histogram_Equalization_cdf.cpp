/* Histogram Equalization using cdf method
   Code is written for colour images, initial and equalised histogram of all channels is stored in a separate csv file
   Histogram can be plotted in MATLAB, python or any other grpah plotting software
   Pass arguments in following order- InputImage.raw OutputImage.raw BytesPerPixel Width Height
   Author- Chinmay Jog
*/


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <fstream>

using namespace std;

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

    // Histogram equalization
    int row1 = 0;
    for (int row = 0; row < ySize*xSize*BytesPerPixel; row+=3)
    {
            r[row1] = Imagedata[row];
            g[row1] = Imagedata[row + 1];
            b[row1] = Imagedata[row + 2];
            row1++;
    }

    double counterr[256] = {0};
    double counterg[256] = {0};
    double counterb[256] = {0};
    unsigned char countvalr;
    unsigned char countvalg;
    unsigned char countvalb;
    unsigned char* newred = new unsigned char[xSize*ySize];
    unsigned char* newgreen = new unsigned char [xSize*ySize];
    unsigned char* newblue = new unsigned char [xSize*ySize];


    for (int j = 0; j < xSize*ySize; j++)
    {
        countvalr = r[j];
        counterr[countvalr]++;
        countvalg = g[j];
        counterg[countvalg]++;
        countvalb = b[j];
        counterb[countvalb]++;
    }
    cout << "histogram done " << endl;

    ofstream histr_original("hist_red_original.csv");
    for (int i =0; i < 256; i++)
    {
        histr_original << i << "," << counterr[i] << endl;
    }
    histr_original.close();

    ofstream histg_original("hist_green_original.csv");
    for (int i =0; i < 256; i++)
    {
        histg_original << i << "," << counterg[i] << endl;
    }
    histg_original.close();

    ofstream histb_original("hist_blue_original.csv");
    for (int i =0; i < 256; i++)
    {
        histb_original << i << "," << counterb[i] << endl;
    }
    histb_original.close();

    for (int i = 0; i < 256; i++)
    {
        counterr[i] = counterr[i]/(xSize*ySize);
        counterg[i] = counterg[i]/(xSize*ySize);
        counterb[i] = counterb[i]/(xSize*ySize);
        //cout << "i is " << i << endl;
    }
    cout << "probability done" << endl;

    for (int i = 1; i < 256; i++)
    {
        counterr[i] = counterr[i-1] + counterr[i];
        counterg[i] = counterg[i-1] + counterg[i];
        counterb[i] = counterb[i-1] + counterb[i];
    }
    cout << "cdf done " << endl;

    for (int i = 0; i < 256; i++)
    {
        counterr[i] = int(counterr[i]*255);
        counterg[i] = int(counterg[i]*255);
        counterb[i] = int(counterb[i]*255);
    }

    ofstream transferr("tranfer_red.csv");
    for (int i =0; i < 256; i++)
    {
        transferr << i << "," << counterr[i] << endl;
    }
    transferr.close();

    ofstream transferg("tranfer_green.csv");
    for (int i =0; i < 256; i++)
    {
        transferg << i << "," << counterg[i] << endl;
    }
    transferg.close();

    ofstream transferb("tranfer_blue.csv");
    for (int i =0; i < 256; i++)
    {
        transferb << i << "," << counterb[i] << endl;
    }
    transferb.close();


   unsigned char* red = new unsigned char[ySize*xSize];
   unsigned char* green = new unsigned char[ySize*xSize];
   unsigned char* blue = new unsigned char[ySize*xSize];

    for (int i = 0; i < ySize*xSize; i++)
    {
        int valuer = r[i];
        red[i] = counterr[valuer];
        int valueg = g[i];
        green[i] = counterg[valueg];
        int valueb = b[i];
        blue[i] = counterb[valueb];

        result[3*i] = red[i];
        result[3*i + 1] = green[i];
        result[3*i + 2] = blue[i];
    }

    cout << "result success" << endl;

    int counterr_new[256] = {0};
    int counterg_new[256] = {0};
    int counterb_new[256] = {0};
    unsigned char countvalr_new;
    unsigned char countvalg_new;
    unsigned char countvalb_new;

    for (int j = 0; j < xSize*ySize; j++)
    {
        countvalr_new = red[j];
        counterr_new[countvalr_new]++;
        countvalg_new = green[j];
        counterg_new[countvalg_new]++;
        countvalb_new = blue[j];
        counterb_new[countvalb_new]++;
    }
    cout << "new histogram done " << endl;

    ofstream histr_new("hist_red_new.csv");
    for (int i =0; i < 256; i++)
    {
        histr_new << i << "," << counterr_new[i] << endl;
    }
    histr_new.close();

    ofstream histg_new("hist_green_new.csv");
    for (int i =0; i < 256; i++)
    {
        histg_new << i << "," << counterg_new[i] << endl;
    }
    histg_new.close();

    ofstream histb_new("hist_blue_new.csv");
    for (int i =0; i < 256; i++)
    {
        histb_new << i << "," << counterb_new[i] << endl;
    }
    histb_new.close();


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
	delete[] red;
	delete[] green;
	delete[] blue;
	delete[] newred;
	delete[] newgreen;
	delete[] newblue;
	return 0;
}
