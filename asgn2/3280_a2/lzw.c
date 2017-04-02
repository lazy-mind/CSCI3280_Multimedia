/*
* CSCI3280 Introduction to Multimedia Systems
*
* --- Declaration ---
*
* I declare that the assignment here submitted is original except for source
* material explicitly acknowledged. I also acknowledge that I am aware of
* University policy and regulations on honesty in academic work, and of the
* disciplinary guidelines and procedures applicable to breaches of such policy
* and regulations, as contained in the website
* http://www.cuhk.edu.hk/policy/academichonesty/
*
* Assignment 2
* Name : Mao Yuxuan
* Student ID : 1155062018
* Email Addr : yxmao5@cse.cuhk.edu.hk
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define CODE_SIZE  12
#define DICT_SIZE 4096
#define EOF_CODE 4095
#define DEBUG 0



// the code dictionary is an array, it is slow in compress, but fast in decompress

int COUNTER;

typedef struct Dict_elements{
	char single_c;
	struct Dict_elements* prefix;
}Dict_element;

typedef struct Dicts{
	Dict_element* char_str;
}Dict;

Dict CodeDict[DICT_SIZE];



void init_dict(Dict* );
void print_dict(Dict* ,int);
unsigned int findcode(Dict_element* , char , Dict* ,int);
unsigned int compare_p(Dict_element* , Dict_element* );
Dict_element* add_p(Dict_element* , char );
Dict_element* make_p(Dict_element* , char );
void addNewEntry(Dict_element*  ,char , Dict* );
void addStringEntry(Dict_element*  ,char , unsigned int , Dict* );
Dict_element* getstring(unsigned int , Dict* );
Dict_element* first_char(Dict_element* );
void write_file(Dict_element* , FILE* );
void print_element(Dict_element* );


//
//	** DO NOT CHANGE **
//
//	readArchiveHeader() - read the fileheader from a LZW archive
//	for 'Decompression' use
//
int readArchiveHeader(FILE *inLZW, char **filenames, int *numFiles)
{

	int numChars = 0;	// number of characters read from header
	char c, lastc = 0;

	//	Find out number of files in the LZW archive by checking number of '\n'
	//
	*numFiles = 0;

	while ((c = fgetc(inLZW)) != EOF) {
		numChars++;
		if (c == '\n') {
			if (lastc == c) break;	//	Double '\n' located
			(*numFiles)++;			//	Each '\n' found represents one compressed file included
		}
		lastc = c;
	}

	//	Premature EOF
	//
	if (c == EOF) {
		/* problem .... file may have corrupted*/
		*numFiles = 0;
		return 0;
	}

	//	Allocate memory and populate 'outFilenames'
	//
	*filenames = (char *)malloc(sizeof(char)*numChars);
	fseek(inLZW, 0, SEEK_SET);
	fread((*filenames), 1, (size_t)numChars, inLZW);

	return 1;

}


//
//	** DO NOT CHANGE **
//
//	writeArchiveHeader() - write out the fileheader of a LZW archive
//	for 'Compression' use
//
void writeArchiveHeader(FILE *outLZW, char **fileNames, int numFiles)
{

	for (int i = 0; i < numFiles; i++) fprintf(outLZW, "%s\n", fileNames[i]);
	fputc('\n', outLZW);	//	An extra \n to mark the end of header

}


//
//	** DO NOT CHANGE **
//
//	readCode() - read one 12-bit code from the LZW archive
//	for 'Decompression' use
//
unsigned int readCode(FILE *input, unsigned int code_size)
{
	unsigned int return_value;
	static int input_bit_count = 0;
	static unsigned long input_bit_buffer = 0L;

	/* The code file is treated as an input bit-stream. Each     */
	/*   character read is stored in input_bit_buffer, which     */
	/*   is 32-bit wide.                                         */

	/* input_bit_count stores the no. of bits left in the buffer */

	while (input_bit_count <= 24) {
		input_bit_buffer |= (unsigned long)getc(input) << (24 - input_bit_count);
		input_bit_count += 8;
	}

	return_value = input_bit_buffer >> (32 - code_size);
	input_bit_buffer <<= code_size;
	input_bit_count -= code_size;

	return(return_value);
}


