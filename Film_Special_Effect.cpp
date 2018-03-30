/* Film Special Effect
   Effect is created by histogram and colour space manipulation. Changing histogram will create different effects that can be tried out.
   Pass arguments in following order- InputImage.raw OutputImage.raw BytesPerPixel Width Height
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

unsigned char filter(unsigned char** rgb, int x, int y);

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
	unsigned char* filmeffect = new unsigned char[ySize*xSize*BytesPerPixel];
	unsigned char* r = new unsigned char [ySize*xSize];
	unsigned char* g = new unsigned char [ySize*xSize];
	unsigned char* b = new unsigned char [ySize*xSize];
	//unsigned char rnew[ySize+2][xSize+2];

    unsigned char** rint = new unsigned char*[ySize];
    for (int i = 0; i < ySize; i++)
    {
        rint[i] = new unsigned char[xSize];
    }

    unsigned char** bint = new unsigned char*[ySize];
    for (int i = 0; i < ySize; i++)
    {
        bint[i] = new unsigned char[xSize];
    }

    unsigned char** gint = new unsigned char*[ySize];
        for (int i = 0; i < ySize; i++)
        {
            gint[i] = new unsigned char[xSize];
        }

    unsigned char** rnew = new unsigned char*[ySize];
    for (int i = 0; i < ySize; i++)
    {
        rnew[i] = new unsigned char[xSize];
    }

    unsigned char** bnew = new unsigned char*[ySize];
    for (int i = 0; i < ySize; i++)
    {
        bnew[i] = new unsigned char[xSize];
    }

    unsigned char** gnew = new unsigned char*[ySize];
    for (int i = 0; i < ySize; i++)
    {
        gnew[i] = new unsigned char[xSize];
    }

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

    // Special Effect

    int row1 = 0;
    for (int row = 0; row < ySize*xSize*BytesPerPixel; row+=3)
    {
            r[row1] = Imagedata[row];
            g[row1] = Imagedata[row + 1];
            b[row1] = Imagedata[row + 2];
            row1++;
    }

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            rint[i][j] = r[i*xSize + (xSize-j)];
            gint[i][j] = g[i*xSize + (xSize-j)];
            bint[i][j] = b[i*xSize + (xSize-j)];
        }
    }

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            rnew[i][j] = 255 - rint[i][j];
            gnew[i][j] = 255 - gint[i][j];
            bnew[i][j] = 255 - bint[i][j]; //bint[i][j] = 0;
            rnew[i][j] = (rnew[i][j]*165)/255 + 90;
            gnew[i][j] = (gnew[i][j]*165)/255 + 35;
            bnew[i][j] = (bnew[i][j]*150)/255 + 25;
        }
    }

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            filmeffect[i*xSize*3 + j*3] = rnew[i][j];
            filmeffect[i*xSize*3 + j*3 + 1] = gnew[i][j];
            filmeffect[i*xSize*3 + j*3 + 2] = bnew[i][j];
        }
    }
    cout << "result success" << endl;

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(filmeffect, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

	delete[] Imagedata;
	delete[] filmeffect;
	delete[] r;
	delete[] g;
	delete[] b;

	for (int i = 0; i < ySize; i++)
    {
        delete[] rint[i];
        delete[] bint[i];
        delete[] gint[i];
        delete[] rnew[i];
        delete[] gnew[i];
        delete[] bnew[i];
    }
	delete[] rint;
	delete[] bint;
	delete[] gint;
	delete[] rnew;
	delete[] bnew;
	delete[] gnew;

	return 0;
}
