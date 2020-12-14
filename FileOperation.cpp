#include "protos.h"

using namespace std;

void ReadMatches(FILE *fp, CvPoint *p1, CvPoint *p2, int &numPoints);
int FindLine_Write3D(FILE *fp, int p1, int p2, int nr);

int FindLine_Write3D(FILE *fp, int p1, int p2, int nr)
{
	char c = '\0';
	char *buff;
	float cf;
	float x1, y1, z1;
	float x2, y2, z2;

	int i = 0;
	int j = 0;
	long int nrpoints ;
	float XYZ1[3] ;
	float XYZ2[3] ;

	//3D_all files
	FILE *fp1, *fp2;

	buff = new char [100];
	sprintf(buff, "C:\\Model.Teddy.Bear\\Bin\\pict%d_3D_all.bin", nr);
	fp1 = fopen(buff, "rb");
	delete [] buff;

	buff = new char [100];
	sprintf(buff, "C:\\Model.Teddy.Bear\\Bin\\pict%d_3D_all.bin", nr+1);
	fp2 = fopen(buff, "rb");
	delete [] buff;

	fread( &nrpoints, sizeof(float), 1, fp1 ) ;
	fseek( fp1, sizeof(long int) + p1*3*sizeof(float), SEEK_SET ) ;
	if(!feof(fp1))
		fread( XYZ1, sizeof(float), 3, fp1 ) ; 
	fclose(fp1) ;

	//do the same thing for file fp2
	fread( &nrpoints, sizeof(float), 1, fp2 ) ;
	fseek( fp2, sizeof(long int) + p2*3*sizeof(float), SEEK_SET ) ;
	if(!feof(fp2))
		fread( XYZ2, sizeof(float), 3, fp2 ) ; 
	fclose(fp2) ;

	if((XYZ1[0] != 0.0) && (XYZ1[1] != 0.0) && (XYZ1[2] != 0.0) && 
		(XYZ2[0] != 0.0) && (XYZ2[1] != 0.0) && (XYZ2[2] != 0.0)){
		fprintf(fp, "%f  %f  %f\t\t", XYZ1[0], XYZ1[1], XYZ1[2]);
		fprintf(fp, "%f  %f  %f\n", XYZ2[0], XYZ2[1], XYZ2[2]);
		//cout<<"1, ";
		return(1);
	}
	else{
		//cout<<"\n A bad one: the 3D point is corrupted,\n";
		return(0);
	}

}
void NextNumber(FILE *fp)
{
  char c = '\0';
  while (!feof(fp))
    {
      c = fgetc(fp);
      if (isdigit(c) || c == '-') break;
    }
  
  ungetc(c, fp);
}

void ReadMatches(FILE *fp, CvPoint *p1, CvPoint *p2, int &numPoints)
{
	int x, y;
	
	for (int i = 0; i < numPoints; i++){
		fscanf(fp, "%d", &x);
		p1[i].x = x;
		NextNumber(fp);
		fscanf(fp, "%d", &y);
		p1[i].y = y;
		NextNumber(fp);
		fscanf(fp, "%d", &x);
		p2[i].x = x;
		NextNumber(fp);
		fscanf(fp, "%d", &y);
		p2[i].y = y;
	}
}
