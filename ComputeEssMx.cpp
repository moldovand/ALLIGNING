#include "protos.h"
using namespace std;

extern int x_crop, y_crop;
int FindLine_Write3D(FILE *fp, int p1, int p2, int nr);
void ReadMatches(FILE *fp, CvPoint *p1, CvPoint *p2, int &numPoints);

/*The function computes the essential matrix based on SIFT correspondences.
Then it applies RANSAC and robustly filters out the 2D outliers.
Also, based on the correspondences 2D-3D it robustly generates 
a file with 3D correspondences.*/
void ComputeEssMx(int nr)
{
	int i1,i2;
	int x, y;
	int numPoints;

	i2 =0;
	FILE *fp_3d;
	FILE *fp1;
	char *buff1, *buff2, *buff3;
	CvPoint *p1;
	CvPoint *p2;

	CvMat* points1;
	CvMat* points2;
	CvMat* status;
	CvMat* fundMatr;

	/* Get the SIFT correspondences
	and the numPoints value*/ 
	buff1 = new char [100];
	sprintf(buff1, "C:\\Model.Teddy.Bear\\SiftData\\sift%d-%d.txt", nr, nr+1);
	fp1 = fopen(buff1, "r");
	delete [] buff1;
	fscanf(fp1, "%d", &numPoints);

	p1 = new CvPoint [numPoints];
	p2 = new CvPoint [numPoints];
	/* Read into p1 and p2 the values 
	of 2D correspondences*/
	ReadMatches(fp1, p1, p2, numPoints);
	fclose(fp1);

	points1 = cvCreateMat(2,numPoints,CV_32F);
	points2 = cvCreateMat(2,numPoints,CV_32F);
	status = cvCreateMat(1,numPoints,CV_32F);

	for ( i1 = 0; i1 < numPoints; i1++) {
		cvSetReal2D(points1,0,i1,p1[i1].x/1); 
		cvSetReal2D(points1,1,i1,p1[i1].y/1); 
		
		cvSetReal2D(points2,0,i1,p2[i1].x/1); 
		cvSetReal2D(points2,1,i1,p2[i1].y/1); 
		}

	//CREATE THE OUTPUT FUNDAMENTAL MATRIX
	fundMatr = cvCreateMat(3,3,CV_32F);

	//see opencv manual for other options in computing the fundamental matrix	
	int num = cvFindFundamentalMat(points1,points2,fundMatr,CV_FM_RANSAC,1.0,0.9999,status);

	buff2 = new char [100];
	buff3 = new char [100];
	sprintf(buff2, "C:\\Model.Teddy.Bear\\RoughApprox\\RobustMatches%d-%d.txt", nr, nr+1);
	sprintf(buff3, "C:\\Model.Teddy.Bear\\RoughApprox\\Robust_3DMatches%d-%d.txt", nr, nr+1);
	fp1 = fopen(buff2, "w");
	fp_3d = fopen(buff3, "w");
	delete [] buff2;
	delete [] buff3;

	int rm = 0;//robust matches
	int p_file1 = 0;//position in file
	int p_file2 = 0;//position in file

	if( num == 1 ){
		/*Print out F*/
		printf("Fundamental matrix was found\n");
		for(int i = 0; i < 3; i ++){
			for(int j = 0; j < 3; j ++){
				float t;
				t = cvmGet(fundMatr, i, j);
			}
		}
		cout<<"\n";

		/*Print Out inliers*/
		for(int i = 0; i < numPoints; i ++){
			int t;
			t = cvmGet(status, 0, i);
			/*cout<<t<<", ";*/
			if ((t == 1)){//&&(i < 10)){
				//fprintf(fp1, "  %d  %d  %d  %d\n", p1[i].x, p1[i].y, p2[i].x, p2[i].y);
				/* dwarf two 180deg views */
				//p_file1 = (p1[i].y + 75) * 640 + p1[i].x + 190;
				//p_file2 = (p2[i].y + 75) * 640 + p2[i].x + 190;
				/* dwarf 17 20deg views */
				p_file1 = (p1[i].y + y_crop) * 640 + p1[i].x + x_crop;
				p_file2 = (p2[i].y + y_crop) * 640 + p2[i].x + x_crop;
				/* general views */
				//p_file1 = (p1[i].y + 69) * 640 + p1[i].x + 120;
				//p_file2 = (p2[i].y + 69) * 640 + p2[i].x + 120;

				/*parameters include positions in file to be searched and file name*/
				if(FindLine_Write3D(fp_3d, p_file1, p_file2, nr))
					fprintf(fp1, "  %d  %d  %d  %d\n", p1[i].x, p1[i].y, p2[i].x, p2[i].y);


				rm++;
			}
		}
		cout<<"\n";
		//fprintf(fp1, "Number of points: %d \n", rm);
	}
	else
	{
		printf("Fundamental matrix was not found\n");
		return;

	}

	fclose(fp1);
	fclose(fp_3d);
	delete [] p1;
	delete [] p2;

}