//
//	** DO NOT CHANGE **
//
//	writeCode() - write one 12-bit code to the LZW archive
//	for 'Compression' use
//
void writeCode(FILE *outLZW, unsigned int code, unsigned int code_size)
{
	static int output_bit_count = 0;
	static unsigned long output_bit_buffer = 0L;

	/* Each output code is first stored in output_bit_buffer,    */
	/*   which is 32-bit wide. Content in output_bit_buffer is   */
	/*   written to the output file in bytes.                    */

	/* output_bit_count stores the no. of bits left              */

	output_bit_buffer |= (unsigned long)code << (32 - code_size - output_bit_count);
	output_bit_count += code_size;

	while (output_bit_count >= 8) {
		putc(output_bit_buffer >> 24, outLZW);
		output_bit_buffer <<= 8;
		output_bit_count -= 8;
	}

	/* only < 8 bits left in the buffer                          */

}


// initialized the dictionary
void init_dict(Dict* CodeDict){
	int i=0;
	for(i=0;i<DICT_SIZE;i++){
		CodeDict[i].char_str = NULL;
	}
	for(i=0;i<256;i++){
		Dict_element* input=NULL;
		input = make_p(input,(char)i);
		CodeDict[i].char_str = (Dict_element*)malloc(sizeof(Dict_element));
		CodeDict[i].char_str = input;
	}
	return;
}

// print the whole dictionary
void print_dict(Dict* CodeDict, int start){
	int i=0;
	for(i=start;i<DICT_SIZE;i++){
		if(CodeDict[i].char_str==NULL){break;}
		printf("%d : ",i);
		print_element(CodeDict[i].char_str);
		printf("\n");
	}
	return;
}

// print dictionary element
void print_element(Dict_element* p){
	if(p == NULL){return;}
	else if(p->prefix != NULL){print_element(p->prefix);}
	printf("%c", p->single_c);
	return ;
}

// find the compressed code for a dict element
unsigned int findcode(Dict_element* p, char c, Dict* CodeDict, int flag){
	if(flag == 0){
		Dict_element* new = NULL;
		new = p;
		new = add_p(new,c);

		int i=0;
		for(i=0;i< DICT_SIZE;i++){
			if(CodeDict[i].char_str==NULL){return -1;}
			else if(compare_p(new,CodeDict[i].char_str)==0){return i;}
		}
		return -1;
	}
	else{
		int i=0;
		for(i=0;i< DICT_SIZE;i++){
			if(CodeDict[i].char_str==NULL){return -1;}
			else if(compare_p(p,CodeDict[i].char_str)==0){return i;}
		}
		return -1;
	}
}

// compare whether 2 dict element is the same
unsigned int compare_p(Dict_element* a, Dict_element* b){
	if (a==NULL && b==NULL){return 0;}
	else if (a->prefix == NULL && b->prefix != NULL){return -1; }
	else if (a->prefix != NULL && b->prefix == NULL){return 1; }
	else if (a->single_c == b->single_c){
		return compare_p(a->prefix,b->prefix);
	}
	else{
		return -1;
	}
}

// add a character to the dict element
Dict_element* add_p(Dict_element* p, char c){
	Dict_element* new = (Dict_element*)malloc(sizeof(*new));
	new->prefix = p;
	new->single_c = c;
	return new;
}

// make dict element equal to a single character
Dict_element* make_p(Dict_element* p, char c){
	Dict_element* new = (Dict_element*)malloc(sizeof(*new));
	new->prefix = NULL;
	new->single_c = c;
	return new;
}

