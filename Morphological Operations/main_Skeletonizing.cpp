/*
// Author Chinmay Jog
// Id: 6171795819
// Email: cjog@usc.edu
// Date: Feb 25, 2018
// Assignment 2
// Question 3- Image Skeletonizing
*/

#include <stdio.h>
#include <math.h>

unsigned char get_pattern(unsigned char** grayint, int u, int v);
unsigned char** create_2D_array(int xSize, int ySize);
	int ySize = 512;
			ySize = atoi(argv[5]);
	unsigned char* result = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* gray = new unsigned char [xSize*ySize];

	unsigned char** grayint = create_2D_array(xSize+2, ySize+2);
	unsigned char** gray2 = create_2D_array(xSize+2, ySize+2);
	unsigned char** resultint = create_2D_array(xSize, ySize);


	for (int i = 0; i < xSize*ySize; i++)
    {
        if (ImageData[i] <= 127)
            gray[i] = 1;
        else
            gray[i] = 0;
    }

    unsigned char pattern[40] = {0b10100000, 0b00101000, 0b00001010, 0b10000010,
                                 0b11000001, 0b01110000, 0b00011100, 0b00000111,
                                 0b11110000, 0b11100001, 0b01111000, 0b00111100, 0b00011110, 0b00001111, 0b10000111, 0b11000011,
                                 0b11110001, 0b01111100, 0b00011111, 0b11000111,
                                 0b11100011, 0b11111000, 0b00111110, 0b10001111,
                                 0b11110011, 0b11100111, 0b11111100, 0b11111001, 0b01111110, 0b00111111, 0b10011111, 0b11001111,
                                 0b11110111, 0b11111101, 0b01111111, 0b11011111,
                                 0b11111011, 0b11111110, 0b10111111, 0b11101111};

    unsigned char wodcmask[12] = {0b00000001, 0b00000100, 0b01000000, 0b00010000,
                                  0b00000010, 0b10000000, 0b00001000, 0b00100000,
                                  0b10100000, 0b00101000, 0b10000010, 0b00001010};

    unsigned char wdcmask[38] = {0b00111000, 0b10000011,                            //corner cluster
                                 0b10101000, 0b00101010, 0b10001010, 0b10100010,    //tee branch
                                 0b01010001, 0b00010101, 0b00010101, 0b01000101,
                                 0b01010010, 0b01010100, 0b00100101, 0b01001001,
                                 0b01010011, 0b01010101, 0b00110101, 0b01001101,
                                 0b01010100, 0b10010100, 0b01000101, 0b01010001,
                                 0b01010101, 0b10010101, 0b01010101, 0b01010101,
                                 0b01010110, 0b11010100, 0b01100101, 0b01011001,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,    //vee branch
                                 0b10100100, 0b00101001, 0b01001010, 0b10010010};   //diag branch

    unsigned char dcemask[38] = {0b00111000, 0b10000011,                             //corner cluster
                                 0b10101000, 0b00101010, 0b10001010, 0b10100010,     //tee branch
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,
                                 0b01010111, 0b11010101, 0b01110101, 0b01011101,     //vee branch
                                 0b11101110, 0b10111011, 0b11101110, 0b10111011};   //diag branch

    //padding
    int times = 14;
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

            for (int k = 0; k < 40; k++)
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

                for (int k = 0; k < 12; k++)
                {
                    if (ucpattern == wodcmask[k])
                        {
                            temp = 1;
                        }
                }

                for (int k = 0; k < 38; k++)
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
    cout << "Result success " << endl;


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