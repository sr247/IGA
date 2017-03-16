////////////////////////////////
// 2007			      //
// TD Informatique Graphique  //
// Université Paris 11	      //  
// Mehdi AMMI - ammi@limsi.fr //
////////////////////////////////

#include <GL/glew.h>   // Headeur pour GLEW
#include <GL/glut.h>    // Header pour GLUT 
#include <GL/gl.h>	// Header pour OpenGL
#include <GL/glu.h>	// Header pour GLu
#include <stdlib.h>     // Header utilitaire général
#include <math.h>       // Header pour les fonctions mathèmatiques
#include <string.h>
#include <vector>
#include <random>
//DEBUG
#include <unistd.h>

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;
#include "common/objloader.hpp"
#include "common/texture.hpp"

/* Définition des paramètes dynamiques */
#define G 20*9.8 // m/s^2
#define B 0.6 // Damping
#define N_S 200 // Nombre de sphères
#define N_B 1 // Nombre de boites
#define R 1.0 // Rayons des sphères

/* Source de lumière */
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 0.0};   /*  composante diffuse */
GLfloat light_position[] = {0.0,10.0,10.0, 0.0};  /*  position */

/* Definitions des matériaux */
GLfloat mat_specularWHITE[] ={255.0,255.0,255.0,1.0};
GLfloat mat_ambientWHITE[] ={255.0,255.0,255.0,1.0};
GLfloat mat_diffuseWHITE[] ={255.0,255.0,255.0,1.0};
GLfloat mat_shininessWHITE[] ={128.0 * 0.4};

GLfloat mat_specularGRAY[] ={0.75,0.75,0.75,1.0};
GLfloat mat_ambientGRAY[] ={0.5,0.5,0.5,1.0};
GLfloat mat_diffuseGRAY[] ={0.50,0.50,0.50,1.0};
GLfloat mat_shininessGRAY[] ={128.0 * 0.6};

GLfloat mat_specularBLUE[] ={0.75,0.75,0.75,1.0};
GLfloat mat_ambientBLUE[] ={0,0,1,1.0};
GLfloat mat_diffuseBLUE[] ={0.50,0.50,0.50,1.0};
GLfloat mat_shininessBLUE[] ={128.0 };

GLfloat mat_specularGREEN[] ={0.633, 0.727811, 0.633,1.0};
GLfloat mat_ambientGREEN[] ={0.0215, 0.1745, 0.0215,1.0};
GLfloat mat_diffuseGREEN[] ={0.07568, 0.61424, 0.07568,1.0};
GLfloat mat_shininessGREEN[] ={128.0};

GLfloat mat_specularYELLOW[] ={0.75,0.75,0.75,1.0};
GLfloat mat_ambientYELLOW[] ={1,1,0,1.0};
GLfloat mat_diffuseYELLOW[] ={0.50,0.50,0.50,1.0};
GLfloat mat_shininessYELLOW[] ={128.0};

GLfloat mat_specularRED[] ={0.75,0.75,0.75,1.0};
GLfloat mat_ambientRED[] ={1.0,0.0,0.0,1.0};
GLfloat mat_diffuseRED[] ={0.8,0.50,0.50,1.0};
GLfloat mat_shininessRED[] ={128.0};

/* Variables d'animation */
float up_down = 0.0, left_right = -1.57;
float cam_pos_x = 0.0;
float cam_pos_z = 50.0;
float cam_pos_y = 100.0;
float cam_look_x = 0.0;
float cam_look_z = 0.0;
float vect_x = 0.0;
float vect_z = 0.0;
double d = 30;

// Postion des sphères : t & t-1
float sphere_p[3][N_S];
float sphere_ip[3][N_S];
// Vitesse des sphères : t & t-1
float sphere_v[3][N_S];
float sphere_iv[3][N_S];
// accélération des sphères
float sphere_a[3][N_S];
float sphere_ia[3][N_S];

// Postion des boites : t & t-1
float box_p[3][N_B];
float box_ip[3][N_B];
// Vitesse des boites : t & t-1
float box_v[3][N_B];
float box_iv[3][N_B];
// accélération des boites
float box_a[3][N_B];
float box_ap[3][N_B];

// Pas de temps
double dt = 0.0092;

/* code ASCII pour la touche escape*/
#define ESCAPE 27

/* Idantifiant de la fenêtre GLUT */
int window; 

