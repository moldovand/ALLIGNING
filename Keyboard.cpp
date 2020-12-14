#include "protos.h"

using namespace std;

extern int all;
extern int step;
extern int step1;
extern int jump;
extern float camAlpha;
extern float camBeta;
extern float camZoom;
extern bool screen_dirty;
extern int Nr_angle;


void Get_Data(void);
void ComputeEssMx(int nr);
void UpdateDisplay_RegisteredData(int nr);

// key press callback
void keyboard( unsigned char key, int, int )
{
	char str_buf[2];

	switch (key)
	{
	/* refresh to starting condition */
	case 'd':
		Get_Data();
		break;

	/*register two views (every 2-nd view)*/
	case 'r':
		for(int i = 0; i < (Nr_angle - 1); i++){
		cout<<"step = "<<step<<"\n";

		/*Generate robust 3D matches */
		ComputeEssMx(step);

		/*Extract rotation and translation*/
		AbsoluteOrientation::selfTest(step);

		/*Update the OpenGL data*/
		UpdateDisplay_RegisteredData(step);

		//display2DImage_SIFT(step);

		/*move to the next set*/
		step = step + 1;
		}

		break;

	case '1':
		break;
	case '2':      
		//Write_RegisteredData2();
		all = 1;
		break;
	case 'f':
		if(jump == 0)
			step1 = 15;
		else {}
		cout<<"step1 = "<<step1<<" \\\\\\\ \n";
		jump ++;
		cout<<"jump = "<<jump<<" \\\\\\\ \n";
		ComputeEssMx(step1);
		//AbsoluteOrientation::selfTest(std::cout, step1);
		AbsoluteOrientation::selfTest(step);
		UpdateDisplay_RegisteredData(step1);
		//display2DImage_SIFT(step);
		step1 = step1 + 2;
		break;
	case '+':
		/*regularization += 0.025;*/
		break;
	case '-':
		/*regularization -= 0.025;*/
		/*if (regularization < 0) regularization = 0;*/

		break;
	case '/': { camZoom -= 1;} break;
	case '*': camZoom += 1; break;
	case 'q': exit( 0 ); break;
	}
	screen_dirty=true;
}
// OGL: keyboard press callback for special characters
void keyboard_special( int key, int, int )
{
  switch (key)
  {
    case GLUT_KEY_UP: camAlpha += 5 ; break;
    case GLUT_KEY_DOWN: camAlpha += -5 ; break;
    case GLUT_KEY_RIGHT: camBeta += -5 ; break;
    case GLUT_KEY_LEFT: camBeta += 5 ; break;
  }
  screen_dirty=true;
}

void menu_select(int mode)
{
  keyboard( (unsigned char)mode, 0,0 );
}

void create_menu(void)
{
  glutCreateMenu(menu_select);
  glutAddMenuEntry(" d     Delete control point",'d');
  glutAddMenuEntry(" a     Add control point",'a');
  glutAddMenuEntry(" c     Clear all",'c');
  glutAddMenuEntry(" +     Relax more",'+');
  glutAddMenuEntry(" -     Rela less",'-');
  glutAddMenuEntry(" /     Zoom in",'/');
  glutAddMenuEntry(" *     Zoom out",'*');
  glutAddMenuEntry(" q     Exit",'q');
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}