////////////////////////////////
// 2008			      //
// TD Animation 3D  //
// Université Paris 11	      //
// Mehdi AMMI - ammi@limsi.fr //
////////////////////////////////

#include <GL/glut.h>    // Header pour GLUT
#include <GL/gl.h>	// Header pour OpenGL
#include <GL/glu.h>	// Header pour GLu
#include <stdlib.h>     // Header  Utilitaire général
#include <math.h>       // Header fonctions mathématiques

/* Postion de la source de lumière */


// g++ -o TD_GL2 TD_GL_2.c -lGL -lGLU -lglut

GLfloat light_position_1[] = {0.0f,20.0f,-15.0f, 0.0f};
GLfloat light_diffuse_1[] = {1.0, 1.0, 1.0, 0.0};

GLfloat light_position_2[] = {0.0f,30.0f,30.0f, 0.0f};
GLfloat light_diffuse_2[] = {1.0, 1.0, 1.0, 0.0};

/* Definition matériaux */

GLfloat mat_specularBLUE[] ={0.05,0.05,0.75,1.0};
GLfloat mat_ambientBLUE[] ={0,0,1,1.0};
GLfloat mat_diffuseBLUE[] ={0.50,0.50,0.50,1.0};
GLfloat mat_shininessBLUE[] ={128.0};

GLfloat mat_specularDarkBLUE[] ={0.05,0.05,0.75,1.0};
GLfloat mat_ambientDarkBLUE[] ={0,0,1,1.0};
GLfloat mat_diffuseDarkBLUE[] ={0.40,0.40,0.40,1.0};
GLfloat mat_shininessDarkBLUE[] ={128.0};

GLfloat mat_specularLightBLUE[] ={0.05,0.05,0.75,1.0};
GLfloat mat_ambientLightBLUE[] ={0,0,1,1.0};
GLfloat mat_diffuseLightBLUE[] ={0.90,0.90,0.90,1.0};
GLfloat mat_shininessLightBLUE[] ={128.0};


GLfloat mat_specularGREEN[] ={0.633, 0.727811, 0.633,1.0};
GLfloat mat_ambientGREEN[] ={0.1215, 0.2745, 0.1215,1.0};
GLfloat mat_diffuseGREEN[] ={0.27568, 0.31424, 0.27568,1.0};
GLfloat mat_shininessGREEN[] ={128.0};

GLfloat mat_specularYELLOW[] ={0.0,0.0,0.0,1.0};
GLfloat mat_ambientYELLOW[] ={1,0.7,0.,1.0};
GLfloat mat_diffuseYELLOW[] ={0.50,0.50,0.50,1.0};
GLfloat mat_shininessYELLOW[] ={128.0};

GLfloat mat_specularRED[] ={0.75,0.75,0.75,1.0};
GLfloat mat_ambientRED[] ={1.0,0.0,0.0,1.0};
GLfloat mat_diffuseRED[] ={0.8,0.50,0.50,1.0};
GLfloat mat_shininessRED[] ={128.0};

GLfloat mat_specularORANGE[] ={0.75,0.75,0.75,1.0};
GLfloat mat_ambientORANGE[] ={0.8,0.5,0.0,1.0};
GLfloat mat_diffuseORANGE[] ={1.0,0.5,0.0,1.0};
GLfloat mat_shininessORANGE[] ={128.0};


/* Paramètres des articulations DDL*/

float base_x = 0.0,base_y = 0.0;
float base_rot = 0.0;
float first_x = 0.0,first_y = 20.0;
float second_x = 0.0,second_y = 20.0;
float third_x = 0.0,third_y = 20.0;
float finger_x = 0.0;

/* Paramètres caméra de navigation */

float up_down = 0.0, left_right = -1.57;
float cam_pos_x = 0.0;
float cam_pos_z = 30.0;
float cam_look_x = 0.0;
float cam_look_z = 0.0;
float vect_x = 0.0;
float vect_z = 0.0;

/* variable d'animation */

double rot_1 = 0.1;
double rot_2 = 0.1;
double rot_3 = 0.1;

float angle = 0.0f;
float axis = 5.0f;

/* code ASCII pour la touche escape*/

#define ESCAPE 27
#define SPACE 32

/* Idantifiant de la fenêtre GLUT */

int window;


/* Headers */