/* Déclaration des en-têtes */
void Draw3DSGrid();
void rotate_camera(double speed);
void move_camera(double speed);
void InitDynamicParam();
void InitGL(int Width, int Height);
void ReSizeGLScene(int Width, int Height);
void DrawGLScene();
void keyPressed(unsigned char key, int x, int y);
void Special_key(int key, int x, int y);
void mouseMotion(int x, int y);
void CDFunction();


/* Eléments constituants le model*/
extern std::string gl_cross;
bool res = false;
bool loadmodel = false;
/// GLFW/GLEW
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals; // Won't be used at the moment.
GLuint vertexbuffer;
GLuint uvbuffer;
GLuint VertexArrayID;
GLuint Texture;
/// GLUT
GLuint objet;
std::string fileObj("model/cube.obj");
bool DrawBox = false;

/* Fonction d'initialisation */
void InitGL(int Width, int Height)	        
{
	// Couleur d'effacement du buffer de couleur
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		

	// Paramètrage du Z-buffer
	glClearDepth(1.0);	
	glDepthFunc(GL_LESS);	
	glEnable(GL_DEPTH_TEST);	

	// Activation de l'éclairage
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	// Activation du lissage
	glShadeModel(GL_SMOOTH);			

	// Projection perceptive
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,500.0f);	

	glMatrixMode(GL_MODELVIEW);

	// Initialisation des paramètres dynamiques
	InitDynamicParam();
  
	if(loadmodel){	  
		glGenVertexArrays((GLsizei)1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
  
		// Chargements des modeles
		// Read our .obj file
		res = loadOBJ(fileObj.c_str(), vertices, uvs, normals);	
	
		// Load it into a VBO
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		
		// Texture = loadDDS("model/cube.DDS");
		// Texture = loadBMP_custom("model/cube.DDS");
		
	}
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
  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,500.0f);	
  glMatrixMode(GL_MODELVIEW);
}

