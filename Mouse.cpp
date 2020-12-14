#include "protos.h"

using namespace std;

extern float camAlpha;
extern float camBeta;

static int mouseX = -999, mouseY = -999;
static bool mouseState[3] = {false};
static int modifiers = 0;
extern bool screen_dirty;

int selected_cp = -1;

void keyboard( unsigned char key, int, int );

void mouse( int button, int state, int, int )
{
	mouseState[ button ] = (state==GLUT_DOWN);
	modifiers = glutGetModifiers();

  glutSetCursor( GLUT_CURSOR_CROSSHAIR );

  if ( button == 1 && state==GLUT_DOWN )
      glutSetCursor( GLUT_CURSOR_CYCLE );

	if ( button == 0 )
  {
    if ( state==GLUT_UP )
    {		  
      screen_dirty=true;
    }
    else if ( state==GLUT_DOWN && selected_cp<0)
      keyboard( 'a', 0,0 );
  }
}
// OGL: mouse movement callback
void mouseMotion( int x, int y )
{
	vector< Vec > control_points;
	if ( mouseState[0] && mouseX != -999 )
	if ( selected_cp >= 0 )
		control_points[selected_cp].y += -(y - mouseY)/3;

	if ( mouseState[1] && mouseX != -999 )
	{
    camAlpha += -(y - mouseY);
    camBeta += (x - mouseX);

    screen_dirty=true;
	}

  if ( mouseX != x || mouseY != y )
  {
    mouseX = x;
    mouseY = y;
    screen_dirty=true;
  }
}