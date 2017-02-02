////////////////////////////////
// 2017			      //
// TD Animation 3D            //
// Université Paris 11	      //  
// Mehdi AMMI - ammi@limsi.fr //
////////////////////////////////

#include <GL/glut.h>    // Header pour GLUT 
#include <GL/gl.h>	// Header pour OpenGL
#include <GL/glu.h>	// Header pour GLu
#include <stdlib.h>     // Heard  Utilitaire général
#include <stdio.h>      // Header pour les fonctions entrées/sorties
#include <math.h>       // Header pour les fonctions mathèmatiques
#include "TD_Animation_3D.h"

// g++ -o TD_ANIMATION TD_animation_3D.c -lGL -lGLU -lglut
/* code ASCII pour la touche escape*/
#define ESCAPE 27

/* Idantifiant de la fenêtre GLUT */
int window; 

/* Fonction d'initialisation */
void InitGL(int Width, int Height)	        
{
// Couleur d'effacement du buffer de couleur
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		

// paramètrage du Z-buffer
  glClearDepth(1.0);	
  glDepthFunc(GL_LESS);	
  glEnable(GL_DEPTH_TEST);	


// Activation du lissage
  glShadeModel(GL_SMOOTH);			

// Projection perceptive
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				

// Projection orthogonale sans aspect ratio
  glOrtho(0.0f,(GLfloat)Width,0.0f,(GLfloat)Height,-25,25);  

  glMatrixMode(GL_MODELVIEW);

// Compilation des lists des objets crées : axex, segments, etc.
  Objects_List();


// Initialisation du système cinématique
  InitBonesystem();


// Intialisation de sequence d'animation
  Init_Keyframing();

}

/* Fonction de redimensionnement de la fenêtre */
void ReSizeGLScene(int Width, int Height)
{

   glViewport(0, 0, Width, Height);		

// Projection perceptive
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();				


// Projection orthogonale avec aspect ratio
   glOrtho(0.0f,(GLfloat)Width,0.0f,(GLfloat)Height,-25,25); 


// Sauvegarde (variables globales) de la taille de la fenêtre
   m_Width = Width;
   m_Height = Height;


// Retour à la modélisation
   glMatrixMode(GL_MODELVIEW);

}

