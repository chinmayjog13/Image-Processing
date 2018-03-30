/*
// Author Chinmay Jog
// Id: 6171795819
// Email: cjog@usc.edu
// Date: Feb 25, 2018
// Assignment 2
// Question 1- Image Warping
*/#include <stdio.h>#include <iostream>#include <stdlib.h>
#include <math.h>using namespace std;

unsigned char** create_2D_array(int xSize, int ySize);
unsigned char map_to_circle(unsigned char** plane, double u, double v);
unsigned char map_to_square(unsigned char** plane, double x, double y);int main(int argc, char *argv[]){	// Define file pointer and variables	FILE *file;	int BytesPerPixel;	int xSize = 512;
	int ySize = 512;	// Check for proper syntax	if (argc < 3){		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;		return 0;	}	// Check if image is grayscale or color	if (argc < 4){		BytesPerPixel = 1; // default is grey image	}	else {		BytesPerPixel = atoi(argv[3]);		// Check if size is specified		if (argc >= 5){			xSize = atoi(argv[4]);
			ySize = atoi(argv[5]);		}	}	// Allocate image data array	unsigned char* Imagedata = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* result_circle = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* result_square = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* r = new unsigned char [xSize*ySize];
	unsigned char* g = new unsigned char [xSize*ySize];
	unsigned char* b = new unsigned char [xSize*ySize];

	unsigned char** rin = create_2D_array(xSize, ySize);
    unsigned char** bin = create_2D_array(xSize, ySize);
    unsigned char** gin = create_2D_array(xSize, ySize);
    unsigned char** rout = create_2D_array(xSize, ySize);
    unsigned char** gout = create_2D_array(xSize, ySize);
    unsigned char** bout = create_2D_array(xSize, ySize);
    unsigned char** routs = create_2D_array(xSize, ySize);
    unsigned char** gouts = create_2D_array(xSize, ySize);
    unsigned char** bouts = create_2D_array(xSize, ySize);	// Read image (filename specified by first argument) into image data matrix	if (!(file=fopen(argv[1],"rb"))) {		cout << "Cannot open file: " << argv[1] <<endl;		exit(1);	}	fread(Imagedata, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);	fclose(file);	// Warping

	int count1 = 0;
	for (int i = 0; i < xSize*ySize*BytesPerPixel; i+=3)
    {
        r[count1] = Imagedata[i];
        g[count1] = Imagedata[i+1];
        b[count1] = Imagedata[i+2];
        result_circle[i] = 0;
        count1++;
    }

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            rin[i][j] = r[i*xSize + j];
            gin[i][j] = g[i*xSize + j];
            bin[i][j] = b[i*xSize + j];
            rout[i][j] = 0;
            gout[i][j] = 0;
            bout[i][j] = 0;
            routs[i][j] = 0;
            gouts[i][j] = 0;
            bouts[i][j] = 0;
        }
    }

    // Warping function call
    for (int i = 0; i < ySize; i++)
    {
        double x1 = double(i)/256;
        for (int j = 0; j < xSize; j++)
        {
            double y1 = double(j)/256;
            rout[i][j] = map_to_circle(rin, x1, y1);
            gout[i][j] = map_to_circle(gin, x1, y1);
            bout[i][j] = map_to_circle(bin, x1, y1);
            result_circle[i*xSize*3+j*3] = rout[i][j];
            result_circle[i*xSize*3+j*3+1] = gout[i][j];
            result_circle[i*xSize*3+j*3+2] = bout[i][j];
        }
    }
    cout << "circle mapping done " << endl;

    for (int i = 0; i < ySize; i++)
    {
        double x1 = double(i)/256;
        for (int j = 0; j < xSize; j++)
        {
            double y1 = double(j)/256;
            routs[i][j] = map_to_square(rout, x1, y1);
            gouts[i][j] = map_to_square(gout, x1, y1);
            bouts[i][j] = map_to_square(bout, x1, y1);
            result_square[i*xSize*3+j*3] = routs[i][j];
            result_square[i*xSize*3+j*3+1] = gouts[i][j];
            result_square[i*xSize*3+j*3+2] = bouts[i][j];
        }
    }
    cout << "Result success " << endl;

	// Write image data (filename specified by second argument) from image data matrix	if (!(file=fopen(argv[2],"wb"))) {		cout << "Cannot open file: " << argv[2] << endl;		exit(1);	}	fwrite(result_circle, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);	fclose(file);

	if (!(file=fopen(argv[6],"wb"))) {		cout << "Cannot open file: " << argv[6] << endl;		exit(1);	}	fwrite(result_square, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);	fclose(file);

	delete[] Imagedata;
	delete[] result_circle;
	delete[] result_square;
	delete[] r;
	delete[] g;
	delete[] b;
	for (int i = 0; i < ySize; i++)
    {
        delete[] rin[i];
        delete[] bin[i];
        delete[] gin[i];
        delete[] rout[i];
        delete[] gout[i];
        delete[] bout[i];
        delete[] routs[i];
        delete[] gouts[i];
        delete[] bouts[i];
    }
	delete[] rin;
	delete[] bin;
	delete[] gin;
	delete[] rout;
	delete[] gout;
	delete[] bout;
	delete[] routs;
    delete[] gouts;
    delete[] bouts;	return 0;}



