#include <cstdlib>
#include <iostream>
#include "stdafx.h"

using namespace std;

extern int *size;

void *get_ColorNr( char *buff );
void *search_i( char *buff,		 int p1 )
{
	unsigned nrpoints ;
	unsigned start_of_binary;
	unsigned XYZ[3] ;
	int read_f;

	FILE *fi ;
	fi = fopen( "C:\\Research_3DStitch\\Software\\Allign\\pict0_3Dcol.bin", "rb" ) ;

	// find out how many points we have... (its in binary)
	fread( &nrpoints, sizeof(unsigned), 1, fi ) ;

	// find out where we are in the file, so we can use this in later loops, etc...
	// should always be 4, but you might add other data to the start of your file...
	start_of_binary = ftell( fi ) ;

	// now jump right into that point for us...
	// we assume here XYZ x sizeof(float) = 12bytes

	read_f = fseek( fi, start_of_binary + p1*3*sizeof(unsigned), SEEK_SET ) ;

	// Get our 3 floating values.
	fread( XYZ, sizeof(unsigned), 3, fi ) ; 

	return( &XYZ );

	fclose(fi) ;
}

// search the 3D file (it has floats)
void *search_f( int p1 )
{
	unsigned nrpoints ;
	unsigned start_of_binary;
	float XYZ[3] ;
	int read_f;

	FILE *fi ;
	fi = fopen( "C:\\Research_3DStitch\\Software\\Allign\\pict0_3D.bin", "rb" ) ;

	// find out how many points we have... (its in binary)
	fread( &nrpoints, sizeof(unsigned), 1, fi ) ;

	// find out where we are in the file, so we can use this in later loops, etc...
	// should always be 4, but you might add other data to the start of your file...
	start_of_binary = ftell( fi ) ;

	// now jump right into that point for us...
	// we assume here XYZ x sizeof(float) = 12bytes

	read_f = fseek( fi, start_of_binary + p1*3*sizeof(float), SEEK_SET ) ;

	// Get our 3 floating values.
	fread( XYZ, sizeof(float), 3, fi ) ; 

	return( &XYZ );

	fclose(fi) ;
}

