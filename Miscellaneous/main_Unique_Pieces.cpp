/*
// Author Chinmay Jog
// Id: 6171795819
// Email: cjog@usc.edu
// Date: Feb 25, 2018
// Assignment 2
// Question 3- Type Counting game
*/#include <stdio.h>#include <iostream>#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>using namespace std;

unsigned char get_pattern(unsigned char** grayint, int u, int v);
unsigned char** create_2D_array(int xSize, int ySize);int main(int argc, char *argv[]){	// Define file pointer and variables	FILE *file;	int BytesPerPixel;	int xSize = 512;
	int ySize = 512;	// Check for proper syntax	if (argc < 3){		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;		return 0;	}	// Check if image is grayscale or color	if (argc < 4){		BytesPerPixel = 1; // default is grey image	}	else {		BytesPerPixel = atoi(argv[3]);		// Check if size is specified		if (argc >= 5){			xSize = atoi(argv[4]);
			ySize = atoi(argv[5]);		}	}	// Allocate image data array	unsigned char* ImageData = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* result = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* gray = new unsigned char [xSize*ySize];

	unsigned char** piecetype = create_2D_array(xSize, ySize);
	// Read images (filename specified by first 3 arguments) into image data matrix	if (!(file=fopen(argv[1],"rb"))) {		cout << "Cannot open file: " << argv[1] <<endl;		exit(1);	}	fread(ImageData, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);	fclose(file);
	// Stitching
	for (int i = 0; i < xSize*ySize; i++)
    {
        if (ImageData[i] <= 127)
            gray[i] = 1;
        else
            gray[i] = 0;
    }

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            piecetype[i][j] = gray[i*xSize+j];
        }
    }

    unsigned char cornertype[2] = {0b00001110, 0b10000011};
    unsigned char ippattern = 0b00000000;
    int rightcorners [16][2] = {0};
    int leftcorners [16][2] = {0};
    int countright = 0;
    int countleft = 0;
    //detecting corners
    for (int i = 1; i < ySize-1; i++)
    {
        for (int j = 1; j < xSize-1; j++)
        {
            if (piecetype[i][j] == 1)
            {
                ippattern = get_pattern(piecetype, i, j);
                if (ippattern == cornertype[0])
                {
                    rightcorners[countright][0] = i;
                    rightcorners[countright][1] = j;
                    countright++;
                }
                if (ippattern == cornertype[1])
                {
                    leftcorners[countleft][0] = i;
                    leftcorners[countleft][1] = j;
                    countleft++;
                }
            }
        }
    }

    // Extracting head, edges and holes
    int length = rightcorners[0][1] - leftcorners[0][1];
    int patterns[16][4] = {0};
    for (int k = 0; k < 16; k++)
    {
        // right edge
        int i = rightcorners[k][0];
        int j = rightcorners[k][1];
        if (piecetype[i+(length/2)][j] == 0)
            patterns[k][0] = 0;
        else if (piecetype[i+(length/2)][j+1] == 1)
            patterns[k][0] = 1;
        else if (piecetype[i+(length/2)][j] == 1)
            patterns[k][0] = 2;

        // top edge
        i = leftcorners[k][0];
        j = leftcorners[k][1];
        if (piecetype[i][j+(length/2)] == 0)
            patterns[k][1] = 0;
        else if (piecetype[i-1][j+(length/2)] == 1)
            patterns[k][1] = 1;
        else if (piecetype[i][j+(length/2)] == 1)
            patterns[k][1] = 2;

        // left edge
        i = leftcorners[k][0];
        j = leftcorners[k][1];
        if (piecetype[i+(length/2)][j] == 0)
            patterns[k][2] = 0;
        else if (piecetype[i+(length/2)][j-1] == 1)
            patterns[k][2] = 1;
        else if (piecetype[i+(length/2)][j] == 1)
            patterns[k][2] = 2;

        // bottom edge
        i = leftcorners[k][0] + length;
        j = leftcorners[k][1];
        if (piecetype[i][j+(length/2)] == 0)
            patterns[k][3] = 0;
        else if (piecetype[i+1][j+(length/2)] == 1)
            patterns[k][3] = 1;
        else if (piecetype[i][j+(length/2)] == 1)
            patterns[k][3] = 2;
    }
    for (int i = 0; i < 16; i++)
    {
        cout << " piece " << i << " is " << patterns[i][0] << " " << patterns[i][1] << " " << patterns[i][2] << " " << patterns[i][3] << endl;
    }
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;

    int unique_type[16] = {0};
    cout << "starting comparison " << endl;
    cout << " " << endl;
    int flag_count = 16;
    for (int i = 0; i < 15; i++)
    {
        for (int j = i+1; j < 16; j++)
        {
            vector<int> one;
            vector<int> two;
            vector<int> twor;
            int flag = 0;
            for (int k = 0; k < 4; k++)
            {
                one.push_back(patterns[i][k]);
                two.push_back(patterns[j][k]);
            }
            while (flag == 0)
            {
                if (one[0] == two[0] && one[1] == two[1] && one[2] == two[2] && one[3] == two[3])
                {
                    flag = 1;
                    if (unique_type[i] == 0 || unique_type[j] == 0)
                    {
                        unique_type[i] = 1;
                        unique_type[j] = 1;
                        flag_count--;
                    }
                    cout << i+1 << " " << j+1 << " rotated 0 times are same " << endl;
                    break;
                }
                rotate(two.begin(),two.begin()+1,two.end());
                if (one[0] == two[0] && one[1] == two[1] && one[2] == two[2] && one[3] == two[3])
                {
                    flag = 1;
                    if (unique_type[i] == 0 || unique_type[j] == 0)
                    {
                        unique_type[i] = 1;
                        unique_type[j] = 1;
                        flag_count--;
                    }
                    cout << i+1 << " " << j+1 << " rotated 1 times are same " << endl;
                    break;
                }
                rotate(two.begin(),two.begin()+1,two.end());
                if (one[0] == two[0] && one[1] == two[1] && one[2] == two[2] && one[3] == two[3])
                {
                    flag = 1;
                    if (unique_type[i] == 0 || unique_type[j] == 0)
                    {
                        unique_type[i] = 1;
                        unique_type[j] = 1;
                        flag_count--;
                    }
                    cout << i+1 << " " << j+1 << " rotated 2 times are same " << endl;
                    break;
                }
                rotate(two.begin(),two.begin()+1,two.end());
                if (one[0] == two[0] && one[1] == two[1] && one[2] == two[2] && one[3] == two[3])
                {
                    flag = 1;
                    if (unique_type[i] == 0 || unique_type[j] == 0)
                    {
                        unique_type[i] = 1;
                        unique_type[j] = 1;
                        flag_count--;
                    }
                    cout << i+1 << " " << j+1 << " rotated 3 times are same " << endl;
                    break;
                }
                else
                {
                    rotate(two.begin(),two.begin()+1,two.end());
                    twor.push_back(two[2]); twor.push_back(two[1]); twor.push_back(two[0]); twor.push_back(two[3]);
                    if (one[0] == twor[0] && one[1] == twor[1] && one[2] == twor[2] && one[3] == twor[3])
                    {
                        flag = 1;
                        if (unique_type[i] == 0 || unique_type[j] == 0)
                        {
                            unique_type[i] = 1;
                            unique_type[j] = 1;
                            flag_count--;
                        }
                        cout << i+1 << " " << j+1 << " reflected & rotated 0 times are same " << endl;
                        break;
                    }
                    rotate(twor.begin(),twor.begin()+1,twor.end());
                    if (one[0] == twor[0] && one[1] == twor[1] && one[2] == twor[2] && one[3] == twor[3])
                    {
                        flag = 1;
                        if (unique_type[i] == 0 || unique_type[j] == 0)
                        {
                            unique_type[i] = 1;
                            unique_type[j] = 1;
                            flag_count--;
                        }
                        cout << i+1 << " " << j+1 << " reflected & rotated 1 times are same " << endl;
                        break;
                    }
                    rotate(twor.begin(),twor.begin()+1,twor.end());
                    if (one[0] == twor[0] && one[1] == twor[1] && one[2] == twor[2] && one[3] == twor[3])
                    {
                        flag = 1;
                        if (unique_type[i] == 0 || unique_type[j] == 0)
                        {
                            unique_type[i] = 1;
                            unique_type[j] = 1;
                            flag_count--;
                        }
                        cout << i+1 << " " << j+1 << " reflected & rotated 2 times are same " << endl;
                        break;
                    }
                    rotate(twor.begin(),twor.begin()+1,twor.end());
                    if (one[0] == twor[0] && one[1] == twor[1] && one[2] == twor[2] && one[3] == twor[3])
                    {
                        flag = 1;
                        if (unique_type[i] == 0 || unique_type[j] == 0)
                        {
                            unique_type[i] = 1;
                            unique_type[j] = 1;
                            flag_count--;
                        }
                        cout << i+1 << " " << j+1 << " reflected & rotated 3 times are same " << endl;
                        break;
                    }
                    else
                    {
                        flag = 1;
                        //flag_count++;
                    }
                }
            }
        }
    }
    cout << " " << endl;
    cout << "Number of unique pieces is " << flag_count << endl;

	// Write image data (filename specified by second argument) from image data matrix//	if (!(file=fopen(argv[2],"wb"))) {//		cout << "Cannot open file: " << argv[2] << endl;//		exit(1);//	}//	fwrite(result, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);//	fclose(file);

	delete[] ImageData;
	delete[] gray;
	delete[] result;
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
