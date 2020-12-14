#include <iostream>
#include <fstream>
#include "AbsOrient.h"
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <string>
#include <time.h>

using namespace std;

void AbsoluteOrientation::compute(std::vector<Point3D> &left, std::vector<Point3D> &right, Frame &result)
{
	int i, pairNum = left.size();
	vnl_matrix<double> muLmuR(3,3,0), M(3,3,0), curMat(3,3,0), N(4,4,0);
	Point3D meanFirst, meanSecond; //assume points set to zero by constructor

	//compute the mean of both point sets
	for (i=0; i<pairNum; i++) {
		meanFirst[0] += left[i][0];
		meanFirst[1] += left[i][1];
		meanFirst[2] += left[i][2];
		meanSecond[0] += right[i][0];
		meanSecond[1] += right[i][1];
		meanSecond[2] += right[i][2];
	}
	meanFirst[0]/=pairNum;	  meanFirst[1]/=pairNum;	  meanFirst[2]/=pairNum;
	meanSecond[0]/=pairNum;	  meanSecond[1]/=pairNum;	  meanSecond[2]/=pairNum;

	//compute the matrix muLmuR
	muLmuR(0,0) = meanFirst[0]*meanSecond[0];		
	muLmuR(0,1) = meanFirst[0]*meanSecond[1];		
	muLmuR(0,2) = meanFirst[0]*meanSecond[2];
	muLmuR(1,0) = meanFirst[1]*meanSecond[0];
	muLmuR(1,1) = meanFirst[1]*meanSecond[1];
	muLmuR(1,2) = meanFirst[1]*meanSecond[2];
	muLmuR(2,0) = meanFirst[2]*meanSecond[0];
	muLmuR(2,1) = meanFirst[2]*meanSecond[1];
	muLmuR(2,2) = meanFirst[2]*meanSecond[2];

	//compute the matrix M
	for (i=0; i<pairNum; i++) {
		Point3D &leftPoint = left[i];
		Point3D &rightPoint = right[i];
		curMat(0,0) = leftPoint[0]*rightPoint[0];		
		curMat(0,1) = leftPoint[0]*rightPoint[1];		
		curMat(0,2) = leftPoint[0]*rightPoint[2];
		curMat(1,0) = leftPoint[1]*rightPoint[0];
		curMat(1,1) = leftPoint[1]*rightPoint[1];
		curMat(1,2) = leftPoint[1]*rightPoint[2];
		curMat(2,0) = leftPoint[2]*rightPoint[0];
		curMat(2,1) = leftPoint[2]*rightPoint[1];
		curMat(2,2) = leftPoint[2]*rightPoint[2];
		M+=curMat;
	}
	M+= (muLmuR *(-pairNum));

	//compute the matrix N	
	vnl_matrix<double> tmpMat(3,3,0);
	double A12, A20, A01;
	double traceM = 0.0;
	for(i=0; i<3; i++)
		traceM+=M(i,i);

	tmpMat.fill_diagonal(-traceM);
	tmpMat += (M + M.transpose());

	A12 = M(1,2) - M(2,1);
	A20 = M(2,0) - M(0,2);
	A01 = M(0,1) - M(1,0);

	N(0,0)=traceM; N(0,1)=A12; N(0,2)=A20; N(0,3)=A01;
	N(1,0)=A12;
	N(2,0)=A20;
	N(3,0)=A01;
	N.update(tmpMat,1,1);

	//find the eigenvector that belongs to the maximal 
	//eigenvalue of N, eigenvalues are sorted from smallest to largest
	vnl_symmetric_eigensystem<double> eigenSystem(N);

	Frame frm;
	frm.setRotationQuaternion(eigenSystem.V(0,3),eigenSystem.V(1,3),eigenSystem.V(2,3),eigenSystem.V(3,3), true);
	frm.apply(meanFirst);	

	result.setRotationQuaternion(eigenSystem.V(0,3),eigenSystem.V(1,3),eigenSystem.V(2,3),eigenSystem.V(3,3), true);
	result.setTranslation(meanSecond[0] - meanFirst[0], meanSecond[1] - meanFirst[1], meanSecond[2] - meanFirst[2]);
}