/* Focntion de dessin */
void DrawGLScene()
{		
	// Effacement du buffer de couleur et de profondeur
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	glLoadIdentity();		
	gluLookAt(cam_pos_x,cam_pos_y,cam_pos_z+50,cam_look_x,0.0,cam_look_z,0.0,1.0,0.0); 
	
	// Dessin de la grille
	glPushMatrix();
		Draw3DSGrid();
	glPopMatrix();
	
	// Affichage des sphères
	for(int i = 0; i < N_S ; i++){
		glPushMatrix();
		glTranslatef(sphere_p[0][i],sphere_p[1][i],sphere_p[2][i]);
		glutSolidSphere(R,50,50);
		glPopMatrix();
	}
	
	glPushMatrix();
	glTranslatef(box_p[0][0],box_p[1][0],box_p[2][0]);
	glBegin(GL_QUADS);
  
	//Face 1  
	glColor3f(1.0,1.0,0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
  
	glVertex3f(3.0f,0.0f,0.0f);
	glVertex3f(3.0f,3.0f,0.0f);
 
	glVertex3f(0.0f,3.0f,0.0f);  


	// Face 2
	glColor3f(1.0,0.0,0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
  
	glVertex3f(0.0f,0.0f,3.0f);
  
	glVertex3f(0.0f,3.0f,3.0f);
 
	glVertex3f(0.0f,3.0f,0.0f);  
  

	// Face 3
	glColor3f(0.0,0.0,1.0);
	glVertex3f(0.0f, 0.0f, 3.0f);
  
	glVertex3f(0.0f,3.0f,3.0f);
    
	glVertex3f(3.0f,3.0f,3.0f);

	glVertex3f(3.0f,0.0f,3.0f);  


	// Face 4
 
	glColor3f(0.0,1.0,1.0);
	glVertex3f(3.0f,0.0f,3.0f);  
  
	glVertex3f(3.0f,0.0f,0.0f);
    
	glVertex3f(3.0f,3.0f,0.0f);
  
	glVertex3f(3.0f,3.0f,3.0f);  
	glNormal3f(0.0,0.0,1.0);
  
 
	// Face 5
	glColor3f(0.0,1.0,0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
  
	glVertex3f(3.0f,0.0f,0.0f);
  
	glVertex3f(3.0f,0.0f,3.0f);
  
	glVertex3f(0.0f,0.0f,3.0f);


	// Face 6
	glVertex3f(0.0f, 3.0f, 0.0f);
  
	glVertex3f(3.0f,3.0f,0.0f);
  
	glVertex3f(3.0f,3.0f,3.0f);
  
	glVertex3f(0.0f,3.0f,3.0f);
 
	glEnd();
	glPopMatrix();
  
	if(res){  
	  if(gl_cross == "glfw/glew"){
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		
	}
	if(gl_cross == "glut"){
		
		//GLuint textureID;
		//glGenTextures(1, &textureID);
		glEnable(GL_TEXTURE_2D) ;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, Texture->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		// write(1, "On passe par ici !\n", sizeof("On passe par ici !\n"));
		glPushMatrix();
		glTranslatef(0,20.0,0);
		glCallList(objet);
		glPopMatrix();
		
    }
	}else{
		write(1, "Failed to draw model\n", 21);
	}
	
	// Permutation des buffers
	glutPostRedisplay();
	glutSwapBuffers();
}

void mouseMotion(int x, int y){
	box_p[0][0] = float(x)/glutGet(GLUT_WINDOW_WIDTH);
	box_p[1][0] = -float(y)/glutGet(GLUT_WINDOW_HEIGHT) ;
}

/* Fonction de gestion du clavier */
void keyPressed(unsigned char key, int x, int y) 
{
		// Press 
	    if (key == 'a'){		
			left_right-= 0.03;
			rotate_camera(left_right);
		}
		if (key == 'e'){		
			left_right+= 0.03;
			rotate_camera(left_right);
		}
	    if (key == 'z'){		
			move_camera(0.02);
		}
	    if (key == 's'){		
			move_camera(-0.02);
		}
		if (key == 'q'){		
			move_camera(0.02);
		}
	    if (key == 'd'){		
			move_camera(-0.02);
		}
		if (key == '+'){		
			cam_pos_y-= 1.0;
		}
		if (key == '-'){		
			cam_pos_y+= 1.0;
		}
		
    if (key == ESCAPE) 
    { 
	/* Eteindre la fenêtre */
		if(gl_cross == "glfw/glew"){
			glDeleteBuffers(1, &vertexbuffer);
			glDeleteBuffers(1, &uvbuffer);
			glDeleteVertexArrays(1, &VertexArrayID);
		}
		glDeleteTextures(1, &Texture);
		glutDestroyWindow(window); 

	/* Sortire du programme */
	exit(0);                   
    }else if(key == ' '){
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> posX(-40, 40);
		std::uniform_int_distribution<> posY(20, 40);
		std::uniform_int_distribution<> posZ(-40, 40);
		std::uniform_int_distribution<> vit(50, 100);
		for(int i = 0; i < N_S; i++){
			// Postion des sphères : t & t-1
			sphere_p[0][i] = posX(gen);
			sphere_p[1][i] = posY(gen);
			sphere_p[2][i] = posZ(gen);
		
			sphere_ip[0][i] = posX(gen);
			sphere_ip[1][i] = posY(gen);
			sphere_ip[2][i] = posZ(gen);
		
			// Vitesse des sphères : t & t-1
			sphere_v[0][i] = vit(gen);
			sphere_v[1][i] = vit(gen);
			sphere_v[2][i] = vit(gen);
			
			sphere_iv[0][i] = vit(gen);
			sphere_iv[1][i] = vit(gen);
			sphere_iv[2][i] = vit(gen);
			
			// accélération de la sphère
			sphere_a[0][i] = 0.0;
			sphere_a[1][i] = -G;
			sphere_a[2][i] = 0.0;
		
			sphere_ia[0][0] = 0.0;
			sphere_ia[1][0] = -G;
			sphere_ia[2][0] = 0.0;
		}
	}else if(key == 'b'){
		DrawBox = !DrawBox;
	}
}


/* Fonction de gestion du clavier spetial */
void Special_key(int key, int x, int y) 
{
	switch (key) 
	{
		case GLUT_KEY_RIGHT:  		
			left_right+= 0.03;
			rotate_camera(left_right);
			break; 

		case GLUT_KEY_LEFT:  		
			left_right-= 0.03;
			rotate_camera(left_right);
			break; 
	
		case GLUT_KEY_UP:  		 
			move_camera(0.02);
			break; 
        
		case GLUT_KEY_DOWN:  		    
			move_camera(-0.02);
			break;    		

		default:
			break;
	}   

   glutPostRedisplay();
   glutSwapBuffers();

}


/* Encapsulation des fonctions matériaux */
void SetMaterial(GLfloat spec[], GLfloat amb[], GLfloat diff[], GLfloat shin[])
{
  glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
  glMaterialfv(GL_FRONT, GL_SHININESS, shin);
  glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
}


/* Dessin d'une grille 3D */
void Draw3DSGrid()
{

	SetMaterial(mat_specularYELLOW, mat_ambientYELLOW, mat_diffuseYELLOW, mat_shininessYELLOW);

	// Draw a 1x1 grid along the X and Z axis'
        float i;
	for( i = -50; i <= 50; i += 5)
	{
		// Start drawing some lines
		glBegin(GL_LINES);
			//Base
			// Do the horizontal lines (along the X)
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);

			// Do the vertical lines (along the Z)
			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);
			
			if(DrawBox){
				
				// Mur gauche
				glVertex3f(-50, 0, 50);
				glVertex3f(-50, 50, 50);
							
				glVertex3f(-50, 50, 50);
				glVertex3f(-50, 50, -50);	
							
				glVertex3f(-50, 0, -50);
				glVertex3f(-50, 50, -50);
				
				// Mur Fond				
				glVertex3f(50, 0, -50);
				glVertex3f(50, 50, -50);
				
				glVertex3f(50, 50, -50);
				glVertex3f(-50, 50, -50);			
				
				glVertex3f(50, 50, -50);
				glVertex3f(-50, 50, -50);
				
				// Mur droite
				glVertex3f(50, 0, 50);
				glVertex3f(50, 50, 50);	
							
				glVertex3f(50, 50, 50);
				glVertex3f(50, 50, -50);				

				/*
				// Roof
				// Do the horizontal lines (along the X)
				glVertex3f(-50, 50, i);
				glVertex3f(50, 50, i);

				// Do the vertical lines (along the Z)
				glVertex3f(i, 50, -50);
				glVertex3f(i, 50, 50);
				
				// Left Wall
				glVertex3f(-50, 0, i);
				glVertex3f(-50, 50, i);
				
				glVertex3f(-50, i, -50);
				glVertex3f(-50, i, 50);
				
				
				// Back Wall
				glVertex3f(-50, i, -50);
				glVertex3f(50, i, -50);
				
				glVertex3f(i, 0, -50);
				glVertex3f(i, 50, -50);
				
				// Right Wall
				glVertex3f(50, 0, i);
				glVertex3f(50, 50, i);
				
				glVertex3f(50, i, -50);
				glVertex3f(50, i, 50);		
			*/	
			
			}
		// Stop drawing lines
		
	
		glEnd();
	}
}



/* Gestion du déplacement de la caméra  */
void move_camera(double speed)
{
	vect_x = cam_look_x - cam_pos_x;
	vect_z = cam_look_z - cam_pos_z;

	cam_pos_x += vect_x * speed;
	cam_pos_z += vect_z * speed;

	cam_look_x += vect_x * speed;
	cam_look_z += vect_z * speed;
}

/* Gestion de l'orientation de la caméra */
void rotate_camera(double speed)
{
	vect_x = cam_look_x - cam_pos_x;
	vect_z = cam_look_z - cam_pos_z;

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(speed);
	float sinTheta = (float)sin(speed);
 
	double new_x = sqrt(pow(vect_x,2) + pow(vect_z,2)) * cosTheta;
	double new_z = sqrt(pow(vect_x,2) + pow(vect_z,2)) * sinTheta;

	cam_look_x = cam_pos_x + new_x;
	cam_look_z = cam_pos_z + new_z;
}

void InitDynamicParam()
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(/*rd()*/1554758852); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> posX(-40, 40);
    std::uniform_int_distribution<> posY(20, 40);
    std::uniform_int_distribution<> posZ(-40, 40);
	std::uniform_int_distribution<> vit(50, 200);
	for(int i = 0; i < N_S; i++){
		// Postion des sphères : t & t-1
		sphere_p[0][i] = posX(gen);
		sphere_p[1][i] = posY(gen);
		sphere_p[2][i] = posZ(gen);

		sphere_ip[0][i] = posX(gen);
		sphere_ip[1][i] = posY(gen);
		sphere_ip[2][i] = posZ(gen);

		// Vitesse des sphères : t & t-1
		sphere_v[0][i] = vit(gen);
		sphere_v[1][i] = vit(gen);
		sphere_v[2][i] = vit(gen);
	
		sphere_iv[0][i] = vit(gen);
		sphere_iv[1][i] = vit(gen);
		sphere_iv[2][i] = vit(gen);
	
		// accélération de la sphère
		sphere_a[0][i] = 0.0;
		sphere_a[1][i] = -G;
		sphere_a[2][i] = 0.0;

		sphere_ia[0][0] = 0.0;
		sphere_ia[1][0] = -G;
		sphere_ia[2][0] = 0.0;
	}
	
	for(int i = 0; i < N_B; i++){
		// Postion des boites : t & t-1
		box_p[0][N_B] = 0.0f;
		box_p[1][N_B] = 0.0f;
		box_p[2][N_B] = 0.0f;
		
		box_ip[0][N_B] = 0.0f;
		box_ip[1][N_B] = 0.0f;
		box_ip[2][N_B] = 0.0f;
		
		// Vitesse des boites : t & t-1
		box_v[0][N_B] = 0.0f;
		box_v[1][N_B] = 0.0f;
		box_v[2][N_B] = 0.0f;
		
		box_iv[0][N_B] = 0.0f;
		box_iv[1][N_B] = 0.0f;
		box_iv[2][N_B] = 0.0f;
		
		// accélération des boites
		box_a[0][N_B] = 0.0f;
		box_a[1][N_B] = -800*G;
		box_a[2][N_B] = 0.0f;
		
		box_ap[0][N_B] = 0.0f;
		box_ap[1][N_B] = -800*G;
		box_ap[2][N_B] = 0.0f;		
	}
}

