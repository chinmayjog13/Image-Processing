/*
// Author Chinmay Jog
// Id: 6171795819
// Email: cjog@usc.edu
// Date: Feb 25, 2018
// Assignment 2
// Question 3- Image Counting game
*/#include <stdio.h>#include <iostream>#include <stdlib.h>
#include <math.h>using namespace std;

unsigned char get_pattern(unsigned char** grayint, int u, int v);
unsigned char** create_2D_array(int xSize, int ySize);int main(int argc, char *argv[]){	// Define file pointer and variables	FILE *file;	int BytesPerPixel;	int xSize = 512;
	int ySize = 512;	// Check for proper syntax	if (argc < 3){		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;		return 0;	}	// Check if image is grayscale or color	if (argc < 4){		BytesPerPixel = 1; // default is grey image	}	else {		BytesPerPixel = atoi(argv[3]);		// Check if size is specified		if (argc >= 5){			xSize = atoi(argv[4]);
			ySize = atoi(argv[5]);		}	}	// Allocate image data array	unsigned char* ImageData = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* result = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* gray = new unsigned char [xSize*ySize];

	unsigned char** grayint = create_2D_array(xSize+2, ySize+2);
	unsigned char** gray2 = create_2D_array(xSize+2, ySize+2);
	unsigned char** resultint = create_2D_array(xSize, ySize);
	// Read images (filename specified by first 3 arguments) into image data matrix	if (!(file=fopen(argv[1],"rb"))) {		cout << "Cannot open file: " << argv[1] <<endl;		exit(1);	}	fread(ImageData, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);	fclose(file);
	// Stitching
	for (int i = 0; i < xSize*ySize; i++)
    {
        if (ImageData[i] <= 220)
            gray[i] = 1;
        else
            gray[i] = 0;
    }

    unsigned char pattern[58] = {0b01000000, 0b00010000, 0b00000100, 0b00000001,
                                 0b10000000, 0b00100000, 0b00001000, 0b00000010,
                                 0b11000000, 0b01100000, 0b00110000, 0b00011000, 0b00001100, 0b00000110, 0b00000011, 0b10000001,
                                 0b11000001, 0b01110000, 0b00011100, 0b00000111,
                                 0b10110000, 0b10100001, 0b01101000, 0b11000010,
                                 0b11100000, 0b00111000, 0b00001110, 0b10000011,
                                 0b10110001, 0b01101100,
                                 0b11110000, 0b11100001, 0b01111000, 0b00111100, 0b00011110, 0b00001111, 0b10000111, 0b11000011,
                                 0b11110001, 0b01111100, 0b00011111, 0b11000111,
                                 0b11100011, 0b11111000, 0b00111110, 0b10001111,
                                 0b11110011, 0b11100111, 0b11111100, 0b11111001, 0b01111110, 0b00111111, 0b10011111, 0b11001111,
                                 0b11110111, 0b11111101, 0b01111111, 0b11011111};

    unsigned char wodcmask[28] = {0b01000000, 0b00010000,
                                  0b00000010, 0b10000000,
                                  0b11000000, 0b01100000, 0b00110000, 0b00011000, 0b00001100, 0b00000110, 0b00000011, 0b10000001,
                                  0b01101000, 0b10110000, 0b10100001, 0b11000010,
                                  0b01100100, 0b00011001, 0b01000110, 0b00010011,
                                  0b11000100, 0b00110001, 0b01001100, 0b10010001,
                                  0b11100100, 0b00111001, 0b01001110, 0b10010011};

    unsigned char wdcmask[41] = {0b00111000,                                                                          //corner cluster
                                 0b10101000, 0b10101000, 0b10001010, 0b10001010, 0b00101010, 0b00101010, 0b10100010, 0b10100010,    //tee branch
                                 0b01010001, 0b00010101, 0b00010101, 0b01000101,
                                 0b01010010, 0b01010100, 0b00100101, 0b01001001,
                                 0b01010011, 0b01010101, 0b00110101, 0b01001101,
                                 0b01010100, 0b10010100, 0b01000101, 0b01010001,
                                 0b01010101, 0b10010101, 0b01010101, 0b01010101,
                                 0b01010110, 0b11010100, 0b01100101, 0b01011001,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,                                            //vee branch
                                 0b10100100, 0b00101001, 0b01001010, 0b10010010};                                           //diag branch

    unsigned char dcemask[41] = {0b00111000,                                        //corner cluster
                                 0b11101011, 0b10111110, 0b10111110, 0b11101011, 0b10101111, 0b11111010, 0b11111010, 0b10101111,    //tee branch
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,    //vee branch
                                 0b11101110, 0b10111011, 0b11101110, 0b10111011};   //diag branch


    //shrinking
    int times = 50;
    int countpiece = 0;
    for (int iteration = 0; iteration < times; iteration++)
    {

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            grayint[0][j] = 0;
            grayint[i+1][j+1] = gray[i*xSize+j];
            grayint[ySize+1][j] = 0;

            gray2[0][j] = 0;
            gray2[i+1][j+1] = 0;
            gray2[ySize+1][j] = 0;

            resultint[i][j] = gray[i*xSize+j];
        }
        grayint[i][0] = 0;
        grayint[i][xSize+1] = 0;

        gray2[i][0] = 0;
        gray2[i][xSize+1] = 0;
    }

    //cout << "shrinking begins " << endl;
    for (int i = 1; i < ySize+1; i++)
    {
        //cout << "i = " << i << endl;
        for (int j = 1; j < xSize+1; j++)
        {
            //cout << "j = " << j << endl;
            unsigned char cpattern = 0b00000000;
            if(grayint[i][j] == 1)
            {
                cpattern = get_pattern(grayint, i, j);
                //cout << int(cpattern) << endl;
                //cout << " " << endl;
            }

            for (int k = 0; k < 58; k++)
            {
                if (cpattern == pattern[k])
                {
                    gray2[i][j] = 1;
                }
            }
        }
    }

    //cout << "unconditional begins " << endl;
    for (int i = 1; i < ySize+1; i++)
    {
        for (int j = 1; j < xSize+1; j++)
        {
            int temp = 0;
            unsigned char ucpattern = 0b00000000;
            if (gray2[i][j] == 1)
            {
                ucpattern = get_pattern(gray2, i ,j);

                for (int k = 0; k < 28; k++)
                {
                    if (ucpattern == wodcmask[k])
                        {
                            temp = 1;
                        }
                }

                for (int k = 0; k < 41; k++)
                {
                    unsigned char pixelstack = ucpattern & dcemask[k];
                    if (pixelstack == wdcmask[k])
                        {
                            temp = 1;
                        }
                }

                if (temp == 1)
                {
                    resultint[i-1][j-1] = 1;
                }
                else
                {
                    resultint[i-1][j-1] = 0;
                }
            }
        }
    }
    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            gray[i*xSize+j] = resultint[i][j];
            result[i*xSize+j] = ((1 - resultint[i][j])*255);
        }
    }

    }

    for (int i = 1; i < ySize-1; i++)
    {
        for (int j = 1; j < xSize-1; j++)
        {
            if (resultint[i][j] == 1)
            {
                unsigned char countpattern = get_pattern(resultint, i, j);
                if (countpattern == 0)
                    countpiece++;
            }
        }
    }
    cout << "Result success " << endl;
    cout << "no of pieces are " << countpiece << endl;

	// Write image data (filename specified by second argument) from image data matrix	if (!(file=fopen(argv[2],"wb"))) {		cout << "Cannot open file: " << argv[2] << endl;		exit(1);	}	fwrite(result, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);	fclose(file);

	delete[] ImageData;
	delete[] gray;
	delete[] result;
	for (int i = 0; i < ySize+2; i++)
    {
        delete[] grayint[i];
        delete[] gray2[i];
    }
    for (int i = 0; i < ySize; i++)
        delete[] resultint[i];

    delete[] grayint;
    delete[] gray2;
    delete[] resultint;
	return 0;}

unsigned char** create_2D_array(int xSize, int ySize)
{
    unsigned char** newArray = new unsigned char*[ySize];
    for (int i = 0; i < ySize; i++)
    {
        newArray[i] = new unsigned char[xSize];
    }
    return newArray;
}


unsigned char get_pattern(unsigned char** grayint, int i, int j)
{
    //cout << " getting pattern " << endl;
    unsigned char mask = 0b10000000;
    unsigned char output = 0b00000000;
    unsigned char frame[8];
    frame[0] = grayint[i][j+1];
    frame[1] = grayint[i-1][j+1];
    frame[2] = grayint[i-1][j];
    frame[3] = grayint[i-1][j-1];
    frame[4] = grayint[i][j-1];
    frame[5] = grayint[i+1][j-1];
    frame[6] = grayint[i+1][j];
    frame[7] = grayint[i+1][j+1];
    for (int k = 0; k < 8; k++)
    {
        if (frame[k] == 1)
        {
            output = mask | output;
            mask = mask >> 1;
        }
        else
            mask = mask >> 1;
    }
    return output;
}
