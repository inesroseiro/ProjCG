
#include <stdlib.h>
#include <stdio.h>			// printf
#include "RgbImage.h"
#include <math.h>
#include <time.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>    // openGL


//--------------------------------- Definir cores
#define AZUL     0.0, 0.0, 1.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define AMARELO  1.0, 1.0, 0.0, 1.0
#define VERDE    0.0, 1.0, 0.0, 1.0
#define LARANJA  1.0, 0.5, 0.1, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.9, 0.92, 0.29, 1.0
#define PI		 3.14159

//================================================================================
//===========================================================Variaveis e constantes

//------------------------------------------------------------ Sistema Coordenadas
GLfloat   xC=15.0, yC=15.0, zC=30.0;
GLint     wScreen=800, hScreen=600;
GLfloat   mesa=3.0;
GLfloat   quad=6.0;
GLfloat   mesaP[]= {4, 0, 10};
GLfloat   quadP[]= {4, 4, 0.1};


//------------------------------------------------------------ Observador
GLint    defineView=0;
GLint    defineProj=1;
GLfloat  raio   = 20;
GLfloat  angulo = 0.35*PI;
//GLfloat  obsP[] = {10, 5.5, 10};
GLfloat  obsP[] = {raio*cos(angulo), 0.5, raio*sin(angulo)};
GLfloat  incy   = 0.5;
GLfloat  inca   = 0.03;

//------------------------------------------------------------ Texturas
GLint    repete=1;
GLint    maxR  =20;
GLint    numQuadro =5;
GLint    msec=10;					//.. definicao do timer (actualizacao)

//================================================================================
//=========================================================================== INIT

//------------------------------------------------------------ Texturas
GLuint  texture[3];
GLuint  tex;
RgbImage imag;


