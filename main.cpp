#include "protos.h"

using namespace std;

/* GLOBALS: 
Transform: transformations between consecutive references
numPoints: Number of corresponding 2D points generated by SIFT
step: step in computing transformations succesively
jump: one time check for jumping to next registration group*/
//Frame Transform[27];
Frame *Transform;

int numPoints;
int transf_count = 0;
int winW = 600;
int winH = 600;

//for location of left corner of my Image crop
int x_crop, y_crop;
//for number of intermediate angles
int Nr_angle;

float camAlpha = -5;
float camBeta = 45;
float camZoom = 50;
float ***p;
float ***pc;

bool screen_dirty = true;

int *size;
int step = 0;
int jump = 0;
int all = 0;
int step1;

/*necessary for writing the transformations
(relative to starting point) to file */ 
ofstream outfile;
ofstream outfile2;


IplImage* image ;
IplImage* image2 ;

/* DECLARATIONS */
void Get_Data(void);
void mouse( int button, int state, int, int );
void mouseMotion( int x, int y );
void display(void);
void reshape( int w, int h );

void keyboard( unsigned char key, int, int );
void keyboard_special( int key, int, int );
void menu_select(int mode);
void create_menu(void);

void display2DImage_SIFT(int nr);
void ComputeEssMx(int nr);

void glCheckErrors();
void idlefunc();

// STARTUP
int main( int argc, char *argv[] )
{
	char *buff;

	x_crop = atoi(argv[1]);
	y_crop = atoi(argv[2]);
	Nr_angle = atoi(argv[3]);
	Get_Data();


	/*prepare the file name for outfile variable*/
	buff = new char [100];
	sprintf(buff, "C:\\Model\\RoughApprox\\transf_AllSteps.txt");
	outfile.open(buff);
	delete [] buff;

	buff = new char [200];
	sprintf(buff, "C:\\Model\\RoughApprox\\transfMatrix_AllSteps.txt");
	outfile2.open(buff);
	delete [] buff;

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( winW, winH );
	glutInitWindowPosition( 0, 0 );
	if ( !glutCreateWindow( "RegisterDemo" ) )
	{
		printf( "Couldn't open window.\n" );
		return 1;
	}
	glutDisplayFunc( display );
	glutIdleFunc( idlefunc );
	glutMouseFunc( mouse );
	glutMotionFunc( mouseMotion );
	glutPassiveMotionFunc( mouseMotion );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( keyboard_special );
	glutReshapeFunc( reshape );
	glutSetCursor( GLUT_CURSOR_CROSSHAIR );

	glCheckErrors();
	create_menu() ;
	glutMainLoop();

	outfile.close();
	outfile2.close();
	return 0;
}
