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

void ConvertRobust3Dtxt_Point3Dvector(int nr, std::vector<Point3D> &rigt, std::vector<Point3D> &left);

void SaveTransform_OneStepBack(int nr);
void SaveTransform_AllStepsBack(int nr);

void ChangeRef_OneStepBack(int nr, std::vector<Point3D> &Ptrans);
void ChangeRef_Robust_AllStepsBack(int nr);
void ChangeRef_AllStepsBack(int nr);
void ChangeRef_Robust(int nr);

void AbsoluteOrientation::selfTest(int nr)//nr is actually variable step
{
	std::vector<Point3D> left;
	std::vector<Point3D> right; //robust 3D matches
	std::vector<Point3D> rightPtrans;

	/* Convert Robust 3D matches from xyz txt format 
	and stack it into a vector Point3D format.
	Do this for both right and left views*/
	ConvertRobust3Dtxt_Point3Dvector(nr, right, left);

	/* Estimate transformation betwen right and left.
	Transform = rotation and translation.
	transf_count = the counter. */
	AbsoluteOrientation::compute(left,right,Transform[transf_count]);

	/*Read a set of 3D Binary coordinates
	and transform them into one step backward reference*/
	ChangeRef_OneStepBack(nr, rightPtrans);

	/*Read a set of 3D Binary coordinates
	and transform them all the way back into initial reference*/
	ChangeRef_AllStepsBack(nr);

	/*Save to file the rotations and translations*/
	//SaveTransform_OneStepBack(nr);

	/*Save to file the rotations and translations*/
	SaveTransform_AllStepsBack(nr);

	/* Convert Robust 3D points into the origin (initial reference system) */
	//ChangeRef_Robust(nr);
	//ChangeRef_Robust_AllStepsBack(nr);


	transf_count ++;
}