void Special_key(int key, int x, int y);
void Keyboard_key(unsigned char key, int x, int y);
void ground();
void SetMaterial(GLfloat spec[], GLfloat amb[], GLfloat diff[], GLfloat shin[]);
void base();
void first_arm();
void second_arm();
void third_arm();
void finger_1();
void finger_2();
void roue();

void move_camera(double speed);
void rotate_camera(double speed);
void axis_rotation(float,float,float);

/* Fonction d'initialisation */
void InitGL(int Width, int Height)
{
// Couleur d'effacement du buffer de couleur
  glClearColor(0.3f, 0.3f, 0.4f, 0.0f);

// paramètrage du Z-buffer
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

// Activation de l'éclairage
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

// Paramètrage de l'éclairage
  glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse_1);
  glLightfv(GL_LIGHT0,GL_POSITION,light_position_1);

  glLightfv(GL_LIGHT1,GL_DIFFUSE,light_diffuse_2);
  glLightfv(GL_LIGHT1,GL_POSITION,light_position_2);

// Normalisation des normales
  glEnable(GL_NORMALIZE);

// Activation du lissage
  glShadeModel(GL_SMOOTH);

// Projection perceptive
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,1000.0f);

  glMatrixMode(GL_MODELVIEW);

}

/* Fonction de redimensionnement de la fenêtre */
void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)
    Height=1;

  glViewport(0, 0, Width, Height);

// Projection perceptive
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,1000.0f);

  glMatrixMode(GL_MODELVIEW);

}

/* Focntion de dessin */
void DrawGLScene()
{
// Effacement du buffer de couleur et de profondeur
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glEnable(GL_LINE_SMOOTH);



//////////////////////////////////////////////////
// La camera

 gluLookAt(0.0,20.0,60.0,0.0,5.0,0.0,0.0,1.0,0.0); // 0.0,5.0,30.0

// Navigation
//...

//////////////////////////////////////////////////
// Le sol

  glPushMatrix();
  ground();
  glPopMatrix();

//////////////////////////////////////////////////
// La scène



//////////////////////////////////////////
// le ring
//...





//////////////////////////////////////////

//////////////////////////////////////////
// les cordes
//...





//////////////////////////////////////////


//////////////////////////////////////////////////
// Le robot

// la base
glTranslatef(base_x,1,base_y);

glRotatef(base_rot,0,1,0);
base();
//..


//angle+=0.08;
// le bras
glRotatef(first_x,1,0,0);
glRotatef(first_y,0,0,1);
first_arm();

glTranslatef(0,4.0,0);

glRotatef(second_x,1,0,0);
glRotatef(second_y,0,0,1);
second_arm();

glTranslatef(0,4.0,0);

glRotatef(third_x,1,0,0);
glRotatef(third_y,0,0,1);
third_arm();

glTranslatef(0,4.0,0);

finger_1();
finger_2();


//..

//////////////////////////////////////////////////

  // Permutation des buffers et rafréchissement de l'image
   glutSwapBuffers();
   glutPostRedisplay();
}

/* Fonction de gestion du clavier */
void keyPressed(unsigned char key, int x, int y)
{

    if (key == ESCAPE)
    {
	/* Eteindre la fenêtre */
	glutDestroyWindow(window);

	/* Sortire du programme */
	exit(0);
    }
}



/* Défintion de la fonction IDLE */
void idle_function()
{

double incr = 0.05;

// incrémentnation des rotations
//..


}


int main(int argc, char **argv)
{
  // Pointeurs vers l'application
  glutInit(&argc, argv);

  /*  Activation des buffers :
     Double buffer
     RGBA color
     Alpha
     Depth buffer */
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

  /* Création de la fenêtre */
  glutInitWindowSize(1024, 765);

  /* Positionnement de la fenêtre */
  glutInitWindowPosition(200, 200);

  /* Ouverture de la fenêtre */
  window = glutCreateWindow("TD Animation 3D");

  /* Spécification de la fontion de dessin */
  glutDisplayFunc(DrawGLScene);

  /* Spécification de la routine de fond */

  glutIdleFunc(idle_function);

  /* Spécification de la fontion de redimensionnement */
  glutReshapeFunc(ReSizeGLScene);

  /* Spécification de la fontion de de gestion du clavier */
  glutKeyboardFunc(Keyboard_key);

  /* Spécification de la fontion special de gestion du clavier */

  glutSpecialFunc(Special_key);

  /* Intitialisation des paramètres de l'affichage et de la fenêtre */
  InitGL(640, 480);

  /* Lancement de la boucle OpenGL */
  glutMainLoop();

  return 1;
}



