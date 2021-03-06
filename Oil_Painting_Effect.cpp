/* Oil Painting Effect
   Output image looks like an oil painting
   Input image should be colour image
   Pass arguments in following order- InputImage.raw OutputImage.raw BytesPerPixel Width Height
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

unsigned char** create_2D_array(int xSize, int ySize);
unsigned char filter(unsigned char** rgb, int x, int y, int window);
unsigned char* quantize(unsigned char* r, int counterr[256], int xSize, int ySize);
unsigned char** pad(unsigned char** rgb, int xSize, int ySize, int window_size);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int xSize = 1800;
	int ySize = 1200;

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

    // Oil painting

    int row1 = 0;
    for (int row = 0; row < ySize*xSize*BytesPerPixel; row+=3)
    {
            r[row1] = Imagedata[row];
            g[row1] = Imagedata[row + 1];
            b[row1] = Imagedata[row + 2];
            row1++;
    }

    int counterr[256] = {0};
    int counterg[256] = {0};
    int counterb[256] = {0};
    unsigned char countvalr;
    unsigned char countvalg;
    unsigned char countvalb;

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

    unsigned char* imgquantr = quantize(r, counterr, xSize, ySize);
    unsigned char* imgquantg = quantize(g, counterg, xSize, ySize);
    unsigned char* imgquantb = quantize(b, counterb, xSize, ySize);

    cout << "Quant success" << endl;

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            rint[i][j] = imgquantr[i*xSize + j];
            gint[i][j] = imgquantg[i*xSize + j];
            bint[i][j] = imgquantb[i*xSize + j];
        }
    }

    int window = 3;     // Choose from 3,5,7,9 for different window sizes
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
            rresult = filter(rnew, i, j, window);
            gresult = filter(gnew, i, j, window);
            bresult = filter(bnew, i, j, window);
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

	delete[] result;
	delete[] Imagedata;
	delete[] imgquantr;
	delete[] imgquantg;
	delete[] imgquantb;
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
	return 0;
}

unsigned char filter(unsigned char** rgb, int x, int y, int window)
{
    int w2 = window*window;
    unsigned char freq[w2];
    unsigned char temp;

    for (int i = 0; i < window; i++)
    {
        for (int j = 0; j < window; j++)
        {
            freq[i*window + j] = rgb[x + i - (window/2)][y + j - (window/2)];
        }
    }

    int maxcount = 0;

    for (int i = 0; i < w2; i++)
    {
        int count1 = 1;
        for (int j = i+1; j < w2; j++)
            if (freq[i] == freq[j])
                count1++;
        if (count1 > maxcount)
            maxcount = count1;
    }

    for (int i = 0; i < w2; i++)
    {
        int count1 = 1;
        for (int j = i+1; j < w2; j++)
            if (freq[i] == freq[j])
                count1++;
        if (count1 == maxcount)
            temp = freq[i];
    }
    return temp;
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

unsigned char* quantize(unsigned char* r, int counterr[], int xSize, int ySize)
{
    int sumr = 0;
    int binr[5] = {0};
    int limitr = 1;
    int sumvalr[4] = {0};
    unsigned char* imgquantr = new unsigned char[xSize*ySize];

    for (int i = 0; i < 256; i++)
    {
        sumr += counterr[i];
        //cout << "sum at " << i << "is: " << sumr << endl;
        if (sumr > (xSize*ySize/4))
            {
                binr[limitr] = i+1;     //location of bins
                sumvalr[limitr-1] = sumr;
                limitr++;
                sumr = 0;
                //cout << "limitr is " << limitr << " " << binr[limitr-1] << " " << i << endl;
            }
    }
    sumvalr[3] = (xSize*ySize) - (sumvalr[0] + sumvalr[1] + sumvalr[2]);
    binr[4] = 256;
    cout << sumvalr[0] << " " << sumvalr[1] << " " << sumvalr[2] << " " << sumvalr[3] << endl;
    cout << "bins calculated " << endl;

    int weight_mean[4] = {0};
    int temp_sumr = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = binr[i]; j < binr[i+1]; j++)
        {
            temp_sumr += j*counterr[j];
        }
        weight_mean[i] = temp_sumr/sumvalr[i];
        temp_sumr = 0;
        cout << "weight_mean of i is " << weight_mean[i] << endl;
    }

    for (int pixelcount = 0; pixelcount < xSize*ySize; pixelcount++)
    {
        if (r[pixelcount] < binr[1])
        {
            imgquantr[pixelcount] = weight_mean[0];
        }
        else if (r[pixelcount] < binr[2])
        {
            imgquantr[pixelcount] = weight_mean[1];
        }
        else if (r[pixelcount] < binr[3])
        {
            imgquantr[pixelcount] = weight_mean[2];
        }
        else
        {
            imgquantr[pixelcount] = weight_mean[3];
        }
    }
    return imgquantr;
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