// add a new compress rule to the dictionary
void addNewEntry(Dict_element* p ,char c, Dict* CodeDict){
	Dict_element* new = NULL;
	new = p;
	new = add_p(new,c);

	CodeDict[COUNTER].char_str=(Dict_element*)malloc(sizeof(Dict_element));
	CodeDict[COUNTER].char_str=new;
	//printf("added code: %d\n",COUNTER);

	COUNTER++;
	if(COUNTER == EOF_CODE){
		COUNTER = 256;
		init_dict(CodeDict);
	}

	// int i=0;
	// for(i=0;i< DICT_SIZE;i++){
	// 	if(CodeDict[i].char_str==NULL){
	// 		CodeDict[i].char_str=(Dict_element*)malloc(sizeof(Dict_element));
	// 		CodeDict[i].char_str=new;
	// 		break;
	// 	}
	// }
	return;
}

// no longer needed
void addStringEntry(Dict_element* p ,char c, unsigned int CodeNum, Dict* CodeDict){
	Dict_element* new = NULL;
	new = p;
	new = add_p(new,c);
	CodeDict[CodeNum].char_str=(Dict_element*)malloc(sizeof(Dict_element));
	CodeDict[CodeNum].char_str=new;
}

// from the compress code to the string
Dict_element* getstring(unsigned int compressed_code, Dict* CodeDict){
	Dict_element* new = NULL;
	if(compressed_code >= EOF_CODE){return new;}
	if(CodeDict[compressed_code].char_str == NULL){return new;}
	return (CodeDict[compressed_code].char_str);
}

// write to the file
void write_file(Dict_element* p, FILE* output){
	if(p == NULL){return;}
	else if(p->prefix != NULL){write_file(p->prefix,output);}
	fprintf(output, "%c", p->single_c);
	return ;
}

// get the first character of a dict element
Dict_element* first_char(Dict_element* p){
	if(p==NULL){return p;}
	if(p->prefix == NULL){return p;}
	else {return first_char(p->prefix);}
}

// print a dict element
void print_dict_elem(Dict_element* p){
	if(p == NULL){return;}
	else if(p->prefix != NULL){print_dict_elem(p->prefix);}
	printf("%c",p->single_c);
	return ;
}



//
//	compress() - to compress one input file
//
void compress(FILE *inFile, FILE *outLZW)
{
	//	Your Code here
	// 1. Initialize the dictionary  CodeDict  with the first 256 ASCII codes ( i.e. 0-255 ). 
	init_dict(CodeDict);
	COUNTER = 256;

	// 2. Let  P  = NULL.
	int is_end;
	char C;
	Dict_element* P = NULL;
	unsigned int compressed_code=0;
	unsigned int dictionary_code=0;

	int i=0;

	// 3. While there are still characters to be read from input file:
	//   	A. Get a new character  C  from the input file.
	while((is_end = fgetc(inFile))!=EOF){
		C = (char)is_end;
	// 	    B. Search for the string  <P,C>  in  CodeDict :
		dictionary_code = findcode(P, C, CodeDict, 0); // 3B

		if(dictionary_code != -1){ 				// i. if  FOUND :
			compressed_code = dictionary_code;  // 3B i  (1)
			P = add_p(P,C);						// 3B i  (2) 
		}
		else{									//	ii. if  NOT FOUND:
			compressed_code = findcode(P, C, CodeDict, 1);
			//if(DEBUG){printf("code is written: %d\n",compressed_code);}
			writeCode(outLZW, compressed_code, CODE_SIZE);// 3B ii (1)
			addNewEntry(P, C, CodeDict);				  // 3B ii (2)
			P = make_p(P,C);							  // 3B ii (3)
		}
	}
	
	// output the code that correspond to P : 4 
	if(compressed_code!=0){
		compressed_code = findcode(P, C, CodeDict, 1);
		writeCode(outLZW, compressed_code, CODE_SIZE);//write the compressed code to the file
		if(DEBUG){printf("code is written: %d\n",compressed_code);}
	}
	writeCode(outLZW, EOF_CODE , CODE_SIZE);//add the EOF to the end of the compressed file
	if(DEBUG){printf("EOF is written: %d\n",EOF_CODE);}
}