// Fonction de détection de collision
void CDFunction()
{

int i,j; // variable d'incrémentation

// Tests avec le sol
	for(i = 0; i < N_S ; i++)
	 {
	 	//on regarde la valeur de y pour chaque sphère
		if(sphere_p[1][i] < R || sphere_p[1][i] > 50)
		{
			sphere_p[1][i] = sphere_ip[1][i];
			sphere_v[1][i] = - sphere_v[1][i];
		}	
		if(sphere_p[0][i] < -50.0 || sphere_p[0][i] > 50.0)
		{
			sphere_v[0][i] = - sphere_v[0][i];
		}
		if(sphere_p[2][i] < -50.0 || sphere_p[2][i] > 50.0)
		{
			sphere_v[2][i] = - sphere_v[2][i];
		}
	 }

// Tests entre sphères
	for(i = 0; i < N_S ; i++)     
	{
		//entre sphères: différence de distance sur chaque axe
		for(j = i+1; j < N_S ; j++)	   
		{       
			if ( (sqrt( pow( ( sphere_p[0][i] - sphere_p[0][j] ),2 ) 
                     + pow( ( sphere_p[1][i] - sphere_p[1][j] ),2 ) 
                     + pow( ( sphere_p[2][i] - sphere_p[2][j] ),2 ) )) < ( 2 * R )  )
			{

			//Axe de projection
			float ProjAxis[3];
			ProjAxis[0] = (sphere_p[0][i] - sphere_p[0][j]) / (sqrt( pow( ( sphere_p[0][i] - sphere_p[0][j] ),2 ) + pow( ( sphere_p[1][i] - sphere_p[1][j] ),2 ) + pow( ( sphere_p[2][i] - sphere_p[2][j] ),2 ) ));
			ProjAxis[1] = (sphere_p[1][i] - sphere_p[1][j]) / (sqrt( pow( ( sphere_p[0][i] - sphere_p[0][j] ),2 ) + pow( ( sphere_p[1][i] - sphere_p[1][j] ),2 ) + pow( ( sphere_p[2][i] - sphere_p[2][j] ),2 ) ));
			ProjAxis[2] = (sphere_p[2][i] - sphere_p[2][j]) / (sqrt( pow( ( sphere_p[0][i] - sphere_p[0][j] ),2 ) + pow( ( sphere_p[1][i] - sphere_p[1][j] ),2 ) + pow( ( sphere_p[2][i] - sphere_p[2][j] ),2 ) ));
		
			// Projection de la vitesse courante sur l'axe de projection : produit scalaire
			float ProjVi =   (ProjAxis[0] * sphere_v[0][i]) + (ProjAxis[1] * sphere_v[1][i]) + (ProjAxis[2] * sphere_v[2][i]);
			float ProjVj =   (ProjAxis[0] * sphere_v[0][j]) + (ProjAxis[1] * sphere_v[1][j]) + (ProjAxis[2] * sphere_v[2][j]);

			// Paramètre de surface
			float K_cont = 0.5;
		
			//Calcul de la vitesse 
			sphere_v[0][i] = -K_cont * ProjAxis[0] * ProjVi;
			sphere_v[1][i] = -K_cont * ProjAxis[1] * ProjVi; 
			sphere_v[2][i] = -K_cont * ProjAxis[2] * ProjVi;
		
			sphere_v[0][j] = -K_cont * ProjAxis[0] * ProjVj;
			sphere_v[1][j] = -K_cont * ProjAxis[1] * ProjVj; 
			sphere_v[2][j] = -K_cont * ProjAxis[2] * ProjVj;
		
			//system update pour chacune des deux sphères
			sphere_p[0][i] = sphere_ip[0][i];
			sphere_p[1][i] = sphere_ip[1][i];
			sphere_p[2][i] = sphere_ip[2][i];

			sphere_p[0][j] = sphere_ip[0][j];
			sphere_p[1][j] = sphere_ip[1][j];
			sphere_p[2][j] = sphere_ip[2][j];
		  }
	  }
  }

}


