////////////////////////////////
// 2008						  //
// TD Informatique Graphique  //
// Université Paris 11	      //
// Mehdi AMMI - ammi@limsi.fr //
////////////////////////////////

#include <GL/glut.h>    // Header pour GLUT
#include <GL/gl.h>	// Header pour OpenGL
#include <GL/glu.h>	// Header pour GLu
#include <stdlib.h>     // Heard  Utilitaire général

int seconds = 0;

/* Postion de la source de lumière */

GLfloat light_position[] = { 1.0F,0.0F,1.0F,0.0F };
GLfloat lightambiant[] = { 1.0f,1.0f,1.0f,0.0};

/* code ASCII pour la touche escape*/
#define ESCAPE 27


/* Idantifiant de la fenêtre GLUT */
int window;

/* .. */
void InitGL(int Width, int Height)
{
// ..
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

// ..
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

// .. **
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glLightfv(GL_LIGHT0,GL_POSITION,light_position);
  glLightfv(GL_LIGHT0,GL_AMBIENT,lightambiant);


// ..
  glShadeModel(GL_SMOOTH);

// ..
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

// Projection orthogonale sans aspect ratio
//..

// Projection orthogonale avec aspect ratio
//..

  glMatrixMode(GL_MODELVIEW);

}

/* .. */
void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)
    Height=1;

  glViewport(0, 0, Width, Height);

// ..
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

// Projection orthogonale sans aspect ratio
//..

// Projection orthogonale avec aspect ratio
//..

  glMatrixMode(GL_MODELVIEW);

}

/* .. */
void DrawGLScene()
{
// ..
  seconds++;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();


  glLoadIdentity();
  gluLookAt(8.0,6.0,4.0,0.0,0.0,0.0,0.0,1.0,0.0);
  
  glClearColor (0.0, 0.0, 0.0, 0.0); 
  glClear(GL_COLOR_BUFFER_BIT);

  // Dessin des poins et segments
//  glBegin();
//  glEnd();

/*
  glBegin(GL_POINTS);
  glEnd();
  
  
  
  glBegin(GL_LINES); 
  glColor3f(1.0,1.0,1.0);
  glVertex3f(0.0f, 0.0f, 0.0f);
  
  glColor3f(1.0,0.0,0.0);
  glVertex3f(1.0f, 1.0f, 1.0f);
  
  glEnd();
  glFlush();
  * */
  
  /*
  glBegin(GL_LINE_STRIP);
  glColor3f(1.0,1.0,1.0);
  glVertex3f(0.0f, 0.0f, 0.0f);
  
  glColor3f(1.0,0.0,0.0);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glEnd();
   */
  
  
  // Dessin d'un référentiel
  glBegin(GL_LINES);
  glColor3f(1.0,0.0,0.0);
  glVertex3f(0.0f, 0.0f, 0.0f);  
  glVertex3f(5.0f, 0.0f, 0.0f);
  
  glColor3f(0.0,1.0,0.0);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 5.0f, 0.0f);
  
  glColor3f(0.0,0.0,1.0);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 5.0f);
  glEnd();
  glFlush();
  
  
  //..
	
	
	/*
  // Dessin d'un triangle
  glBegin(GL_TRIANGLES);
  glColor4f(1.0,0.0,0.0,0.4);
  glVertex3f(0.0f, 0.0f, 0.0f);
  
  glColor4f(0.0,0.0,1.0,0.5);
  glVertex3f(3.0f,0.0f, 0.0f);
  
  glColor4f(0.0,1.0,0.0,1.0);
  glVertex3f(0.0f,3.0f, 0.0f);
  glEnd();
  glFlush();
*/
  //..