void criaDefineTexturas()
{   
	//----------------------------------------- Parte de cima das latas
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("lata.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	
	//----------------------------------------- Chao 
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("prate.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//----------------------------------------- Lata 1
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("oi.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	
}


void init(void)
{
	glClearColor(BLACK);
	glShadeModel(GL_SMOOTH);
	criaDefineTexturas( );
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}


void resizeWindow(GLsizei w, GLsizei h)
{
 	wScreen=w;
	hScreen=h;
	//glViewport( 0, 0, wScreen,hScreen );	
	//glutReshapeWindow(wScreen,hScreen);
	glutPostRedisplay();
}

void DrawCircle(float raio, float lineW)
{
	float rad;	 
	float angle, radian, x, y, tx, ty, xcos, ysin;
   	int i;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
   	glLineWidth(lineW);
    glBegin(GL_TRIANGLE_FAN);
	for (i=0; i<360; i++) { 
       	rad = i*PI/180.0;
		xcos = (float)cos(rad);
		ysin = (float)sin(rad);
		x = xcos * raio;
		y = ysin * raio*lineW;
		tx = xcos * 0.5 + 0.5;
		ty = ysin * 0.5 + 0.5;
		
		glTexCoord2f(tx, ty);
		glVertex2f(x, y);
		
    }
    glEnd();
	glDisable(GL_TEXTURE_2D);
}


void drawScene(){
	
	GLUquadricObj*  y = gluNewQuadric ( );
		
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Chao y=0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( -5,  0, 0 ); 
			glTexCoord2f(5.0f,0.0f); glVertex3i( xC, 0, 0 ); 
			glTexCoord2f(5.0f,10.0f); glVertex3i( xC, 0, xC/3); 
			glTexCoord2f(0.0f,5.0f); glVertex3i( -5,  0,  xC/3); 
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//==================================== Parte de baixo da lata 1 
	glPushMatrix();
			glTranslatef (3,0.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();
	//==================================== Parte de cima da lata 1 
	glPushMatrix();
			glTranslatef (3,2.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();

	//==================================== PAralelipipedo Amarelo = LATA 1
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glTranslatef (3,0.1,2);			
		glRotatef (-90, 1, 0, 0);			
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluQuadricTexture   ( y, GL_TRUE    );
		gluCylinder( y, 0.6, 0.6, 2, 100, 100);				
	glPopMatrix();

	//==================================== PAralelipipedo Amarelo = LATA 2
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glTranslatef (4.5,0.1,2);			
		glRotatef (-90, 1, 0, 0);			
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluQuadricTexture   ( y, GL_TRUE    );
		gluCylinder( y, 0.6, 0.6, 2, 100, 100);				
	glPopMatrix();

	//==================================== Parte de baixo da lata 1 
	glPushMatrix();
			glTranslatef (4.5,0.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();
	//==================================== Parte de cima da lata 1 
	glPushMatrix();
			glTranslatef (4.5,2.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();

	//==================================== PAralelipipedo Amarelo = LATA 3
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glTranslatef (6,0.1,2);			
		glRotatef (-90, 1, 0, 0);			
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluQuadricTexture   ( y, GL_TRUE    );
		gluCylinder( y, 0.6, 0.6, 2, 100, 100);				
	glPopMatrix();

	//==================================== Parte de baixo da lata 1 
	glPushMatrix();
			glTranslatef (6,0.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();
	//==================================== Parte de cima da lata 1 
	glPushMatrix();
			glTranslatef (6,2.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();

	//==================================== PAralelipipedo Amarelo = LATA 4
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glTranslatef (7.5,0.1,2);			
		glRotatef (-90, 1, 0, 0);			
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluQuadricTexture   ( y, GL_TRUE    );
		gluCylinder( y, 0.6, 0.6, 2, 100, 100);				
	glPopMatrix();

	//==================================== Parte de baixo da lata 1 
	glPushMatrix();
			glTranslatef (7.5,0.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();
	//==================================== Parte de cima da lata 1 
	glPushMatrix();
			glTranslatef (7.5,2.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();

	//==================================== PAralelipipedo Amarelo = LATA 5 - 1 2o andar
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glTranslatef (3.75,2.1,2);			
		glRotatef (-90, 1, 0, 0);			
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluQuadricTexture   ( y, GL_TRUE    );
		gluCylinder( y, 0.6, 0.6, 2, 100, 100);				
	glPopMatrix();

	//==================================== Parte de baixo da lata 1 
	glPushMatrix();
			glTranslatef (3.75,2.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();
	//==================================== Parte de cima da lata 1 
	glPushMatrix();
			glTranslatef (3.75,4.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();


	//==================================== PAralelipipedo Amarelo = LATA 6 - 2 2o andar
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glTranslatef (6.75,2.1,2);			
		glRotatef (-90, 1, 0, 0);			
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluQuadricTexture   ( y, GL_TRUE    );
		gluCylinder( y, 0.6, 0.6, 2, 100, 100);				
	glPopMatrix();

	//==================================== Parte de baixo da lata 1 
	glPushMatrix();
			glTranslatef (6.75,2.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();
	//==================================== Parte de cima da lata 1 
	glPushMatrix();
			glTranslatef (6.75,4.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();

	//==================================== PAralelipipedo Amarelo = LATA 7 - 3 2o andar
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glTranslatef (5.25,2.1,2);			
		glRotatef (-90, 1, 0, 0);			
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluQuadricTexture   ( y, GL_TRUE    );
		gluCylinder( y, 0.6, 0.6, 2, 100, 100);				
	glPopMatrix();

	//==================================== Parte de baixo da lata 1 
	glPushMatrix();
			glTranslatef (5.25,2.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();
	//==================================== Parte de cima da lata 1 
	glPushMatrix();
			glTranslatef (5.25,4.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();

	//==================================== PAralelipipedo Amarelo = LATA 2 - 3º andar
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glTranslatef (4.5,4.1,2);			
		glRotatef (-90, 1, 0, 0);			
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluQuadricTexture   ( y, GL_TRUE    );
		gluCylinder( y, 0.6, 0.6, 2, 100, 100);				
	glPopMatrix();

	//==================================== Parte de baixo da lata 1 
	glPushMatrix();
			glTranslatef (4.5,4.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();
	//==================================== Parte de cima da lata 1 
	glPushMatrix();
			glTranslatef (4.5,6.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();
	//==================================== PAralelipipedo Amarelo = LATA 2 - 3º andar
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glTranslatef (6,4.1,2);			
		glRotatef (-90, 1, 0, 0);			
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluQuadricTexture   ( y, GL_TRUE    );
		gluCylinder( y, 0.6, 0.6, 2, 100, 100);				
	glPopMatrix();

	//==================================== Parte de baixo da lata 1 
	glPushMatrix();
			glTranslatef (6,4.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();
	//==================================== Parte de cima da lata 1 
	glPushMatrix();
			glTranslatef (6,6.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();

		//==================================== PAralelipipedo Amarelo = LATA 6 - 1 4o andar
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glTranslatef (5.25,6.1,2);
		glRotatef (-90, 1, 0, 0);			
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluQuadricTexture   ( y, GL_TRUE    );
		gluCylinder( y, 0.6, 0.6, 2, 100, 100);

	glPopMatrix();

	//==================================== Parte de baixo da lata 1 
	glPushMatrix();
			glTranslatef (5.25,6.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();
	//==================================== Parte de cima da lata 1 
	glPushMatrix();
			glTranslatef (5.25,8.1,2);
			glRotatef (-90, 1, 0, 0);
			DrawCircle(0.6,1);
	glPopMatrix();


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixos
	/*glColor4f(BLACK);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0); 
		glVertex3i(10, 0, 0); 
	glEnd();
	glBegin(GL_LINES);
		glVertex3i(0,  0, 0); 
		glVertex3i(0, 10, 0); 
	glEnd();
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0); 
		glVertex3i( 0, 0,10); 
	glEnd();
	*/
	


}

void display(void){
  	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Apagar ]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Janela Visualizacao ]
	glViewport (0,0,wScreen, hScreen);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Projeccao]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (defineProj) {
		case 1: gluPerspective(88.0, wScreen/hScreen, 0.1, zC); break;
		default: glOrtho (-xC,xC,-yC,yC,-zC,zC);
			break;
	}
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Modelo+View(camera/observador) ]
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], 0,0,0, 0, 1, 0);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Objectos ]
	drawScene();
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
	glutSwapBuffers();
}


void Timer(int value) 
{
	glutPostRedisplay();
	glutTimerFunc(msec,Timer, 1);
}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y){
	
	switch (key) {
	//--------------------------- Textura no quadro
	case 'r':
	case 'R':
		glutPostRedisplay();
		break;
	//--------------------------- Textura no azulejo
	case 't':
	case 'T':
		glutPostRedisplay();
		break;
	//--------------------------- Projeccao
	case 'q':
	case 'Q':
		defineProj=(defineProj+1)%2;
		 
		break;
	//--------------------------- Escape
	case 27:
		exit(0);
		break;	
  }
}

void teclasNotAscii(int key, int x, int y){
    if(key == GLUT_KEY_UP)
		obsP[1]=obsP[1]+incy; 
	if(key == GLUT_KEY_DOWN) 
		obsP[1]=obsP[1]-incy; 
	if(key == GLUT_KEY_LEFT)
		angulo=angulo+inca; 
	if(key == GLUT_KEY_RIGHT) 
		angulo=angulo-inca; 

	if (obsP[1]> yC)
		obsP[1]= yC;
    if (obsP[1]<-yC)
		obsP[1]=-yC;
    obsP[0] = raio*cos(angulo);
	obsP[2] = raio*sin(angulo);
	
	glutPostRedisplay();	
}

//======================================================= MAIN
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (wScreen, hScreen); 
	glutInitWindowPosition (100, 100); 
	glutCreateWindow ("{jh,pjmm}@dei.uc.pt-CG ::::::::::::::: (left,right,up,down, 'q', 'r', 't)' ");
  
	init();
	
	glutSpecialFunc(teclasNotAscii); 
	glutDisplayFunc(display); 
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}

