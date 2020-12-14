#include <string>
#include<fstream>
#include <iostream>
#include<cstdio>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

using namespace std;

extern IplImage* image ;
extern IplImage* image2 ;
extern float ***p;
extern int *size;

//int FindLine_Write3D(FILE *fp, int p1, int p2, int nr);
void NextNumber(FILE *fp);
void ReadMatches(FILE *fp, CvPoint *p1, CvPoint *p2, int &numPoints);

void DisplaySequencePoints(CvPoint *p1, CvPoint *p2, int &numPoint)
{
	CvFont font;
	int cntr;
	char cntr_s[5];

	cvInitFont(&font,CV_FONT_HERSHEY_PLAIN|CV_FONT_ITALIC,1,1,0,1);

	cvCircle(image,p1[numPoint],1,cvScalar(0,0,255),3);
	cntr = numPoint;
	_itoa(cntr,cntr_s,10);

	cvPutText(image,cntr_s,p1[numPoint],&font,cvScalar(0,0,255));
	cvShowImage("Image1", image);
	//cvSaveImage("C:\\AllignData\\doll\\sift\\img0.jpg",image);

	cvCircle(image2,p2[numPoint],1,cvScalar(0,0,255),3);
	cntr = numPoint;
	_itoa(cntr,cntr_s,10);

	cvPutText(image2,cntr_s,p2[numPoint],&font,cvScalar(0,0,255));
	cvShowImage("Image2", image2);
	//cvSaveImage("C:\\AllignData\\doll\\sift\\img1.jpg",image2);

}
void display2DImage_SIFT(int nr)
{
	FILE *pFile;
	char *buff1, *buff2, *buff3;
	int numPoints;
	int x, y;

	CvPoint *p1;
	CvPoint *p2;

	// LOAD 8-BIT, 1 CHANNEL GRAYSCALE PGM IMAGE
	buff1 = new char [100];
	buff2 = new char [100];
	sprintf(buff1, "C:\\AllignData\\doll_img_feat\\pict%d.pgm", nr-1);
	sprintf(buff2, "C:\\AllignData\\doll_img_feat\\pict%d.pgm", nr+1);
	image = cvLoadImage(buff1);
	image2 = cvLoadImage(buff2);
	delete [] buff1;
	delete [] buff2;

	// CREATE A WINDOW IN WHICH THE CAPTURED IMAGES WILL BE PRESENTED
	cvNamedWindow("Image1", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image1", image);

	// CREATE A WINDOW IN WHICH THE CAPTURED IMAGES WILL BE PRESENTED
	cvNamedWindow("Image2", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image2", image2);

	buff3 = new char [100];
	sprintf(buff3, "C:\\AllignData\\doll_img_feat\\SIFT\\RobustMatchesCtr%d-%d.txt", nr-1, nr+1);
	pFile = fopen(buff3, "r");
	delete [] buff3;
	fscanf(pFile, "%d", &numPoints);

	p1 = new CvPoint [numPoints];
	p2 = new CvPoint [numPoints];
	ReadMatches(pFile, p1, p2, numPoints);
	fclose(pFile);

	for(int i = 0; i < numPoints; i++){//numPoints
		DisplaySequencePoints(p1, p2, i);
		//cvWaitKey(0);
	}

	delete [] p1;
	delete [] p2;
}