/* Noise Removal for colour images
   Median filter and gaussian filter have been used. Unwanted filter can be commented out
   Pass arguments in following order- InputImage.raw OutputImage.raw BytesPerPixel Width Height
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <math.h>

using namespace std;

unsigned char** create_2D_array(int xSize, int ySize);
unsigned char med_filter(unsigned char** rgb, int x, int y, int window);
unsigned char avg_filter(unsigned char** rgb, int x, int y, int window);
unsigned char** pad(unsigned char** rgb, int xSize, int ySize, int window);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int xSize = 600;
	int ySize = 338;

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
	unsigned char* Imagedata1 = new unsigned char [ySize*xSize*BytesPerPixel];
	unsigned char* r = new unsigned char [ySize*xSize];
	unsigned char* g = new unsigned char [ySize*xSize];
	unsigned char* b = new unsigned char [ySize*xSize];

    unsigned char** rint = create_2D_array(xSize, ySize);
    unsigned char** bint = create_2D_array(xSize, ySize);
    unsigned char** gint = create_2D_array(xSize, ySize);

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen(argv[6],"rb"))) {
		cout << "Cannot open file: " << argv[6] <<endl;
		exit(1);
	}
	fread(Imagedata1, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

    // Noise Removal

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
            rint[i][j] = r[i*xSize + j];
            gint[i][j] = g[i*xSize + j];
            bint[i][j] = b[i*xSize + j];
        }
    }

    int window = 3;     //choose from 3,5,7,9 for different window sizes
    int w = window/2;
    unsigned char** rnew = pad(rint, xSize, ySize, window);
    unsigned char** gnew = pad(gint, xSize, ySize, window);
    unsigned char** bnew = pad(bint, xSize, ySize, window);

    cout << "padding success" << endl;

    unsigned char* result = new unsigned char [xSize*ySize*BytesPerPixel];
    unsigned char rresult;
    unsigned char gresult;
    unsigned char bresult;

    for (int i = w; i < ySize+w; i++)
    {
        for (int j = w; j < xSize+w; j++)
        {
            rresult = med_filter(rnew, i, j, window);
            gresult = med_filter(gnew, i, j, window);
            bresult = med_filter(bnew, i, j, window);
            rint[i-w][j-w] = rresult;
            gint[i-w][j-w] = gresult;
            bint[i-w][j-w] = bresult;
        }
    }
    cout << "median filter applied " << endl;

    rnew = pad(rint, xSize, ySize, window);
    gnew = pad(gint, xSize, ySize, window);
    bnew = pad(bint, xSize, ySize, window);

    for (int i = w; i < ySize+w; i++)
    {
        for (int j = w; j < xSize+w; j++)
        {
            rresult = avg_filter(rnew, i, j, window);
            gresult = avg_filter(gnew, i, j, window);
            bresult = avg_filter(bnew, i, j, window);
            result[(i-w)*xSize*3 + (j-w)*3] = rresult;
            result[(i-w)*xSize*3 + (j-w)*3 + 1] = gresult;
            result[(i-w)*xSize*3 + (j-w)*3 + 2] = bresult;
        }
    }
    cout << "result success" << endl;

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(result, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

	unsigned char* r1 = new unsigned char [ySize*xSize];
	unsigned char* g1 = new unsigned char [ySize*xSize];
	unsigned char* b1 = new unsigned char [ySize*xSize];

	row1 = 0;
	for (int row = 0; row < ySize*xSize*BytesPerPixel; row+=3)
    {
            r1[row1] = Imagedata1[row];
            g1[row1] = Imagedata1[row + 1];
            b1[row1] = Imagedata1[row + 2];
            row1++;
    }
    row1 = 0;
    for (int row = 0; row < ySize*xSize*BytesPerPixel; row+=3)
    {
            r[row1] = result[row];
            g[row1] = result[row + 1];
            b[row1] = result[row + 2];
            row1++;
    }

	double sumr = 0;
	double sumg = 0;
	double sumb = 0;
	for (int i = 0; i < ySize*xSize; i++)
	{
	    sumr += (r1[i] - r[i])*(r1[i] - r[i]);
	    sumg += (g1[i] - g[i])*(g1[i] - g[i]);
	    sumb += (b1[i] - b[i])*(b1[i] - b[i]);
	}
	double mser = sumr / (xSize*ySize);
	double mseg = sumg / (xSize*ySize);
	double mseb = sumb / (xSize*ySize);
	double psnr_dbr = 10 * log10(255*255/ (mser));
	double psnr_dbg = 10 * log10(255*255/ (mseg));
	double psnr_dbb = 10 * log10(255*255/ (mseb));
	cout << "psnr in db red is " << psnr_dbr << endl;
	cout << "psnr in db green is " << psnr_dbg << endl;
	cout << "psnr in db blue is " << psnr_dbb << endl;


	delete[] result;
	delete[] Imagedata;
	//delete[] imgquant;
	delete[] r;
	delete[] g;
	delete[] b;

	for (int i = 0; i < ySize; i++)
    {
        delete[] rint[i];
        delete[] bint[i];
        delete[] gint[i];
    }
	delete[] rint;
	delete[] bint;
	delete[] gint;
	for (int i = 0; i < ySize+window-1; i++)
    {
     	delete[] rnew[i];
        delete[] bnew[i];
        delete[] gnew[i];
    }
	delete[] rnew;
	delete[] bnew;
	delete[] gnew;

	delete[] r1;
	delete[] g1;
	delete[] b1;
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

unsigned char med_filter(unsigned char** rgb, int x, int y, int window)
{
    int w2 = window*window;
    unsigned char window_size[w2];
    for (int i = 0; i < window; i++)
    {
        //cout << "i is now " << i << endl;
        for (int j = 0; j < window; j++)
        {
            window_size[i*window + j] = rgb[x + i - (window/2)][y + j - (window/2)];
        }
        //cout << endl;
    }
    //cout << endl;
    sort(window_size, window_size+w2);
    return window_size[w2/2];
}

unsigned char avg_filter(unsigned char** rgb, int x, int y, int window)
{
    int w2 = window*window;
    //unsigned char window[9];
    double sum = 0;

    for (int i = 0; i < window; i++)
    {
        for (int j = 0; j < window; j++)
        {
            sum += (rgb[x + i - (window/2)][y + j - (window/2)]);
            //sum += 1/double(rgb[x + i - 1][y + j - 1]);
            //cout << "sum is " << sum << endl;
        }
    }

    int avg = sum /(w2);
    //int avg = 9/sum;
    //cout << "sum is " << sum << endl;
    //cout << "avg is " << avg << endl;
    return avg;
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

    else if (window_size == 5)
    {
        for (int row = 0; row < ySize; row++)
        {
            for (int col = 0; col < xSize; col++)
            {
                rgbnew[1][col+2] = rgb[0][col];
                rgbnew[0][col+2] = rgb[1][col];
                rgbnew[row+2][col+2] = rgb[row][col];
                rgbnew[ySize+2][col+2] = rgb[ySize-1][col];
                rgbnew[ySize+3][col+2] = rgb[ySize-2][col];
            }
        }

        for (int row = 0; row < ySize+4; row++)
        {
            rgbnew[row][0] = rgbnew[row][3];
            rgbnew[row][1] = rgbnew[row][2];
            rgbnew[row][xSize+2] = rgbnew[row][xSize];
            rgbnew[row][xSize+3] = rgbnew[row][xSize+1];
        }
    }

    else if (window_size == 7)
    {
        for (int row = 0; row < ySize; row++)
        {
            for (int col = 0; col < xSize; col++)
            {
                rgbnew[2][col+3] = rgb[0][col];
                rgbnew[1][col+3] = rgb[1][col];
                rgbnew[0][col+3] = rgb[2][col];
                rgbnew[row+3][col+3] = rgb[row][col];
                rgbnew[ySize+3][col+3] = rgb[ySize-1][col];
                rgbnew[ySize+4][col+3] = rgb[ySize-2][col];
                rgbnew[ySize+5][col+3] = rgb[ySize-3][col];
            }
        }

        for (int row = 0; row < ySize+6; row++)
        {
            rgbnew[row][0] = rgbnew[row][5];
            rgbnew[row][1] = rgbnew[row][4];
            rgbnew[row][2] = rgbnew[row][3];
            rgbnew[row][xSize+3] = rgbnew[row][xSize+2];
            rgbnew[row][xSize+4] = rgbnew[row][xSize+1];
            rgbnew[row][xSize+5] = rgbnew[row][xSize];
        }
    }

    else if (window_size == 9)
    {
        for (int row = 0; row < ySize; row++)
        {
            for (int col = 0; col < xSize; col++)
            {
                rgbnew[3][col+4] = rgb[0][col];
                rgbnew[2][col+4] = rgb[1][col];
                rgbnew[1][col+4] = rgb[2][col];
                rgbnew[0][col+4] = rgb[3][col];
                rgbnew[row+4][col+4] = rgb[row][col];
                rgbnew[ySize+4][col+4] = rgb[ySize-1][col];
                rgbnew[ySize+5][col+4] = rgb[ySize-2][col];
                rgbnew[ySize+6][col+4] = rgb[ySize-3][col];
                rgbnew[ySize+7][col+4] = rgb[ySize-4][col];
            }
        }

        for (int row = 0; row < ySize+8; row++)
        {
            rgbnew[row][0] = rgbnew[row][7];
            rgbnew[row][1] = rgbnew[row][6];
            rgbnew[row][2] = rgbnew[row][5];
            rgbnew[row][3] = rgbnew[row][4];
            rgbnew[row][xSize+4] = rgbnew[row][xSize+3];
            rgbnew[row][xSize+5] = rgbnew[row][xSize+2];
            rgbnew[row][xSize+6] = rgbnew[row][xSize+1];
            rgbnew[row][xSize+7] = rgbnew[row][xSize];
        }
    }

    return rgbnew;
}
