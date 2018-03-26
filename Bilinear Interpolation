/* Image resizing using Bilinear Interpolation
   This code is for square images, but it can be used for all images with a few simple changes
   Pass arguments in following order- InputImage.raw OutputImage.raw BytesPerPixel Input_Size Output_Size
   Author- Chinmay Jog
*/


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Size = 512;
	int newSize = 650;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
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
			Size = atoi(argv[4]);
			newSize = atoi(argv[5]);
		}
	}

	// Allocate image data array
	unsigned char Imagedata[Size][Size][BytesPerPixel];
	unsigned char resizeImg[newSize][newSize][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

    // Resizing

    //double displacement = 0.7873;	// 512/650
    double displacement = double(Size)/double(newSize);
    for (int resizedrow = 0; resizedrow < newSize; resizedrow++)
    {
        for (int resizedcol = 0; resizedcol < newSize; resizedcol++)
        {
            double dispX = resizedrow * displacement;
            double dispY = resizedcol * displacement;
            double baseX = floor(dispX);
            double baseY = floor(dispY);
            double delX = dispX - baseX;
            double delY = dispY - baseY;
            for (int plcount = 0; plcount < BytesPerPixel; plcount++)
            {
                resizeImg[resizedrow][resizedcol][plcount] = ((1-delX)*(1-delY)*Imagedata[int(baseX)][int(baseY)][plcount])
                                                            + ((delX)*(1-delY)*Imagedata[int(baseX)][int(baseY)+1][plcount])
                                                            + ((1-delX)*(delY)*Imagedata[int(baseX)+1][int(baseY)][plcount])
                                                            + ((delX)*(delY)*Imagedata[int(baseX)+1][int(baseY)+1][plcount]);
            }
        }
    }
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(resizeImg, sizeof(unsigned char), newSize*newSize*BytesPerPixel, file);
	fclose(file);
	return 0;
}
