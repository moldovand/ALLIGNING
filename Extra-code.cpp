/////////////////FROM GET_DATA FUNCTION  
FILE *fp;
  char *buff;
  int i, j;
  float **a;
  float x, y, z;

//READ THE 3D CORRESPONDENCES
  buff = new char [100];
  sprintf(buff, "C:\\AllignData\\doll\\sift\\Robust_3DMatches01.txt");
  fp = fopen(buff, "r");
  if (! fp){
	  printf("Error open file %d \n", i);
	  exit(0);
  }
  
  while(!feof(fp)){
	  fscanf(fp, "%f", &x);
	  fscanf(fp, "%f", &y);
	  fscanf(fp, "%f", &z);
	  leftPoint[0] = x;
	  leftPoint[1] = y;
	  leftPoint[2] = z;
	  left.push_back(leftPoint);

	  fscanf(fp, "%f", &x);
	  fscanf(fp, "%f", &y);
	  fscanf(fp, "%f", &z);
	  rightPoint[0] = x;
	  rightPoint[1] = y;
	  rightPoint[2] = z;
	  right.push_back(rightPoint);
  }
  fclose(fp);

  delete [] buff;

////////////////FROM DISPLAY FUNCTION
  glMatrixMode(GL_PROJECTION);
//Make a rotation matrix out of mouse point
const Mtx& rot = rotateY( camBeta ) * rotateX( camAlpha );

// Rotate camera
 Vec cam_loc(0,0,-150), cam_up(0,1,0);
 cam_loc = cam_loc * rot;
 cam_up = cam_up * rot;

//DISPLAY ONE COLORED VIEW (GREEN OR RED)
 if (i == 0)
	 glColor3f(1.0, 0.0, 0.0);
 else
	 glColor3f(0.0, 1.0, 0.0);

//LOAD 3D POINTS
glPointSize(5.0);
glColor3f(0.0f,0.0f,1.0f);
glBegin(GL_POINTS);
for(int i = 0; i < pt3D; i++){
	//glVertex3f(left[i][0], left[i][1], left[i][2] + 900);
	glVertex3f(right[i][0], right[i][1], right[i][2] + 900);
}
glEnd();

///////////////////FROM KEYBOARD FUNCTION
static void keyboard( unsigned char key, int, int )
{
  switch (key)
  {
    case 'a':
		Get_Data();
      break;
	case 'r':
		{
  FILE *fp;
  char *buff;
  int nr, j;

//read in the 3D data
  buff = new char [100];
  
  sprintf(buff, "C:\\AllignData\\doll\\pict0_3Dtrans.txt");
  fp = fopen(buff, "r");
  if (! fp){
  printf("Error open file 0 \n");
	  exit(0);
  }

  fscanf(fp, "%d", &nr);
  for(j = 0; j < size[0]; j ++)
	fscanf(fp, "%f %f %f", &p[0][j][0], &p[0][j][1], &p[0][j][2]);

  fclose(fp);
  delete [] buff;

		}
		break;
    case 'd':
		{ 
		pt3D++;
		}
      break;
    case 'c':      
		{
			pt3D --;
		}
      break;
    case '+':
      /*regularization += 0.025;*/
      break;
    case '-':
      /*regularization -= 0.025;*/
      /*if (regularization < 0) regularization = 0;*/
      
      break;
    case '/': camZoom -= 1; break;
    case '*': camZoom += 1; break;
    case 'q': exit( 0 ); break;
  }
  screen_dirty=true;
}

///////////////////FROM ABSOLUTE ORIENTATION
  //Test the 3D transformation
  for(i = 0; i <9; i++)
  {
	  checkLeft[0] = left[i][0];
	  checkLeft[1] = left[i][1];
	  checkLeft[2] = left[i][2];
	  out<<checkLeft;
	  computedTransformation.apply(checkLeft);
	  out<<checkLeft;
	  std::cout<<"\n";
  }