void Special_key(int key, int x, int y)
{

// Interaction avec la base

  switch (key)
  {


	//3nd Arm

	case GLUT_KEY_RIGHT:
	third_x+=0.8f;
	break;

	case GLUT_KEY_LEFT:
	third_x-=0.8f;
	break;

	case GLUT_KEY_DOWN:
	third_y+=0.8f;
	break;

	case GLUT_KEY_UP:
	third_y-=0.8f;
	break;
	
//	case ...:
//	...
//		break;

//..

        default:
        break;
  }



   glutSwapBuffers();
   glutPostRedisplay();
}

void Keyboard_key(unsigned char key, int x, int y)
{

  switch (key)
  {

///////////////////////////////////////////
// Interaction avec le bras

//	case '...':

//base
	case 's':
	base_y+=0.8f;
	break;

	case 'z':
	base_y-=0.8f;
	break;

	case 'd':
	base_x+=0.8f;
	break;

	case 'q':
	base_x-=0.8f;
	break;
	
	case 'a':
	base_rot+=1.0f;
	break;

	case 'e':
	base_rot-=1.0f;
	break;
	// 1st Arm

	case 'f':
	first_x-=0.8f;
	break;

	case 'h':
	first_x+=0.8f;
	break;

	case 't':
	first_y-=0.8f;
	break;

	case 'g':
	first_y+=0.8f;
	break;

	//2nd Arm

	case 'j':
	second_x-=0.8f;
	break;

	case 'l':
	second_x+=0.8f;
	break;

	case 'i':
	second_y-=0.8f;
	break;

	case 'k':
	second_y+=0.8f;
	break;
	
	case ' ':
	first_x = first_y = second_x = second_y = third_x = third_y = 0.0f;
	base_rot = 90.0f;
	break;
	
	
	case 'c':
	finger_x+= 0.9f;
	break;
	
	case 'w':
	finger_x-= 0.9f;
	break;





//		...
//		...
//		break;

//
//
//
//


//////////////////////////////////////////////////
// Navigation avec la caméra


// ..

//////////////////////////////////////////////////

        case ESCAPE :
               {
	       glutDestroyWindow(window);
	       exit(0);
               }

        default:
        break;
  }

   glutPostRedisplay();
   glutSwapBuffers();

}


void ground()
{

  glDisable(GL_LIGHTING);
  glColor3f(0.5,0.5,0.0);
  glLineWidth(1.0);


	// Draw a 1x1 grid along the X and Z axis'
        float i;
	for( i = -50; i <= 50; i += 5)
	{
		// Start drawing some lines
		glBegin(GL_LINES);

			// Do the horizontal lines (along the X)
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);

			// Do the vertical lines (along the Z)
			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);

		// Stop drawing lines
		glEnd();
	}

  glEnable(GL_LIGHTING);
}

void SetMaterial(GLfloat spec[], GLfloat amb[], GLfloat diff[], GLfloat shin[])
{
  glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
  glMaterialfv(GL_FRONT, GL_SHININESS, shin);
  glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
}

void base()
{
	SetMaterial(mat_specularDarkBLUE, mat_ambientDarkBLUE, mat_diffuseDarkBLUE, mat_shininessDarkBLUE);

	glPushMatrix();

		glPushMatrix();

		glRotatef(90.0,1.0,0.0,0.0); // roatation autour de l'axe x : L'axe z devient y  et l'axe y devient z

		GLUquadricObj *p;
		p = gluNewQuadric();
		gluCylinder(p, 3.0, 3.0,1.0, 30, 30);
		gluDisk(p, 0.0,3.0, 50, 5);
				
		axis_rotation(0.0,0.0,axis); // Donc l'axe de la base ici est z ( mais en réalité y )
		
		glPopMatrix();
        glTranslatef(0.0,0.0,0.0);
		gluSphere(p, 1.0, 30, 30);

        gluDeleteQuadric(p);

	glPopMatrix();
}


void first_arm()
{
	glPushMatrix();

		SetMaterial(mat_specularBLUE, mat_ambientBLUE, mat_diffuseBLUE, mat_shininessBLUE);

		axis_rotation(0.0, 0.0, axis);
		axis_rotation(axis, 0.0, 0.0);
		glTranslatef(0.0,2.0,0.0); // 1/2 hauteur du segment

		glPushMatrix();
		glScalef(1.2,4.0,1.2);
		glutSolidCube(1);
		glPopMatrix();

        glTranslatef(0.0,2.0,0.0);
        glutSolidSphere(1.0,20,20);

	glPopMatrix();
}



