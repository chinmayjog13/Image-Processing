/* RGB to Gray
   Lightness method
   Gray value is obtained by taking (max(R,G,B)+min(R,G,B)/2)
   Pass arguments in following order- InputImage.raw OutputImage.raw BytesPerPixel Width Height
   Author- Chinmay Jog
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int xSize;
	int ySize;

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
	unsigned char Imagedata[xSize][ySize][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

    // Rgb to Gray by Lightness method

    unsigned char grayval[xSize][ySize];
    int small = 0;
    int large = 0;
    for(int row = 0; row < ySize; row++)
    {
        for(int col = 0; col < xSize; col++)
        {
            if(Imagedata[row][col][0] < Imagedata[row][col][1] && Imagedata[row][col][0] < Imagedata[row][col][2])
            {
                small = Imagedata[row][col][0];
                if(Imagedata[row][col][1] > Imagedata[row][col][2])
                    large = Imagedata[row][col][1];
                else
                    large = Imagedata[row][col][2];
            }
            else if(Imagedata[row][col][1] < Imagedata[row][col][0] && Imagedata[row][col][1] < Imagedata[row][col][2])
            {
                small = Imagedata[row][col][1];
                if(Imagedata[row][col][0] > Imagedata[row][col][2])
                    large = Imagedata[row][col][0];
                else
                    large = Imagedata[row][col][2];
            }
            else
            {
                small = Imagedata[row][col][2];
                if(Imagedata[row][col][1] > Imagedata[row][col][0])
                    large = Imagedata[row][col][1];
                else
                    large = Imagedata[row][col][0];
            }
            grayval[row][col] = (small + large) / 2;
        }
    }
    
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(grayval, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);
	return 0;
}
