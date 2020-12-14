#include "AbsOrient.h"
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <iostream>
#include <string>
#include <time.h>

extern Frame *Transform;
extern int transf_count;
extern int jump;

//number of 3D points 
int num_3DPoints;

void ConvertRobust3Dtxt_Point3Dvector(int nr, std::vector<Point3D> &rigt, std::vector<Point3D> &left);

/* Convert Robust 3D matches into the initial reference system
nr encodes the counter of the files pair
transf_count encodes the starting counter for transformation */ 
void ChangeRef_Robust_AllStepsBack(int nr)
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

	if ((nr != 1) && (transf_count >= 1))
		//Apply backwards the transformation to conect with previous C.S
		for(i = 0; i < left.size(); i++)
		{
			finalT[0] = left[i][0];
			finalT[1] = left[i][1];
			finalT[2] = left[i][2];
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
	else
		for(i = 0; i < left.size(); i++)
		{
			finalT[0] = left[i][0];
			finalT[1] = left[i][1];
			finalT[2] = left[i][2];
			//find the transformation of previous step
			//new_count keeps the track
			//new_count is initialized with previous transformation counter
			new_count = transf_count;
			//apply the transformation on the set of 3d Range Data
			Transform[new_count].apply(finalT);
			//transformed 3D range data is pushed back on finalT vector
			finalPtrans.push_back(finalT);
		}

	//Save it
	buff = new char [100];
	sprintf(buff, "C:\\AllignData\\RoughApprox\\Robust3D_Origin_%d.txt", nr+1);
	fp = fopen(buff, "w");
	delete [] buff;
	//fprintf(fp, "%d\n", left.size());
	for (i = 0; i < left.size(); i++)
		fprintf(fp, "%.2f\t%.2f\t%.2f\n", finalPtrans[i][0], finalPtrans[i][1], finalPtrans[i][2]);
	fclose(fp);

	//Also save the 3D points detected in the Original C.S
	if (nr == 1){
		buff = new char [100];
		sprintf(buff, "C:\\AllignData\\RoughApprox\\Robust3D_Origin_0.txt");
		fp = fopen(buff, "w");
		delete [] buff;

		for(i = 0; i < right.size(); i++)
			fprintf(fp, "%.2f\t%.2f\t%.2f\n", right[i][0], right[i][1], right[i][2]);
		fclose(fp);
	}

}
/* Convert Robust 3D matches from xyz txt format 
	and stack it into a vector Point3D format*/
void ConvertRobust3Dtxt_Point3Dvector(int nr, std::vector<Point3D> &right, std::vector<Point3D> &left)
{
	Point3D leftPoint, rightPoint;
	float x, y, z;
	int er1, er2, er3;
	char *buff;
	FILE *fp;

	buff = new char [200];
	sprintf(buff, "C:\\Model.Teddy.Bear\\RoughApprox\\Robust_3DMatches%d-%d.txt", nr, nr+1);


	fp = fopen(buff, "r");
	while(!feof(fp))
	{
		if( (fscanf(fp, "%f", &x)<0)||(fscanf(fp, "%f", &y)<0)||(fscanf(fp, "%f", &z)<0) )
			break;
		else {
			rightPoint[0] = x;
			rightPoint[1] = y;
			rightPoint[2] = z;
			right.push_back(rightPoint);
		}

		if( (fscanf(fp, "%f", &x)<0)||(fscanf(fp, "%f", &y)<0)||(fscanf(fp, "%f", &z)<0) )
			break;
		else {
			leftPoint[0] = x;
			leftPoint[1] = y;
			leftPoint[2] = z;
			left.push_back(leftPoint); 
		}
	}
	
	fclose(fp);
	delete [] buff;

}
void ChangeRef_OneStepBack(int nr, std::vector<Point3D> &Ptrans)
{
	Point3D P;
	FILE *fp;
	float xyz[3];
	char *buff;
	int i;
	long int num_3DPoints;

	buff = new char [100];
	sprintf(buff, "C:\\Model.Teddy.Bear\\Bin\\pict%d_3D.bin", nr+1);
	fp = fopen(buff, "rb") ;

	//fscanf(fp, "%d", &numPoints);
	fread( &num_3DPoints, sizeof(long int), 1, fp ) ;
	while(!feof(fp)){
		fread( xyz, sizeof(float), 3, fp ) ; 
		P[0] = xyz[0];
		P[1] = xyz[1];
		P[2] = xyz[2];
		Transform[transf_count].apply(P);
		Ptrans.push_back(P);
	}
	fclose(fp);

	delete [] buff;

	//Save the set of transformed 3D Range Data
	buff = new char [100];
	if ((nr == 0) || (transf_count == 0) || (jump == 1))
		//in case it's the first transformation
		sprintf(buff, "C:\\Model.Teddy.Bear\\Bin\\pict%d_3Dtrans.bin", nr+1);
	else
		//otherwise save it in a temporal version
		sprintf(buff, "C:\\Model.Teddy.Bear\\Bin\\pict%d_3Dtrans_temp.bin", nr+1);

	fp = fopen(buff, "wb");

	//Write to file the transformed 3D Range Data
	fwrite( &num_3DPoints, sizeof(long int), 1, fp  ) ;

	for (i = 0; i < num_3DPoints; i++){
		xyz[0] = Ptrans[i][0];
		xyz[1] = Ptrans[i][1];
		xyz[2] = Ptrans[i][2];
		fwrite(xyz, sizeof(float), 3, fp);
	}
	fclose(fp);
	delete [] buff;
}