/*
  // Dessin d'un triangle coloré uniformément
  glShadeModel(GL_FLAT);
  glBegin(GL_TRIANGLES);
  glColor4f(1.0,0.0,0.0,0.4);
  glVertex3f(0.0f, 0.0f, 0.0f);
  
  glColor4f(0.0,0.0,1.0,0.5);
  glVertex3f(3.0f,0.0f, 0.0f);
  
  glColor4f(0.0,1.0,0.0,1.0);
  glVertex3f(0.0f,3.0f, 0.0f);
  glEnd();
  glFlush();
  //..
*/
/*
  // Dessin d'un triangle coloré par interpolation
  glBegin(GL_TRIANGLES);
  
  glColor3f(1.0,0.0,0.0);
  glVertex3f(0.0f, 0.0f, 0.0f);
  
  glColor3f(0.0,1.0,0.0);
  glVertex3f(3.0f,0.0f,0.0f);
  
  glColor3f(0.0,0.0,1.0);
  glVertex3f(0.0f,3.0f,0.0f);
  glEnd();
  glFlush();

  //..
*/	



  // Cube sans couleur

  //Translation de l'objet

  //..
  
  
  glTranslatef(-1.5,-1.5,-1.5);  
  glRotatef(seconds,1,0,0);
  
  
  glBegin(GL_QUADS);
  
  //Face 1  
  glColor3f(1.0,1.0,0.0);
  glVertex3f(0.0f, 0.0f, 0.0f);
  
  glVertex3f(3.0f,0.0f,0.0f);
  
  glVertex3f(3.0f,3.0f,0.0f);
 
  glVertex3f(0.0f,3.0f,0.0f);  
  
  glEnd();
 
  //.

  // Face 2
  
   glBegin(GL_QUADS);

  glColor3f(1.0,0.0,0.0);
  glVertex3f(0.0f, 0.0f, 0.0f);
  
  glVertex3f(0.0f,0.0f,3.0f);
  
  glVertex3f(0.0f,3.0f,3.0f);
 
  glVertex3f(0.0f,3.0f,0.0f);  
  
  glEnd();
 
  
   //..
   

  // Face 3
  
   glBegin(GL_QUADS);
  
  glColor3f(0.0,0.0,1.0);
  glVertex3f(0.0f, 0.0f, 3.0f);
  
  glVertex3f(0.0f,3.0f,3.0f);
    
  glVertex3f(3.0f,3.0f,3.0f);

  glVertex3f(3.0f,0.0f,3.0f);  
  
  glEnd();


  // Face 4
  
   glBegin(GL_QUADS);
 
  glColor3f(0.0,1.0,1.0);
  glVertex3f(3.0f,0.0f,3.0f);  
  
  glVertex3f(3.0f,0.0f,0.0f);
    
  glVertex3f(3.0f,3.0f,0.0f);
  
  glVertex3f(3.0f,3.0f,3.0f);  
  glNormal3f(0.0,0.0,1.0);
  
  glEnd();


  glBegin(GL_QUADS);
 
  // Face 5
  glColor3f(0.0,1.0,0.0);
  glVertex3f(0.0f, 0.0f, 0.0f);
  
  glVertex3f(3.0f,0.0f,0.0f);
  
  glVertex3f(3.0f,0.0f,3.0f);
  
  glVertex3f(0.0f,0.0f,3.0f);
  //..


  // Face 6
  
  glVertex3f(0.0f, 3.0f, 0.0f);
  
  glVertex3f(3.0f,3.0f,0.0f);
  
  glVertex3f(3.0f,3.0f,3.0f);
  
  glVertex3f(0.0f,3.0f,3.0f);
  //..
  
  glEnd();
  glFlush();
 
  
 
    
  


// glutSolidCube(2.0);





  // Cube avec une couleur pour chaque face

  //Translation de l'objet

  //..

  //Face 1
  //..


  // Face 2
  //..


  // Face 3
  //..



  // Face 4
  //..


  // Face 5
  //..


  // Face 4
  //..



  // Cube avec noramles

  //Translation de l'objet
  //..

  //Face 1
  //..


  // Face 2
  //..


  // Face 3
  //..


  // Face 4
  //..

  // Face 5
  //..

  // Face 6
  //..


  // Cube avec noramles

  //..

  //Face 1
  //..


  // Face 2
  //..

  // Face 3
  //..

  // Face 4
  //..

  // Face 5
  //..

  // Face 6
  //..

  // Permutation des buffers
  
  
  glutSwapBuffers();
  glutPostRedisplay();
}

/* .. */
void keyPressed(unsigned char key, int x, int y)
{

    if (key == ESCAPE)
    {
	/* .. */
	glutDestroyWindow(window);

	/* .. */
	exit(0);
    }
}

/* .. */
int main(int argc, char **argv)
{
  /* .. */
  glutInit(&argc, argv);

  /* .. */
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

  /* .. */
  glutInitWindowSize(640, 480);

  /* .. */
  glutInitWindowPosition(0, 0);

  /* .. */
  window = glutCreateWindow("TD Informatique Graphique");

  /* .. */
  glutDisplayFunc(&DrawGLScene);  

  /* .. */
  glutReshapeFunc(&ReSizeGLScene);

  /* .. */
  glutKeyboardFunc(&keyPressed);

  /* .. */
  InitGL(640, 480);

  /* .. */
  glutMainLoop();

  return 1;
}