//
//	decompress() - to decompress one file from the archive
//
void decompress(FILE *inLZW, FILE *outFile)
{
	//	Your Code here
	COUNTER = 256;

	// 1. Initialize the dictionary  CodeDict  with the first 256 ASCII codes ( i.e. 0-255 ). 
	init_dict(CodeDict);

	// 2. Let  PW  = first code read from the archive.
	unsigned int PW = readCode(inLZW, CODE_SIZE);
	if(DEBUG){printf("1. code is read: %d\n",PW);}
	unsigned int CW = 0;

	// 3. Let  C  =  StrDict [ PW ] and output  C .
	Dict_element* C = getstring(PW, CodeDict);
	write_file(C, outFile);

	int i=0;

	// 4. While there are still codes to be read from the archive:
	while((CW = readCode(inLZW, CODE_SIZE)) != EOF_CODE){// A. Get a new code  CW  from the archive.

		if(DEBUG){printf("2. code is read: %d\n",CW);}
		Dict_element* S = getstring(CW, CodeDict);  	 // B. Search for the code  CW  in  StrDict :
		if(S != NULL){  // i. if  FOUND :
			C = first_char(S);
			S = getstring(CW, CodeDict);
		}
		else{			// ii. if  NOT FOUND:
			S = getstring(PW,CodeDict);
			C = first_char(S);
			S = add_p(S,C->single_c);

		}
		write_file(S, outFile);	 							// C. Output  S
		Dict_element* P = getstring(PW, CodeDict);  		// D. Let  P  =  StrDict[PW]
		addNewEntry(P, C->single_c, CodeDict); 			    // F. Add a new entry  <P,C>  to  StrDict
		PW = CW;					 						// G. Let  PW  =  CW .
	}

	if(DEBUG){printf("EOF is read: %d\n",CW);}
}


int main(int argc, char **argv)
{
    char	**input_file_names;    
	char	*output_file_names;
	int		numFiles;
	FILE	*lzwFile;
	int		printusage = 0;

    if (argc >= 3) {

		if ( strcmp(argv[1], "-c") == 0) {		//	Compression

			lzwFile = fopen(argv[2] ,"wb");
        
			/* write the file header */
			input_file_names = argv + 3;
			numFiles = argc - 3;
			writeArchiveHeader(lzwFile, input_file_names, numFiles);
        	        	
			/* ADD CODES HERE */
			//printf("file nubmer: %d\n",numFiles);
			int i =0;
			while(i<numFiles){
				FILE* inputFile = fopen(input_file_names[i],"r");
				compress(inputFile,lzwFile);
				if(DEBUG){printf("compress success\n");}
				fclose(inputFile);
				i++;
			}
			writeCode(lzwFile,EOF_CODE,CODE_SIZE);
			fclose(lzwFile);


			// FILE* check = fopen(argv[2] ,"rb");
			// readArchiveHeader(lzwFile, &output_file_names, &numFiles);
			// unsigned int CW;
			// //int i=0;
			// while((CW = readCode(check, CODE_SIZE)) != EOF_CODE){
			// 	if(DEBUG){printf("CW %d\n",CW);}
			// 	//if(i==20){break;}
			// 	i++;
			// }       
			// if(DEBUG){printf("CW %d\n",CW);} 	

		} else if ( strcmp(argv[1],"-d") == 0) {	//	Decompression

			lzwFile = fopen(argv[2] ,"rb");
			
			/* read the file header */
			numFiles = 0;			
			readArchiveHeader(lzwFile, &output_file_names, &numFiles);

			/* ADD CODES HERE */
			int i=0;
			char *seperate_file;
			seperate_file = strtok(output_file_names, "\n");
			while(i<numFiles){
				FILE* fd = fopen(seperate_file,"w");
				decompress(lzwFile,fd);
				if(DEBUG){printf("decompress success\n");}
				fclose(fd);
				i++;
				if (i >= numFiles) break;
                seperate_file = strtok(NULL, "\n");
			}

			fclose(lzwFile);		
			free(output_file_names);

		} else printusage = 1;

    } else printusage = 1;

	if (printusage) printf("Usage: %s -<c/d> <lzw filename> <list of files>\n", argv[0]);
 	
	return 0;
}