/* Focntion de dessin */
void DrawGLScene()
{
// Effacement du buffer de couleur et de profondeur
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
  glLoadIdentity();		


// Dessin du robot
//..
	glPushMatrix();
	glTranslatef(400,300,0);
	
// UpArm 	
	glTranslatef(m_UpArm.trans.x,m_UpArm.trans.y,m_UpArm.trans.z);
	glRotatef(m_UpArm.rot.z, 0,0,1);		
	glCallList(UpArm_DLIST);	
	glCallList(AXIS_DLIST);				
//.. 
// LowArm
	glTranslatef(m_LowArm.trans.x,m_LowArm.trans.y,m_LowArm.trans.z);
	glRotatef(m_LowArm.rot.z,0,0,1);
	glCallList(LowArm_DLIST);
	glCallList(AXIS_DLIST);
//..
// Effector
  	glTranslatef(m_Effector.trans.x, m_Effector.trans.y, m_Effector.trans.z);
	glCallList(AXIS_DLIST);
	glPopMatrix();

//..
// Dessin des Keyframes
//..


  // Permutation des buffers
  glutSwapBuffers();
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


// Fonction de sauvegarde des états de la souris et du système cinématique au moment du clique
void processMouse(int button, int state, int x, int y)
{
	// Sauvegarde du bouton (droit, gauche, milieu)
	m_boutton = button;

	// Sauvegarde de la position de la souris et de l'orientation des segment pour la gestion continue des angles
	//..
	m_mousepos_x = x;
	m_Grab_UPArm_Rot_Z = m_UpArm.rot.z;
	m_Grab_LowArm_Rot_Z = m_LowArm.rot.z;
}


// Fonction d'interaction : choix de l'opération à faire (cinématique directe / inverse)
void processMouseActiveMotion(int x, int y)
{
			
	switch (m_boutton)
	{

// Cinématique inverse
	case GLUT_LEFT_BUTTON : // Manipulation par cinématique inverse		
		if(ComputeIK(x,y));
		//..
		break;


// Cinématique directe
	case GLUT_MIDDLE_BUTTON : // Manipulation directe du segment UpArm
		m_UpArm.rot.z = m_Grab_UPArm_Rot_Z + (float)ROTATE_SPEED * (m_mousepos_x - x);
		//..
		break;

	case GLUT_RIGHT_BUTTON : // Manipulation directe du segment LowArm
		m_LowArm.rot.z = m_Grab_LowArm_Rot_Z + (float)ROTATE_SPEED * (m_mousepos_x - x);
		//..
		break;
	}
	DrawGLScene();
}



int main(int argc, char **argv) 
{
  /* Initialize GLUT state - glut will take any command line arguments that pertain to it or 
     X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */  
  glutInit(&argc, argv);

  /*  Activation des buffers :   
     Double buffer 
     RGBA color
     Alpha
     Depth buffer */  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  

  /* Création de la fenêtre */
  glutInitWindowSize(800, 600);  

  /* Positionnement de la fenêtre */
  glutInitWindowPosition(200, 200);  

  /* Ouverture de la fenêtre */  
  window = glutCreateWindow("TD Animation 3D");  

  /* Spécification de la fontion de dessin */
  glutDisplayFunc(&DrawGLScene);  

  /* Spécification de la fontion de redimensionnement */
  glutReshapeFunc(&ReSizeGLScene);

  /* Spécification de la fontion de  gestion du clavier */
  glutKeyboardFunc(&keyPressed);

  /* Spécification de la fontion de la souris : boutons appuyés */
  glutMouseFunc(processMouse);

  /* Spécification de la fontion de la souris : boutons appuyés avec mouvement */
  glutMotionFunc(processMouseActiveMotion);

  /* Spécification de la fontion gestion de l'animation */
  glutIdleFunc(Idle);

  /* Intitialisation des paramètres de l'affichage et de la fenêtre */
  InitGL(800, 600);
  
  /* Lancement de la boucle OpenGL */  
  glutMainLoop();  

  return 1;
}





// Listes des objets à dessiner
void Objects_List()
{

// Liste Axes
glNewList(AXIS_DLIST,GL_COMPILE);

	glPushMatrix();
	glTranslatef(0.0,0.0,1.0);
	glScalef(200.0,200.0,200.0);

	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);	
		glVertex3f(-0.2f,  0.0f, 0.0f);
		glVertex3f( 0.2f,  0.0f, 0.0f);
		glVertex3f( 0.2f,  0.0f, 0.0f);	
		glVertex3f( 0.15f,  0.04f, 0.0f);
		glVertex3f( 0.2f,  0.0f, 0.0f);	
		glVertex3f( 0.15f, -0.04f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);	
		glVertex3f( 0.0f,  0.2f, 0.0f);
		glVertex3f( 0.0f, -0.2f, 0.0f);			
		glVertex3f( 0.0f,  0.2f, 0.0f);	
		glVertex3f( 0.04f,  0.15f, 0.0f);
		glVertex3f( 0.0f,  0.2f, 0.0f);	
		glVertex3f( -0.04f,  0.15f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);	
		glVertex3f( 0.0f,  0.0f,  0.2f);
		glVertex3f( 0.0f,  0.0f, -0.2f);
		glVertex3f( 0.0f,  0.0f, 0.2f);	
		glVertex3f( 0.0f,  0.04f, 0.15f);
		glVertex3f( 0.0f, 0.0f, 0.2f);	
		glVertex3f( 0.0f, -0.04f, 0.15f);
	glEnd();
	glPopMatrix();

glEndList();


// Liste UpArm
glNewList(UpArm_DLIST,GL_COMPILE);

	glPushMatrix();

		glTranslatef(UpArm_Length/2.0,0.0,0.0);
		glScalef(UpArm_Length,Arm_Width ,1.0);
		glColor3f(0.6,0.6,0.6);
		glutSolidCube(1.0);

	glPopMatrix();

glEndList();


// Liste LowArm
glNewList(LowArm_DLIST,GL_COMPILE);

	glPushMatrix();

		glTranslatef(LowArm_Length/2.0,0.0,0.0);
		glScalef(LowArm_Length,Arm_Width ,1.0);
		glColor3f(0.8,0.8,0.8);
		glutSolidCube(1.0);

	glPopMatrix();

glEndList();

}

