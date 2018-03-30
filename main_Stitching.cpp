/*
// Author Chinmay Jog
// Id: 6171795819
// Email: cjog@usc.edu
// Date: Feb 25, 2018
// Assignment 2
// Question 1- Image Stitching
*/#include <stdio.h>#include <iostream>#include <stdlib.h>
#include <math.h>using namespace std;

unsigned char projective_left(unsigned char** plane, int u, int v);
unsigned char projective_right(unsigned char** plane, int u, int v);
unsigned char** create_2D_array(int xSize, int ySize);int main(int argc, char *argv[]){	// Define file pointer and variables	FILE *file;	int BytesPerPixel;	int xSize = 512;
	int ySize = 512;	// Check for proper syntax	if (argc < 3){		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;		return 0;	}	// Check if image is grayscale or color	if (argc < 4){		BytesPerPixel = 1; // default is grey image	}	else {		BytesPerPixel = atoi(argv[4]);		// Check if size is specified		if (argc >= 5){			xSize = atoi(argv[5]);
			ySize = atoi(argv[6]);		}	}	// Allocate image data array	unsigned char* LeftImage = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* MiddleImage = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* RightImage = new unsigned char [xSize*ySize*BytesPerPixel];
	unsigned char* result = new unsigned char [(xSize+900)*(ySize+500)*BytesPerPixel];
	unsigned char* rleft = new unsigned char [xSize*ySize];
	unsigned char* gleft = new unsigned char [xSize*ySize];
	unsigned char* bleft = new unsigned char [xSize*ySize];
	unsigned char* rmid = new unsigned char [xSize*ySize];
	unsigned char* gmid = new unsigned char [xSize*ySize];
	unsigned char* bmid = new unsigned char [xSize*ySize];
	unsigned char* rright = new unsigned char [xSize*ySize];
	unsigned char* gright = new unsigned char [xSize*ySize];
	unsigned char* bright = new unsigned char [xSize*ySize];

	unsigned char** rinleft = create_2D_array(xSize, ySize);
    unsigned char** binleft = create_2D_array(xSize, ySize);
    unsigned char** ginleft = create_2D_array(xSize, ySize);

    unsigned char** routleft = create_2D_array(xSize+900, ySize+500);
    unsigned char** goutleft = create_2D_array(xSize+900, ySize+500);
    unsigned char** boutleft = create_2D_array(xSize+900, ySize+500);
    unsigned char** rinmid = create_2D_array(xSize, ySize);
    unsigned char** ginmid = create_2D_array(xSize, ySize);
    unsigned char** binmid = create_2D_array(xSize, ySize);
    unsigned char** rinright = create_2D_array(xSize, ySize);
    unsigned char** ginright = create_2D_array(xSize, ySize);
    unsigned char** binright = create_2D_array(xSize, ySize);

    unsigned char** routright = create_2D_array(xSize+900, ySize+500);
    unsigned char** goutright = create_2D_array(xSize+900, ySize+500);
    unsigned char** boutright = create_2D_array(xSize+900, ySize+500);	// Read images (filename specified by first 3 arguments) into image data matrix	if (!(file=fopen(argv[1],"rb"))) {		cout << "Cannot open file: " << argv[1] <<endl;		exit(1);	}	fread(LeftImage, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);	fclose(file);

	if (!(file=fopen(argv[2],"rb"))) {		cout << "Cannot open file: " << argv[2] <<endl;		exit(1);	}	fread(MiddleImage, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);	fclose(file);

	if (!(file=fopen(argv[3],"rb"))) {		cout << "Cannot open file: " << argv[3] <<endl;		exit(1);	}	fread(RightImage, sizeof(unsigned char), xSize*ySize*BytesPerPixel, file);	fclose(file);	// Stitching

	int count1 = 0;
	for (int i = 0; i < xSize*ySize*BytesPerPixel; i+=3)
    {
        rleft[count1] = LeftImage[i];
        gleft[count1] = LeftImage[i+1];
        bleft[count1] = LeftImage[i+2];

        rmid[count1] = MiddleImage[i];
        gmid[count1] = MiddleImage[i+1];
        bmid[count1] = MiddleImage[i+2];

        rright[count1] = RightImage[i];
        gright[count1] = RightImage[i+1];
        bright[count1] = RightImage[i+2];
        count1++;
    }

    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            rinleft[i][j] = rleft[i*xSize + j];
            ginleft[i][j] = gleft[i*xSize + j];
            binleft[i][j] = bleft[i*xSize + j];

            rinmid[i][j] = rmid[i*xSize + j];
            ginmid[i][j] = gmid[i*xSize + j];
            binmid[i][j] = bmid[i*xSize + j];

            rinright[i][j] = rright[i*xSize + j];
            ginright[i][j] = gright[i*xSize + j];
            binright[i][j] = bright[i*xSize + j];
        }
    }

    // Warping function call

