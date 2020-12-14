#include "AbsOrient.h"
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <iostream>
#include <string>
#include <time.h>

extern Frame Transform[20];
extern int transf_count;

void ConvertRobust3Dtxt_Point3Dvector(int nr, std::vector<Point3D> &rigt, std::vector<Point3D> &left);

/* Convert Robust 3D matches into the initial reference system
nr encodes the counter of the files pair
transf_count encodes the starting counter for transformation */ 

void ChangeRef_Robust(int nr)
{
	FILE *fp;
	char *buff;
	float x, y, z;
	//Point3D leftPoint, rightPoint;
	std::vector<Point3D> left, right;
	std::vector<Point3D> finalPtrans;
	Point3D finalT;
	int i;
	int new_count;

	//Read Robust 3D Matches 
	ConvertRobust3Dtxt_Point3Dvector(nr, right, left);

	//Apply backwards the transformation to conect with previous C.S
	for(i = 0; i < right.size(); i++)
		{
			finalT[0] = right[i][0];
			finalT[1] = right[i][1];
			finalT[2] = right[i][2];
			//find the transformation of previous step
			//new_count keeps the track
			//new_count is initialized with previous transformation counter
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

	//Save it
	buff = new char [100];
	sprintf(buff, "C:\\AllignData\\doll_img_feat\\SIFT\\Robust3D_Origin_%d.txt", nr-1);
	fp = fopen(buff, "w");
	delete [] buff;
	//fprintf(fp, "%d\n", left.size());
	for (i = 0; i < right.size(); i++)
		fprintf(fp, "%.2f\t%.2f\t%.2f\n", finalPtrans[i][0], finalPtrans[i][1], finalPtrans[i][2]);
	fclose(fp);
}