/* Défintion de la fonction IDLE */
//on va d'abord commencer par cette fonction: plus simple. Et passer ensuite à la détection de collision
void idle_function()
{
int i ; // variable d'incrémentation

	////////////////////////////////////
	// Numerical integration
       for(i = 0; i < N_S ; i++)
       {
		sphere_v[0][i] = sphere_iv[0][i] + ( sphere_ia[0][i] * dt );
		sphere_v[1][i] = sphere_iv[1][i] + ( sphere_ia[1][i] * dt );
		sphere_v[2][i] = sphere_iv[2][i] + ( sphere_ia[2][i] * dt );

		sphere_p[0][i] = sphere_ip[0][i] + ( sphere_v[0][i] * dt );
		sphere_p[1][i] = sphere_ip[1][i] + ( sphere_v[1][i] * dt );
		sphere_p[2][i] = sphere_ip[2][i] + ( sphere_v[2][i] * dt );
       }
	////////////////////////////////////
	// Collision detection

        CDFunction();

	////////////////////////////////////
	// Acceleration calculation

       for(i = 0; i < N_S ; i++)
       {
		sphere_a[0][i] =      - (sphere_v[0][i] * B);

		sphere_a[1][i] =  - G - (sphere_v[1][i] * B);

		sphere_a[2][i] =      - (sphere_v[2][i] * B);
       }

	////////////////////////////////////
	// system update
       for(i = 0; i < N_S ; i++)
       {
		sphere_ia[0][i] = sphere_a[0][i];
		sphere_ia[1][i] = sphere_a[1][i];
		sphere_ia[2][i] = sphere_a[2][i];

		sphere_iv[0][i] = sphere_v[0][i];
		sphere_iv[1][i] = sphere_v[1][i];
		sphere_iv[2][i] = sphere_v[2][i];

		sphere_ip[0][i] = sphere_p[0][i];
		sphere_ip[1][i] = sphere_p[1][i];
		sphere_ip[2][i] = sphere_p[2][i];
       }
}

