/* RGB to CMY
   CMY value is obtained by subtracting RGB value from 255.
   Simply put, output image is negative image of original input image for both colour and grayscale images.
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
	int rSize = 854;
	int cSize = 480;

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
			rSize = atoi(argv[4]);
			cSize = atoi(argv[5]);
		}
	}

	// Allocate image data array
	unsigned char* Imagedata = new unsigned char [rSize*cSize*BytesPerPixel];
	unsigned char* cmyval = new unsigned char [rSize*cSize*BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), rSize*cSize*BytesPerPixel, file);
	fclose(file);

    // Rgb to cmy

    for (int pixelcount = 0; pixelcount < rSize*cSize*BytesPerPixel; pixelcount++)
    {
        cmyval[pixelcount] = 255 - Imagedata[pixelcount];
    }

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(cmyval, sizeof(unsigned char), rSize*cSize*BytesPerPixel, file);
	fclose(file);
	delete[] Imagedata;
    delete[] cmyval;

	return 0;
}
