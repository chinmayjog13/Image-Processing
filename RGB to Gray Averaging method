/* RGB to Gray
   Averaging method
   Gray value is obtained by taking ((R+G+B)/3)
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
	unsigned char Imagedata[xSize][ySize][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);

    // Rgb to Gray by Averaging

    unsigned char grayval[xSize][ySize];

    for(int row = 0; row < ySize; row++)
    {
        for(int col = 0; col < xSize; col++)
        {
            int sum = 0;
            for(int plane = 0; plane < 3; plane++)
            {
                sum = sum + Imagedata[row][col][plane];
            }
            grayval[row][col] = sum / 3;
        }
    }
    BytesPerPixel = 1;

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(grayval, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);
	fclose(file);
	return 0;
}
