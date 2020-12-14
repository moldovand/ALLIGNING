#include <iostream>
#include <fstream>
#include "AbsOrient.h"
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <string>
#include <time.h>

using namespace std;

extern Frame *Transform;	
extern int transf_count;
extern ofstream outfile;
extern ofstream outfile2;

/*Print to file the transformation: 
one quaternion for rotation and 3 vectors for translation */
void SaveTransform_OneStepBack(int nr)
{
	char *buff;
	filebuf fb;
	
	double translation[3];
	double quaternion[4];

	//Save to file the rotations and translations
	buff = new char [100];
	sprintf(buff, "C:\\AllignData\\doll_img_feat\\Sequence\\transform%d_OneStepBack.txt", nr+1);
	fb.open (buff,ios::out);
	delete [] buff;

	ostream out(&fb);

	cout<<"Computed transformation:\n"<<transf_count<<"; \n";
	cout<<"***********************\n";
	cout<<Transform[transf_count]<<"\n";

	/* to print on console use out<< 
	to print to file use outfile<< */
	Transform[transf_count].setOutputFormat(3);
	cout<<Transform[transf_count]<<"\n";

	/* Extract rotation values quaternion style
	and print them to file*/ 
	Transform[transf_count].getRotationQuaternion(quaternion);
	
	out<<quaternion[0]<<" "<<quaternion[1]<<" "<<quaternion[2]<<" "<<
			quaternion[3]<<" ";

	/* Extract translation values
	and print them to file*/ 
	Transform[transf_count].getTranslation(translation);

	out<<translation[0]<<" "<<
		translation[1]<<" "<<translation[2]<<"\n";


	fb.close();
}
void SaveTransform_AllStepsBack(int nr)
{
	double translation[3];
	double quaternion[4];
	double rotMatrix[3][3];
	int new_count;
	Frame temp;

	/*First write the transformation corresponding to first step*/
	if(transf_count == 0){
		/* Extract rotation values quaternion style
		and print them to file*/ 
		Transform[transf_count].getRotationQuaternion(quaternion);
		
		outfile<<quaternion[0]<<" "<<quaternion[1]<<" "<<quaternion[2]<<" "<<
				quaternion[3]<<" ";

		Transform[transf_count].getRotationMatrix(rotMatrix);
		outfile2<<rotMatrix[0][0]<<" "<<rotMatrix[0][1]<<" "<<rotMatrix[0][2]<<"\n"<<
				rotMatrix[1][0]<<" "<<rotMatrix[1][1]<<" "<<rotMatrix[1][2]<<"\n"<<
				rotMatrix[2][0]<<" "<<rotMatrix[2][1]<<" "<<rotMatrix[2][2]<<"\n";

		/* Extract translation values
		and print them to file*/ 
		Transform[transf_count].getTranslation(translation);

		outfile<<translation[0]<<" "<<
			translation[1]<<" "<<translation[2]<<"\n";
		outfile2<<translation[0]<<" "<<
			translation[1]<<" "<<translation[2]<<"\n";

	}

	/* Here we compute the Transformations all the way back
	to initial coordinate system.
	First test if nr of transformations is big enough (>1) */

	if (transf_count >= 1){

		/* Go back one level */
		new_count = transf_count - 1;

		/*use temp as a reference.
		transf_count starts counting from 0*/
		temp = Transform[transf_count];

		/*until reach the bottom compute combination
			then move down*/
		while(new_count >= 0){

			temp.mul(Transform[new_count], temp);

			new_count = new_count - 1;

		}
		/* Extract rotation values quaternion style
		and print them to file*/ 
		temp.getRotationQuaternion(quaternion);
	
		outfile<<quaternion[0]<<" "<<quaternion[1]<<" "<<quaternion[2]<<" "<<
				quaternion[3]<<" ";

		temp.getRotationMatrix(rotMatrix);
		outfile2<<rotMatrix[0][0]<<" "<<rotMatrix[0][1]<<" "<<rotMatrix[0][2]<<"\n"<<
				rotMatrix[1][0]<<" "<<rotMatrix[1][1]<<" "<<rotMatrix[1][2]<<"\n"<<
				rotMatrix[2][0]<<" "<<rotMatrix[2][1]<<" "<<rotMatrix[2][2]<<"\n";

		/* Extract translation values
		and print them to file*/ 
		temp.getTranslation(translation);

		outfile<<translation[0]<<" "<<
			translation[1]<<" "<<translation[2]<<"\n";
		outfile2<<translation[0]<<" "<<
			translation[1]<<" "<<translation[2]<<"\n";


	}
}