int main(int argc, char **argv) 
{
	write(1, argv[0], sizeof(argv[0]));
	write(1, argv[1], sizeof(argv[1]));
	if(argv[1]){
		if(strstr(argv[1],"-m") && strstr(argv[2],"true")){
			loadmodel = true;
		}else{
			loadmodel = false;
		}
		if(strstr(argv[1],".obj")){
			fileObj = argv[1];
			loadmodel = true;
		}
	}


	

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
  glutInitWindowSize(640, 480);  

  /* Positionnement de la fenêtre */
  glutInitWindowPosition(0, 0);  

  /* Ouverture de la fenêtre */  
  window = glutCreateWindow("TD Animation 3D");  

  /* Spécification de la fontion de dessin */
  glutDisplayFunc(&DrawGLScene);  

  /* Spécification de la routine de fond */

  glutIdleFunc(idle_function);

  /* Spécification de la fontion de redimensionnement */
  glutReshapeFunc(&ReSizeGLScene);

  /* Spécification de la fontion de de gestion du clavier */
  glutKeyboardFunc(&keyPressed);
  
  glutPassiveMotionFunc(mouseMotion);

  /* Spécification de la fontion special de gestion du clavier */
  glutSpecialFunc(Special_key);

  /* Intitialisation des paramètres de l'affichage et de la fenêtre */
  write(1, "Attentions ça va bug\n", 22);
  glewExperimental = true;
	glewInit();
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glutDestroyWindow(window); 
		return -1;
	}
  InitGL(640, 480);
  write(1, "Vraiment ici?\n", 21);
  
  /* Lancement de la boucle OpenGL */  
  glutMainLoop();

  return 1;
}