//    cout << "Now stitching left and middle " << endl;
//    for (int i = 0; i < ySize; i++)
//    {
//        for (int j = 0; j < xSize; j++)
//        {
//            //cout << "i is " << i << " j is " << j << endl;
//            result[(i+312)*(xSize+1500)*3 + (j+515)*3] = rinmid[i][j];
//            result[(i+312)*(xSize+1500)*3 + (j+515)*3 + 1] = ginmid[i][j];
//            result[(i+312)*(xSize+1500)*3 + (j+515)*3 + 2] = binmid[i][j];
//        }
//    }

    cout << "now projection " << endl;
    for (int i = 0; i < ySize+500; i++)
    {
        //cout << "i is " << i << endl;
        for (int j = 0; j < xSize+900; j++)
        {
            routleft[i][j] = projective_left(rinleft, i, j);
            goutleft[i][j] = projective_left(ginleft, i, j);
            boutleft[i][j] = projective_left(binleft, i, j);
            if (result[i*(xSize+900)*3 + (j)*3] == 0)
            {
                result[i*(xSize+900)*3 + (j)*3] = routleft[i][j];
                result[i*(xSize+900)*3 + (j)*3 + 1] = goutleft[i][j];
                result[i*(xSize+900)*3 + (j)*3 + 2] = boutleft[i][j];
            }
            //result[i*(xSize+200)*3+j*3] = routleft[i][j];
        }
    }

    cout << "Now stitching left and middle " << endl;
    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            if (result[(i+312)*(xSize+900)*3 + (j+515)*3] != 0)
            {
                result[(i+312)*(xSize+900)*3 + (j+515)*3] = (result[(i+312)*(xSize+900)*3 + (j+515)*3] + rinmid[i][j])/2;
                result[(i+312)*(xSize+900)*3 + (j+515)*3 + 1] = (result[(i+312)*(xSize+900)*3 + (j+515)*3 + 1] + ginmid[i][j])/2;
                result[(i+312)*(xSize+900)*3 + (j+515)*3 + 2] = (result[(i+312)*(xSize+900)*3 + (j+515)*3 + 2] + binmid[i][j])/2;
            }
            else
            {
                result[(i+312)*(xSize+900)*3 + (j+515)*3] = rinmid[i][j];
                result[(i+312)*(xSize+900)*3 + (j+515)*3 + 1] = ginmid[i][j];
                result[(i+312)*(xSize+900)*3 + (j+515)*3 + 2] = binmid[i][j];
            }
            //cout << "i is " << i << " j is " << j << endl;
        }
    }

    cout << "now right projection and stitching " << endl;
    for (int i = 0; i < ySize+500; i++)
    {
        for (int j = 0; j < xSize+900; j++)
        {
            routright[i][j] = projective_right(rinright, i, j);
            goutright[i][j] = projective_right(ginright, i, j);
            boutright[i][j] = projective_right(binright, i, j);
            if (result[i*(xSize+900)*3 + (j)*3] == 0)
            {
                result[i*(xSize+900)*3 + (j)*3] = routright[i][j];
                result[i*(xSize+900)*3 + (j)*3 + 1] = goutright[i][j];
                result[i*(xSize+900)*3 + (j)*3 + 2] = boutright[i][j];
            }
            else
            {
                if (routright[i][j] != 0)
                    result[i*(xSize+900)*3 + (j)*3] = (result[i*(xSize+900)*3 + (j)*3] + routright[i][j])/2;
                if (goutright[i][j] != 0)
                    result[i*(xSize+900)*3 + (j)*3 + 1] = (result[i*(xSize+900)*3 + (j)*3 + 1] + goutright[i][j])/2;
                if (boutright[i][j] != 0)
                    result[i*(xSize+900)*3 + (j)*3 + 2] = (result[i*(xSize+900)*3 + (j)*3 + 2] + boutright[i][j])/2;
            }
        }
    }
    cout << "Result success " << endl;

	// Write image data (filename specified by second argument) from image data matrix	if (!(file=fopen(argv[7],"wb"))) {		cout << "Cannot open file: " << argv[7] << endl;		exit(1);	}	fwrite(result, sizeof(unsigned char), (xSize+900)*(ySize+500)*BytesPerPixel, file);	fclose(file);

	delete[] LeftImage;
	delete[] MiddleImage;
	delete[] RightImage;
	delete[] result;
	delete[] rleft;
	delete[] gleft;
	delete[] bleft;
	for (int i = 0; i < ySize; i++)
    {
        delete[] rinleft[i];
        delete[] binleft[i];
        delete[] ginleft[i];
        delete[] routleft[i];
        delete[] goutleft[i];
        delete[] boutleft[i];

        delete[] rinmid[i];
        delete[] ginmid[i];
        delete[] binmid[i];
        delete[] rinright[i];
        delete[] ginright[i];
        delete[] binright[i];
    }
	delete[] rinleft;
	delete[] binleft;
	delete[] ginleft;
	delete[] rinmid;
    delete[] ginmid;
    delete[] binmid;
    delete[] rinright;
    delete[] ginright;
    delete[] binright;
    delete[] routleft;
    delete[] goutleft;
    delete[] boutleft;
    delete[] routright;
    delete[] goutright;
    delete[] boutright;