void second_arm()
{
//..

glPushMatrix();

		SetMaterial(mat_specularBLUE, mat_ambientBLUE, mat_diffuseBLUE, mat_shininessBLUE);

		axis_rotation(0.0, 0.0, axis);
		axis_rotation(axis, 0.0, 0.0);
		
		glTranslatef(0.0,2.0,0.0); // 1/2 hauteur du segment

		glPushMatrix();
		glScalef(0.8,4.0,0.8);
		glutSolidCube(1);
		glPopMatrix();

                glTranslatef(0.0,2.0,0.0);
                glutSolidSphere(1.0,20,20);

        glPopMatrix();

}




void third_arm()
{
//..

glPushMatrix();

		SetMaterial(mat_specularBLUE, mat_ambientBLUE, mat_diffuseBLUE, mat_shininessBLUE);

		axis_rotation(0.0, 0.0, axis);
		axis_rotation(axis, 0.0, 0.0);
		
		glTranslatef(0.0,2.0,0.0); // 1/2 hauteur du segment

		glPushMatrix();
		glScalef(0.6,4.0,0.6);
		glutSolidCube(1);
		glPopMatrix();

                glTranslatef(0.0,2.0,0.0);
                glutSolidSphere(1.0,20,20);

glPopMatrix();

}



void finger_1()
{
//..

glPushMatrix();
glTranslatef(0.8,0.0,0.0);
glRotatef(finger_x,0,0,1);
axis_rotation(0.0, 0.0, axis);
glPushMatrix();

	SetMaterial(mat_specularBLUE, mat_ambientBLUE, mat_diffuseBLUE, mat_shininessBLUE);
	glTranslatef(0.0,1.0,0.0);
	
		glPushMatrix();	
			glTranslatef(0.0,-1.0,0.0);
			
		glPopMatrix();
		
		glPushMatrix();
			glScalef(0.2,1.0,0.2);
			glutSolidCube(1);
		glPopMatrix();
                
		glTranslatef(-0.3,1.0,0.0);
                
		glPushMatrix();
			glRotatef(32,1,0,25);
			glScalef(0.2,1.0,0.2);
			glutSolidCube(1);                
		glPopMatrix();
    
glPopMatrix();
glPopMatrix();
}


void finger_2()
{
//..

glPushMatrix();
glTranslatef(-0.8,0.0,0.0);
glRotatef(-finger_x,0,0,1);
axis_rotation(0.0, 0.0, axis);
glPushMatrix();

	SetMaterial(mat_specularBLUE, mat_ambientBLUE, mat_diffuseBLUE, mat_shininessBLUE);
	glTranslatef(0.0,1.0,0.0);
    
		glPushMatrix();	
			glTranslatef(0.0,-1.0,0.0);
			
		glPopMatrix();
		
		glPushMatrix();
			glScalef(0.2,1.0,0.2);
			glutSolidCube(1);
		glPopMatrix();
                
		glTranslatef(0.3,1.0,0.0);
                
		glPushMatrix();
			glRotatef(-32,1,0,25);
			glScalef(0.2,1.0,0.2);
			glutSolidCube(1);                
		glPopMatrix();
    
glPopMatrix();
glPopMatrix();
}



void roue()
{
	glPushMatrix();

	SetMaterial(mat_specularGREEN, mat_ambientGREEN, mat_diffuseGREEN, mat_shininessGREEN);
	GLUquadricObj *p;
	p = gluNewQuadric();
	gluCylinder(p, 1.0, 1.0,1.0, 10, 10);
	gluDisk(p, 0.0,1.0, 10, 5);
        gluDeleteQuadric(p);

	glPopMatrix();
}


void move_camera(double speed)
{
//..

}


void rotate_camera(double speed)
{
//..

}

void axis_rotation(float axe_x,float axe_y, float axe_z){
	
	glPushMatrix();	
	glLineWidth(2.0);	
	
	glDisable(GL_LIGHTING);
	glColor3f(0.0,1.0,0.0);
	
	glBegin(GL_LINES);
	glVertex3f(-axe_x, -axe_y, -axe_z);
	glVertex3f(axe_x, axe_y, axe_z);
	glEnd();
	glPopMatrix();
	
	glEnable(GL_LIGHTING);
}
