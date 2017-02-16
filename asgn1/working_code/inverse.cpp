/*

	CSCI 3280, Introduction to Multimedia Systems
	
	Assignment 01 Skeleton

	inverse.cpp
	
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
	int 	w, h;
	
	//	Open ASCII Art file ( command line: argv[1] )
	FILE *fin = fopen( argv[1], "r" );
	fscanf( fin, "%d %d", &w, &h );
		
	//	Create a Bitmap object accordingly
	Bitmap myBmp( w, h );

	//
	//	Your code goes here ....
	//
	//	Advice:
	//	Use Bitmap.setColor(x,y,R,G,B) for setting color of pixel at (x,y)
	//	Use fgetc()
	//

	// try to print the text file to see the structure of it 
	int x_pos, y_pos;
	char pos_color;
	unsigned char red, green, blue;
	// printing
	for (y_pos= 0; y_pos <= h-1; y_pos++) {
		//note that the \n position should be checked
		pos_color = fgetc(fin);
		for (x_pos = 0; x_pos <= w-1 ; x_pos++) {
			pos_color = fgetc(fin);
			int scale_num = 0;
			for (scale_num = 0; scale_num <= 15; scale_num++) {
				if (pos_color == shades[scale_num]) {
					// use 17 because :
					// at dark : 0, then from 0 to 255, add 17 at a time 
					// 17 * 15 = 255, therefore, can cover the whole range
					scale_num *= 17;
					myBmp.setColor(x_pos, y_pos, (unsigned char)scale_num, (unsigned char)scale_num, (unsigned char)scale_num);
					break;
				}
			}
		}
	}

	//	Save Bitmap file ( command line: argv[2] )	
	myBmp.save( argv[2] );
	fclose(fin);
		
	return 0;

} 