//	delete[] rout;
//	delete[] gout;
//	delete[] bout;	return 0;}



unsigned char** create_2D_array(int xSize, int ySize)
{
    unsigned char** newArray = new unsigned char*[ySize];
    for (int i = 0; i < ySize; i++)
    {
        newArray[i] = new unsigned char[xSize];
    }
    return newArray;
}


unsigned char projective_left(unsigned char** plane, int u, int v)
{
    unsigned char output = 0;
    int left = 0;
    int top = 0;
    double delX = 0;
    double delY = 0;

    double x = (0.671641419*u - 0.170916775*v - 104.239976)/(0.0000141046254*u + -0.000554889381*v + 1);
    double y = (-0.00330654332*u + 0.460786062*v + -36.5581096)/(0.0000141046254*u + -0.000554889381*v + 1);
    if (x >= 0 && x < 639 && y>= 0 && y < 479)
    {
        left = floor(x);
        top = floor(y);
        delX = x - left;
        delY = y - top;

        //cout << "now bilinear " << endl;

        output = ((1-delX)*(1-delY)*plane[left][top]) + ((delX)*(1-delY)*plane[left][top+1])
                + ((1-delX)*(delY)*plane[left+1][top]) + ((delX)*(delY)*plane[left+1][top+1]);
    }
    return output;
}

unsigned char projective_right(unsigned char** plane, int u, int v)
{
    //cout << "function entered " << endl;
    unsigned char output = 0;
    int left = 0;
    int top = 0;
    double delX = 0;
    double delY = 0;

    //double x = (1.04679568376569*u + 0.172255438495553*v + -469.954179500672)/(-2.23469834773348e-05*u + 0.000554403618039415*v + 0.605275885383875);
    //double y = (0.0210196538928225*u + 1.12691370038267*v + -824.239392638731)/(-2.23469834773348e-05*u + 0.000554403618039415*v + 0.605275885383875);
    //double x = (1.01152832158642*u + 0.198786437246042*v + -473.471412178908)/(-9.66690802237395e-05*u + 0.000639607929172788*v + 0.568407667192168);
    //double y = (0.0104684050596606*u + 1.12947597549977*v + -821.168980298455)/(-9.66690802237395e-05*u + 0.000639607929172788*v + 0.568407667192168);
    double x = (1.06476193210222*u + 0.201497622260042*v + -499.800313250537)/(-1.68472256851966e-05*u + 0.000661254908071017*v + 0.527036925843002);
    double y = (0.00855047884459119*u + 1.16468564013660*v + -845.749556553356)/(-1.68472256851966e-05*u + 0.000661254908071017*v + 0.527036925843002);

    if (x >= 0 && x < 639 && y >= 0 && y < 479)
    {
        left = floor(x);
        top = floor(y);
        delX = x - left;
        delY = y - top;

        //cout << "now bilinear " << endl;

        output = ((1-delX)*(1-delY)*plane[left][top]) + ((delX)*(1-delY)*plane[left][top+1])
                + ((1-delX)*(delY)*plane[left+1][top]) + ((delX)*(delY)*plane[left+1][top+1]);
    }
    return output;
}
