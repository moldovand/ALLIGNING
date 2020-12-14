#include <iostream>
#include <fstream>
#include "AbsOrient.h"
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <string>
#include <time.h>

using namespace std;

extern Frame *Transform;
extern int transf_count;
extern int jump;

void ChangeRef_AllStepsBack(int nr)
{
	FILE *fp;
	char *buff;
	long int i, numPoints;
	int new_count;
	float xyz[3];
	Point3D finalT;//final transformed points
	std::vector<Point3D> finalPtrans;

//Apply backwards the transformation to conect with previous C.S
	if ((nr != 0) && (transf_count >= 1) && (jump != 1) ){
		//Read the set of 3D Range Data to be transformed
		buff = new char [100];
		sprintf(buff, "C:\\Model.Teddy.Bear\\Bin\\pict%d_3Dtrans_temp.bin", nr+1);
		fp = fopen(buff, "rb");
		delete [] buff;

		fread( &numPoints, sizeof(long int), 1, fp ) ;
		while(!feof(fp)){
			fread( xyz, sizeof(float), 3, fp ) ; 
			finalT[0] = xyz[0];
			finalT[1] = xyz[1];
			finalT[2] = xyz[2];
			//find the transformation of previous step
			//new_count keeps the track
			//new_count is initialized with previous transformation counter
			//new_count = transf_count - 1;
			//--------------------------
			if(jump == 0){
				
				new_count = transf_count - 1;

				while(new_count >= 0){
					//apply the transformation on the set of 3d Range Data
					Transform[new_count].apply(finalT);
					//new_count is moved to the next transformation
					new_count = new_count - 1;
					//the process repets until the starting position is reached
				}
				//transformed 3D range data is pushed back on finalT vector
				finalPtrans.push_back(finalT);
			}
			//--------------------------

			if(jump > 1){
				
				new_count = transf_count - 1;

				while(new_count >= 7){
					//apply the transformation on the set of 3d Range Data
					Transform[new_count].apply(finalT);
					//new_count is moved to the next transformation
					new_count = new_count - 1;
					//the process repets until the starting position is reached
				}
				//transformed 3D range data is pushed back on finalT vector
				finalPtrans.push_back(finalT);
			}


		}

		fclose(fp);

		//Save it
		buff = new char [100];
		sprintf(buff, "C:\\Model.Teddy.Bear\\Bin\\pict%d_3Dtrans.bin", nr+1);
		fp = fopen(buff, "wb");
		delete [] buff;

		fwrite( &numPoints, sizeof(long int), 1, fp  ) ;
		for (i = 0; i < numPoints; i++){
			xyz[0] = finalPtrans[i][0];
			xyz[1] = finalPtrans[i][1];
			xyz[2] = finalPtrans[i][2];
			fwrite(xyz, sizeof(float), 3, fp);
		}

		fclose(fp);
	}
}