#include "protos.h"

extern float ***p;
extern float ***pc;
extern int *size;
extern int winW, winH;
extern int Nr_angle;
extern Frame *Transform;


void Get_Data(void)
{
	FILE *fp ;
	char *buff ;
	int i, j ;
	float **a ;
	float x, y, z ;
	unsigned start_of_binary ;
	unsigned fileColor_start;

	//for Coordinate transformations
	Transform = new Frame [Nr_angle];
	//ALLOCATE MEMORY FOR DOUBLE-POINTERS
	p = new float **[Nr_angle] ;

	pc = new float **[Nr_angle] ;

	size = new int [Nr_angle];

	printf("Load data ... \n");

	//READ IN THE 3D DATA FOR 10 VIEWS
	buff = new char [100];
	for (i = 0; i < Nr_angle; i++){
		
		//sprintf(buff, "C:\\AllignData\\binary\\pict%d_3D.bin", i) ;
		sprintf(buff, "C:\\Model.Teddy.Bear\\Bin\\pict%d_3D.bin", i) ;
		fp = fopen(buff, "rb") ;

		fread( &size[i], sizeof(int), 1, fp ) ;
		p[i] = new float *[size[i]];

		// find out where we are in the file
		start_of_binary = ftell( fp ) ;

		for(j = 0; j < size[i]; j ++){
			p[i][j] = new float [3];		
			fread( p[i][j], sizeof(float), 3, fp ) ; 
		}
		//------------------------
		fclose(fp);

	}
	delete [] buff;

	printf("Finished loading 3D positions! \n");

	//READ IN THE COLOR DATA
	buff = new char [100];
	for(i = 0; i < Nr_angle; i++){
		sprintf(buff, "C:\\Model.Teddy.Bear\\Bin\\pict%d_3Dcol.bin", i);
		fp = fopen(buff, "rb");

		pc[i] = new float *[size[i]];

		fread( &fileColor_start, sizeof(unsigned), 1, fp ) ;

		for(j = 0; j < size[i]; j ++){
			pc[i][j] = new float [3];
			fread( pc[i][j], sizeof(float), 3, fp ) ; 
			pc[i][j][0] = pc[i][j][0]/255;
			pc[i][j][1] = pc[i][j][1]/255;
			pc[i][j][2] = pc[i][j][2]/255;
		}
		fclose(fp);

	}
	delete [] buff;

	printf("Finished loading texture! \n");
}