unsigned char** create_2D_array(int xSize, int ySize)
{
    unsigned char** newArray = new unsigned char*[ySize];
    for (int i = 0; i < ySize; i++)
    {
        newArray[i] = new unsigned char[xSize];
    }
    return newArray;
}
unsigned char map_to_circle(unsigned char** plane, double u, double v)
{
    u = u - 1;
    v = v - 1;
    double x = 0;
    double y = 0;
    int left = 0;
    int top = 0;
    double delX = 0;
    double delY = 0;
    unsigned char output = 0;

    if (sqrt((u)*(u) + (v)*(v)) <= 1)
    {
        x = ((0.5*(sqrt(2 + (u*u) - (v*v) + 2*sqrt(2)*u))) - (0.5*(sqrt(2 + (u*u) - (v*v) - 2*sqrt(2)*u))) + 1)*256;
        y = ((0.5*(sqrt(2 - (u*u) + (v*v) + 2*sqrt(2)*v))) - (0.5*(sqrt(2 - (u*u) + (v*v) - 2*sqrt(2)*v))) + 1)*256;
        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;
        if (x >= 511)
            {left = floor(x)-1;}
        else
            {left = floor(x);}
        if (y >= 511)
            {top = floor(y)-1;}
        else
            {top = floor(y);}
        delX = x - left;
        delY = y - top;

        output = ((1-delX)*(1-delY)*plane[left][top]) + ((delX)*(1-delY)*plane[left][top+1])
            + ((1-delX)*(delY)*plane[left+1][top]) + ((delX)*(delY)*plane[left+1][top+1]);
    }
    return output;
}

unsigned char map_to_square(unsigned char** plane, double x, double y)
{
    x = x - 1;
    y = y - 1;
    double u = 0;
    double v = 0;
    int left = 0;
    int top = 0;
    double delU = 0;
    double delV = 0;
    unsigned char output = 0;

    u = (x*sqrt(1 - (y*y/2)) + 1)*256;
    v = (y*sqrt(1 - (x*x/2)) + 1)*256;

    if (u >= 511)
        left = floor(u)-1;
    else
        left = floor(u);
    if (v >= 511)
        top = floor(v)-1;
    else
        top = floor(v);
    delU = u - left;
    delV = v - top;

    output = ((1-delU)*(1-delV)*plane[left][top]) + ((delU)*(1-delV)*plane[left][top+1])
            + ((1-delU)*(delV)*plane[left+1][top]) + ((delU)*(delV)*plane[left+1][top+1]);
    return output;
}