// Fonction d'initilisation du système cinématique
void ResetBone(t_Bone *bone, float ox, float oy, float oz, float tx, float ty, float tz)
{
	(*bone).rot.x = ox;
	(*bone).rot.y = oy;
	(*bone).rot.z = oz;
	
	(*bone).trans.x = tx;
	(*bone).trans.y = ty;
	(*bone).trans.z = tz;
//..
}


// Initilisation du système cinématique
void InitBonesystem()
{
//..
	ResetBone(&m_Body, 0, 0, 0, 0, 0, 0);
	ResetBone(&m_UpArm, 0, 0, 0, 0, 0, 0);
	ResetBone(&m_LowArm, 0, 0, 0, UpArm_Length, 0, 0);
	ResetBone(&m_Effector, 0, 0, 0, LowArm_Length, 0, 0);	
}


// Fonction de calcul de la cinématique inverse : Résultat vrais ou faux en fonction de l'objectif donnée (accessible ou non)
int ComputeIK(int x, int y)
{
/// Variables locales
  float ex,ey;		// Vecteur déplacement
  float sin2,cos2;	// SINE ry COSINE de l'ANGLE 2
  float angle1,angle2;  // ANGLE 1 et 2 en RADIANS
  float tan1;		// TAN de ANGLE 1


// Changement de repère (inversion de l'axe Y)
  y = m_Height - y - 1;

//..
  ex = x - m_UpArm.trans.x;
  ey = y - m_UpArm.trans.y;
   
  cos2 = (pow(ex,2) + pow(ey,2) - pow(UpArm_Length,2) - pow(LowArm_Length,2) )/ (2*UpArm_Length*LowArm_Length);
  
  if (cos2 >2 && cos2 <-2) return 0; 
	
  angle2 = acos (cos2);

  sin2 = sin (angle2);

  tan1 = (-(LowArm_Length*sin2*ex) + ((UpArm_Length+(LowArm_Length * cos2))*ey) ) / ((LowArm_Length*sin2*ey) + (UpArm_Length+(LowArm_Length*cos2))*ex);

  angle1 = atan (tan1);
	
  m_UpArm.rot.z = RADTODEG(angle1);

  m_LowArm.rot.z = RADTODEG(angle2);	
  return 1;	
}


// Fonction gérant le Keyframing
void Idle()
{

// Incrémentation de la varaible temps (si l'animation et trop rapide diminuer le pas d'incrémentation)
  time+=0.02;

// Variables intérmédiares entre la fonction de Keyframing et la fonction de cinématique inverse
  int X,Y,Z;

// Intérpolation linéaire
//..

// Interpolation Hermité
// ..


//..

}


void Init_Keyframing()
{

// vairialbe temps
time = 0;

// Paramètes de l'interpolation
float tension = 0.0;
float continuity = 0.0;
float bias = 0.0;

// Keyframe 1
//..

// Keyframe 2
//..

// Keyframe 3
//..

// Keyframe 4
//..

// Keyframe 5
//..

}


void Draw_animation_sequence()
{

//..

}

// Fonction d'intérpolation linéaire
void SolveLinear(float t, int *x, int *y, int *z)
{

// Déclaration des Keyframes utilisés
Key* CurKey, *NextKey;

// Varaible d'incrémentation
int i ;

// Taille du tableau de Keyframe
const int NumKeys = ((double)sizeof(TabKey))/((double)sizeof(Key));//TabKey.Count();
const int NumKeysMinusOne = NumKeys-1;


// Boucle de parcours des Keyframes
//..

 
}


//Fonction H0 : H0 = 2t3 - 3t2 + 1
float H0(float t)
{
        //..
}

//Fonction H1 : H1 = -2t3 + 3t2
float H1(float t)
{
        //..
}

//Fonction H2 : H2 = t3 - 2t2 +t 
float H2(float t)
{
        //..
}

//Fonction H3 : H3 = t3 - t2
float H3(float t)
{
       //..
}



// Fonction d'intérpolation Hermite
void SolveTCB ( float t, int *x, int *y, int *z)
{
// Déclaration des Keyframes utilisés
Key *NextKey, *NextNextKey, *CurKey, *PrevKey;

// Varaible d'incrémentation
int i;

// Taille du tableau de Keyframe
const int NumKeys = ((double)sizeof(TabKey))/((double)sizeof(Key));
const int NumKeysMinusOne = NumKeys-1;

 
// Boucle de parcours des Keyframes
//..


}
