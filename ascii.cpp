/*

	CSCI 3280, Introduction to Multimedia Systems
	
	Assignment 01 Skeleton

	ascii.cpp
	
*/

#include "stdio.h"
#include "malloc.h"
#include "memory.h"
#include "math.h"
#include "bmp.h"

#define MAX_SHADES 16


//
//	***** PUT ALL YOUR CODE INSIDE main() *****
//
int main( int argc, char** argv)
{

	char shades[MAX_SHADES] = {' ', '.', ':', '=', '+', '!', '*', 'x', 'z', '%', 'Z', 'X', '$', '&', '#', '@' };
	int		w, h;

	//	Read in image data ( command line: argv[1] )
	//
	Bitmap myBmp( argv[1] );
	
	w = myBmp.getWidth();
	h = myBmp.getHeight();

	//	Prepare output TXT file ( command line: argv[2] )
	//
	FILE *fout = fopen( argv[2], "w" );
	fprintf( fout, "%d %d", w, h );
	
	//
	//	Your code goes here ....
	//	
	//	Advic:
	//	Use Bitmap.getColor(x,y,R,G,B) for getting color of pixel at (x,y)
	//	Use fputc() to read character and "\n" to end your line.
	//

	// so basically , it is the reverse process as the part 1
	// the idea is : for every pixel , we read, care the \n again
	// then for each pixel recieved, try to retrieve the color
	// use the color range to convert to gray , which is given in the specification
	// and convert to the ascii characters, using the similar scale we compute

	int x_pos, y_pos;
	double grey_num;
	unsigned char red, green, blue;
	for (y_pos = 0; y_pos <= h - 1; y_pos++) {
		//handle the \n place
		fputc('\n',fout);
		for (x_pos = 0; x_pos <= w - 1; x_pos++) {
			// for each pixel position
			// get the color and do the convert
			myBmp.getColor(x_pos, y_pos, red, green, blue);
			grey_num = 0.299*(double)red + 0.587*(double)green + 0.114*(double)blue;
			grey_num /= 17;
			// convert to ascii characters
			int scale_num = 0;
			scale_num = (int)grey_num;
			if ((grey_num - (double)scale_num) > (1 + (double)scale_num - grey_num)) { scale_num++; }
			// i think it is a bit unreasonable here
			//scale_num /= 16;
			fputc(shades[scale_num], fout);
		}
	}

	fclose(fout);

	return 0;

} 
