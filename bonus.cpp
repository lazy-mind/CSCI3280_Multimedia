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
    
	char shades[MAX_SHADES] = { 'O', '.', ':', '=', '+', '!', '*', 'x', 'z', '%', 'Z', 'X', '$', '&', '#', '@' };
	char* color_set[MAX_SHADES] = { "white","blue","pink","yellow","green","black","silver","orange" };
    int		w, h;
	// for handling the space situation
	char* space= "&nbsp;";
    
    //	Read in image data ( command line: argv[1] )
    //
    Bitmap myBmp( argv[1] );
    
    w = myBmp.getWidth();
    h = myBmp.getHeight();
    
    //	Prepare output TXT file ( command line: argv[2] )
    //
	// the user should specify a file with .html as the extension name
    FILE *fout = fopen( argv[2], "w" );

	// the header of the html page
    fprintf( fout, "<!DOCTYPE thml>\n<html>\n<head>\n</head>\n" );
	// use a Courier New front style, which makes all characters take up same space:Courier New
	fprintf(fout, "<style>\nbody\n{font-family:Courier New;\n}\np.small{line-height:0.1em}\n</style>\n");
	fprintf(fout, "<body>\n");
    
    
    //
    //	Your code goes here ....
    //
    //	Advic:
    //	Use Bitmap.getColor(x,y,R,G,B) for getting color of pixel at (x,y)
    //	Use fputc() to read character and "\n" to end your line.
    //
    
    int x_pos, y_pos;
    double grey_num;
    unsigned char red, green, blue;
    for (y_pos = 0; y_pos <= h - 1; y_pos++) {
        //handle the \n place
		// to sepreate different lines
		fprintf(fout,"<p class=\"small\">\n");
        for (x_pos = 0; x_pos <= w - 1; x_pos++) {
            myBmp.getColor(x_pos, y_pos, red, green, blue); // get the color, useful in setting the html word color
            grey_num = 0.299*(double)red + 0.587*(double)green + 0.114*(double)blue; // counting the grey number, still a nesscessarity for identify the ascii character
            int scale_num = 0;
            scale_num = (int)grey_num;
            scale_num /= 16; // have converted to the index number
			if (scale_num != 0) {
				fprintf(fout, "<a style=\"color:rgb(%d,%d,%d);\">%c", (int)red, (int)green, (int)blue, shades[scale_num]);
			}
			else { fprintf(fout, "<a style=\"color:rgb(%d,%d,%d);\">%s", (int)red, (int)green, (int)blue, space); }
			// otherwise the space will be ignored and the graph is distored
        }
		fprintf(fout,"</p>\n");
    }
	// the ending of the html
	fprintf(fout, "</body>");
	fprintf(fout, "</html>\n");
    
    
    fclose(fout);
    
    return 0;
    
} 



// inline style (the one I am using)
/*

<html>

<head>
</head>

<body>
<h1 style = "color:#36C;"> This is inline CSS </h1>
</body>

</html>

*/

//another usage example from the text part of CSS
//how to seperate different lines
/*
<!DOCTYPE html>
<html>
<head>
<style>
h1 {
text-align: center;
}

h2 {
text-align: left;
}

h3 {
text-align: right;
}
</style>
</head>
<body>

<h1>Heading 1 (center)</h1>
<h2>Heading 2 (left)</h2>
<h3>Heading 3 (right)</h3>

<p>The three headings above are aligned center,</p>
<p>left and right.</p>

</body>
</html>

*/

//how to set the color with rgb style
/*
<!DOCTYPE html>
<html>
<body>

<h2>RGB Color Examples</h2>

<h2 style="background-color:rgb(255, 0, 0)">
Background-color set by using rgb(255, 0, 0)
</h2>

<h2 style="background-color:rgb(0, 255, 0)">
Background-color set by using rgb(0, 255, 0)
</h2>

<h2 style="background-color:rgb(0, 0, 255)">
Background-color set by using rgb(0, 0, 255)
</h2>

<h2 style="background-color:rgb(255, 165, 0)">
Background-color set by using rgb(255, 165, 0)
</h2>

<h2 style="background-color:rgb(255, 255, 0)">
Background-color set by using rgb(255, 255, 0)
</h2>

<h2 style="background-color:rgb(0, 255, 255)">
Background-color set by using rgb(0, 255, 255)
</h2>

</body>
</html>

*/
