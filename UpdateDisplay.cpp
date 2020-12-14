#include "protos.h"

extern float ***p;
extern int *size;

void UpdateDisplay_RegisteredData(int nr)
{
	FILE *fp;
	char *buff;
	int i, j;
	long int file_size;
	float xyz[3];

	//READ IN THE 3D DATA FOR pict_3Dtrans.bin
	buff = new char [100];
	sprintf(buff, "C:\\Model.Teddy.Bear\\Bin\\pict%d_3Dtrans.bin", nr+1);
	fp = fopen(buff, "rb") ;

	fread( &file_size, sizeof(long int), 1, fp ) ;

	for(j = 0; j < file_size; j ++){
		fread(xyz, sizeof(float), 3, fp);
		p[nr+1][j][0] = xyz[0];
		p[nr+1][j][1] = xyz[1];
		p[nr+1][j][2] = xyz[2];
	}

	fclose(fp);
	delete [] buff;

}

void Write_RegisteredData2(void)
{
  FILE *fp;
  char *buff;
  int i, j;

//READ IN THE 3D DATA FOR 10 VIEWS
  buff = new char [100];
  for (i = 2; i < 3; i++){
	  sprintf(buff, "C:\\AllignData\\doll_img_feat\\pict%d_3Dtrans.txt", i);
	  fp = fopen(buff, "r");
	  if (! fp){
		  printf("Error open file %d \n", i);
		  exit(0);
	  }

	  fscanf(fp, "%d", &size[i]);
	  
	  for(j = 0; j < size[i]; j ++){
		  
		  fscanf(fp, "%f %f %f", &p[i][j][0], &p[i][j][1], &p[i][j][2]);
	  }

	  fclose(fp);

  }
  delete [] buff;

}
