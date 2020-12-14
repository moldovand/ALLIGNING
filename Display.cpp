#include "protos.h"

extern float camAlpha, camBeta, camZoom;
extern bool screen_dirty;
extern int winW, winH;
extern float ***p;
extern float ***pc;
extern int *size;
extern int Nr_angle;


Mtx rotateX( float ang );
Mtx rotateY( float ang );
Mtx operator * ( const Mtx& a, const Mtx& b );


void draw_string (const char* str)
{
  for (unsigned i=0; i<strlen(str); i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
};

void display(void)
{
	unsigned i;
	Vec cam_loc(0,0,150), cam_up(0,1,0);
	Mtx& rot = rotateY( camBeta ) * rotateX( camAlpha );
	//Make a rotation matrix out of mouse point
	cam_loc = cam_loc * rot;
	cam_up = cam_up * rot;

	// CLEAR THE SCREEN
	glClearColor(1.0f,1.0f,1.0f,0.0f);
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

	// PREPARE ZOOM BY CHANGIQNG FOV
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float fov = 45 + camZoom;
	if ( fov < 5 ) fov = 5;
	if ( fov > 160 ) fov = 160;
	gluPerspective( fov, (float)winW/(float)winH, 1.0, 500.0 );

	gluLookAt( cam_loc.x, cam_loc.y, cam_loc.z,  // eye
		0, 0, 0, // target
		cam_up.x, cam_up.y, cam_up.z ); // up

	glDisable(GL_LIGHTING) ;

	//LOAD 3D DATA
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPointSize(1.0);
	glBegin(GL_POINTS);
	for(int i = 0; i < Nr_angle; i=i+1)//18
		for(int j = 0; j < size[i]; j++){
			if((p[i][j][1] < -120) && (pc[i][j][0] != 1.0) && (pc[i][j][1] != 1.0) && (pc[i][j][2] != 1.0))
			;
			else {
				glColor3f(pc[i][j][0], pc[i][j][1], pc[i][j][2]);
				glVertex3f(p[i][j][0], p[i][j][1], p[i][j][2] + 950);
			}
		}
	glEnd();

  // Axes: (x,y,z)=(r,g,b)
/*
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glPushMatrix();
  glRotatef(90.0, 0.0, 1.0, 0.0);
  glColor3ub( 255, 0, 0 );
  glutSolidCone(0.5, 80.0, 5, 1);
  glPopMatrix();
  glPushMatrix();
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  glColor3ub( 0, 255,  0 );
  glutSolidCone(0.5, 80.0, 5, 1);
  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 1.0, 0.0, 0.0);
  glColor3ub( 0, 0, 255 );
  glutSolidCone(0.5, 80.0, 5, 1);
  glPopMatrix();
*/
  static char tmp_str[255];
  glDisable( GL_DEPTH_TEST );
  glLoadIdentity();
  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
  glColor3ub( 0, 0, 0 );
  glRasterPos2f (-0.95, -0.95);
  sprintf( tmp_str, "Aligning: %d views", Nr_angle);
  draw_string( tmp_str );
  glEnable( GL_DEPTH_TEST );

  glFlush();
  glutSwapBuffers();
  screen_dirty = false;
}

void reshape( int w, int h )
{
	winW = w;
	winH = h;
	glViewport( 0, 0, winW, winH );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
  glEnable( GL_NORMALIZE );
  glDepthFunc( GL_LESS );
}

// Print out OpenGL errors, if any
void glCheckErrors()
{
	GLenum errCode = glGetError();
	if ( errCode != GL_NO_ERROR )
	{
		const GLubyte *errString = gluErrorString( errCode );
		fprintf(stderr, "OpenGL error: %s\n", errString);
	}
}

// OGL: idle callback, continuosly keep redrawing
void idlefunc()
{
  glCheckErrors();
  if (screen_dirty)
    glutPostRedisplay();
}
