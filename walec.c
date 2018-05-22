
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <GL/glut.h>
#include "materials.h"
#define X_OFFSET_SWIATLO    10
#define Y_OFFSET_SWIATLO    120
#define X_OFFSET_MAT        10
#define Y_OFFSET_MAT        220
#define X_OFFSET_OBIEKT     10
#define Y_OFFSET_OBIEKT     300

#define M_PI   3.14159
#define dAlfa                   (360.0/N)
#define PROMIEN                       1
// Makro przeliczajace stopnie na radiany
#define DEG2RAD(x)       ((float)(x)*M_PI/180.0)
// Zmienne globalne
#define promien       2
#define PI 3.1415926535


int N=8;  //Podzialy poziome
int N2=16; //Podzialy pionowe
double r=PROMIEN; //Promienn

//Zmienne odpowiedzialne za polozenie obserwatora
double  katY = -20.0;         
double  katRX = 0.0;         
double  katRY = 0.0;         
double  katRZ = 0.0;         
double  katX = 20.0;   
//Zmienne pomocnicze dla predkosci katowych zrodla swiatla.
double  RX = 0.0;  
double  RZ= 0.0;   
double  RY= 0.0; 

int		obsR=15;   //Odleglosc obserwatora                    
int     szerokoscOkna = 800;
int     wysokoscOkna  = 600;
int		lightSwitch[2]={0,0};
int		obrot;
float	rRef=10;
GLfloat swiatlo[7][4];
GLfloat swiatlo2[7][4];
GLfloat material[6][4];
GLfloat material2[6][4];
GLfloat braz[6][4];
int zmienna ;

void RysujTekstRastrowy(void *font, char *tekst)
{
  int i;

  for (i = 0; i < (int)strlen(tekst); i++)
    glutBitmapCharacter(font, tekst[i]);
}

void Sfera(double poziom, double pion);
void UstawDomyslneWartosciParametrow(void);
void WlaczOswietlenie(int, int);
void DefiniujMaterial(int);
void RysujWalec(double h,  int nv, int nh);
void UstawParametryWidoku(int szer, int wys);
void RysujNakladke(void);
void WyswietlObraz(void);
void ObslugaKlawiatury(unsigned char klawisz, int x, int y);
void ObslugaKlawiszySpecjalnych(int klawisz, int x, int y);
void UstawDomyslneWartosciParametrow(void)
{
	//Material pierwszy: ¿ó³ty blyszczacy
  GLfloat param_materialu[6][4] = {
{1.0, 1.0, 0.0, 1.0},  // [0] wspolczynnik odbicia swiatla otoczenia
{0.5, 0.5, 0.0, 1.0},  // [1] wspolczynnik odbicia swiatla rozproszonego
{1.0, 1.0, 0.0, 1.0},  // [2] wspolczynnik odbicia swiatla lustrzanego
{128.0, 0.0, 0.0, 0.0}, // [3] polysk 
{0.0, 0.0, 0.0, 0.0}}; // [4] kolor swiatla emitowanego
  
	//Material drugi: niebieski matowy
  GLfloat param_materialu2[6][4] = {
{0.0, 0.0, 1.0, 1.0},  // [0] wspolczynnik odbicia swiatla otoczenia
{0.0, 0.0, 0.5, 1.0},  // [1] wspolczynnik odbicia swiatla rozproszonego
{0.0, 0.0, 0.0, 1.0},  // [2] wspolczynnik odbicia swiatla lustrzanego
{0.0, 0.0, 0.0, 0.0},  // [3] polysk 
{0.0, 0.0, 0.0, 0.0}}; // [4] kolor swiatla emitowanego
					   

  GLfloat param_braz[6][4] = {
	  { 0.212500, 0.127500, 0.054000, 1.000000 },  // [0] wspolczynnik odbicia swiatla otoczenia
	  { 0.714000, 0.428400, 0.181440, 1.000000 },  // [1] wspolczynnik odbicia swiatla rozproszonego
	  { 0.393548, 0.271906, 0.166721, 1.000000 },  // [2] wspolczynnik odbicia swiatla lustrzanego
	  { 25.6, 0.0, 0.0, 0.0 },  // [3] polysk 
	  { 0.0, 0.0, 0.0, 0.0 } }; // [4] kolor swiatla emitowanego


//Swiatlo pierwsze: Reflektor
  GLfloat param_swiatla[5][4] = {
{0.0, 0.0, 0.0, 1.0},   // [0] otoczenie
{1.0, 1.0, 1.0, 1.0},   // [1] rozproszenie
{1.0, 1.0, 1.0, 1.0},   // [2] lustrzane
{0.0, 0.0, 0.0, 1.0},   // [3] polozenie
	{0.0, 0.0, 0.0, 0.0}};  // [4] kierunek swiecenia
	
	//Swiatlo drugie: Kierunkowe
  GLfloat param_swiatla2[5][4] = {
{0.0, 0.0, 0.0, 1.0},   // [0] otoczenie
{1.0, 1.0, 0.0, 0.8},   // [1] rozproszenie
{1.0, 1.0, 0.0, 0.8},   // [2] lustrzane
{10.0, 10.0, 10.0, 1.0},// [3] polozenie
	{0.0, 0.0, 0.0, 0.0}};// [4] kierunek swiecenia
	

  memcpy(material, param_materialu, 5*4*sizeof(GLfloat));
  memcpy(swiatlo, param_swiatla, 7*4*sizeof(GLfloat));  
  memcpy(material2, param_materialu2, 5*4*sizeof(GLfloat));
  memcpy(swiatlo2, param_swiatla2, 7*4*sizeof(GLfloat));  
  memcpy(braz, param_braz, 5 * 4 * sizeof(GLfloat));
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}
void WlaczOswietlenie(int s1, int s2)
{
  glEnable(GL_LIGHTING);
  if (s1==1)
  {
	  glEnable(GL_LIGHT0);
	  obrot=(obrot+5)%360;
	obrot=45;
	swiatlo[3][0]=rRef*cos(DEG2RAD(obrot));
	swiatlo[3][1]=r/2;
	swiatlo[3][2]=rRef*sin(DEG2RAD(obrot));
	swiatlo[3][3]=1.0;
	swiatlo[4][0]=-rRef*cos(DEG2RAD(obrot));
	swiatlo[4][1]=-r/3;
	swiatlo[4][2]=-rRef*sin(DEG2RAD(obrot));
	swiatlo[4][3]=-1.0;
	glLightfv(GL_LIGHT0, GL_AMBIENT, swiatlo[0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, swiatlo[1]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, swiatlo[2]);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, swiatlo[5][0]);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45); //kat odciecia
  }
  else glDisable(GL_LIGHT0);
  if (s2==1)
  { 
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, swiatlo2[0]);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, swiatlo2[1]);
	glLightfv(GL_LIGHT1, GL_SPECULAR, swiatlo2[2]);
	glLightfv(GL_LIGHT1, GL_POSITION, swiatlo2[3]);  
  }
  else glDisable(GL_LIGHT1);
}
void DefiniujMaterial(int typ) 
// Funckja umozliwiajaca wybor jednego z dwoch materialow.
{
	if(typ==4)//Wybor materialu pierwszego.
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, material[0]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material[1]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, material[2]);
		glMaterialfv(GL_FRONT, GL_SHININESS, material[3]);
		glMaterialfv(GL_FRONT, GL_EMISSION, material[4]);
	}
	if(typ==5) //Wybor materialu drugiego.
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, material2[0]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material2[1]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, material2[2]);
		glMaterialfv(GL_FRONT, GL_SHININESS, material2[3]);
		glMaterialfv(GL_FRONT, GL_EMISSION, material2[4]);
	}
	if (typ == 6)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, braz[0]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, braz[1]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, braz[2]);
		glMaterialfv(GL_FRONT, GL_SHININESS, braz[3]);
		glMaterialfv(GL_FRONT, GL_EMISSION, braz[4]);
	}
}

void piers(double N) {
	DefiniujMaterial(zmienna);
	glColor3f(0.0, 1.0, 0.0);
	//DOL
	/*glBegin(GL_QUAD_STRIP);
	for (a = 0; a*dAlfa <= 360.0; a++)
	{
	glNormal3f(cos(DEG2RAD(a*dAlfa)), 0, sin(DEG2RAD(a*dAlfa)));
	glVertex3f(0, 0, 0);
	glVertex3f(r*cos(DEG2RAD(a*dAlfa)), 0, r*sin(DEG2RAD(a*dAlfa)));

	}
	glEnd();

	//Powierzchnia boczna
	for (b = 0; b*dh <= (5.01 - dh); b++)
	{
	glBegin(GL_QUAD_STRIP);
	for (a = 0; a*dAlfa <= (360.1 - dAlfa); a++)
	{
	glNormal3f((((h - (b*dh))) / h)*cos(DEG2RAD(a*dAlfa)), b*dh, (((h - (b*dh))) / h)*sin(DEG2RAD(a*dAlfa)));
	glVertex3f(((r*(h - (b*dh))) / h)*cos(DEG2RAD(a*dAlfa)), b*dh, ((r*(h - (b*dh))) / h)*sin(DEG2RAD(a*dAlfa)));
	glNormal3f((((h - ((b + 1)*dh))) / h)*cos(DEG2RAD(a*dAlfa)), (b + 1)*dh, (((h - ((b + 1)*dh))) / h)*sin(DEG2RAD(a*dAlfa)));
	glVertex3f(((r*(h - ((b + 1)*dh))) / h)*cos(DEG2RAD(a*dAlfa)), (b + 1)*dh, ((r*(h - ((b + 1)*dh))) / h)*sin(DEG2RAD(a*dAlfa)));
	glNormal3f((((h - (b*dh))) / h)*cos(DEG2RAD((a + 1)*dAlfa)), b*dh, (((h - (b*dh))) / h)*sin(DEG2RAD((a + 1)*dAlfa)));
	glVertex3f(((r*(h - (b*dh))) / h)*cos(DEG2RAD((a + 1)*dAlfa)), b*dh, ((r*(h - (b*dh))) / h)*sin(DEG2RAD((a + 1)*dAlfa)));
	glNormal3f((((h - ((b + 1)*dh))) / h)*cos(DEG2RAD((a + 1)*dAlfa)), (b + 1)*dh, (((h - ((b + 1)*dh))) / h)*sin(DEG2RAD((a + 1)*dAlfa)));
	glVertex3f(((r*(h - ((b + 1)*dh))) / h)*cos(DEG2RAD((a + 1)*dAlfa)), (b + 1)*dh, ((r*(h - ((b + 1)*dh))) / h)*sin(DEG2RAD((a + 1)*dAlfa)));
	}
	glEnd();
	}*/
	//void RysujPierscien()
	//{
	//double dAlfa = 360.0 / (double)n;
	double i = 0, h, r;
	//int		n = 30;
	double  a = 5;
	r = 15;

	h = 5;



	// Podstawa (miêdzy pkt-mi 1-2)
	glBegin(GL_QUADS);
	for (i = 1; i <= 360; i++)
	{
		glColor3f(1.0, 0.0, 0.0);
		glNormal3f(cos(DEG2RAD(i*dAlfa)), 0.0, sin(DEG2RAD(i*dAlfa)));
		glVertex3f((r)*cos(DEG2RAD(i*dAlfa)), 0.0, (r)*sin(DEG2RAD(i*dAlfa)));


		glNormal3f((a)*cos(DEG2RAD(i*dAlfa)), 0.0, (a)*sin(DEG2RAD(i*dAlfa)));
		glVertex3f((r + a)*cos(DEG2RAD(i*dAlfa)), 0.0, (r + a)*sin(DEG2RAD(i*dAlfa)));

		glNormal3f((a)*cos(DEG2RAD((i + 1)*dAlfa)), 0.0, (a)*sin(DEG2RAD((i + 1)*dAlfa)));
		glVertex3f((r + a)*cos(DEG2RAD((i + 1)*dAlfa)), 0.0, (r + a)*sin(DEG2RAD((i + 1)*dAlfa)));

		glNormal3f(cos(DEG2RAD((i + 1)*dAlfa)), 0.0, sin(DEG2RAD((i + 1)*dAlfa)));
		glVertex3f((r)*cos(DEG2RAD((i + 1)*dAlfa)), 0.0, (r)*sin(DEG2RAD((i + 1)*dAlfa)));

	}
	// Œciana miêdzy pkt-mi 2-3
	for (i = 1; i <= 360; i++)
	{
		glColor3f(0.0, 1.0, 0.0);
		//glNormal3f((r + a)*cos(DEG2RAD(i*dAlfa)), h, (r + a)*sin(DEG2RAD(i*dAlfa)));


		glNormal3f((a)*cos(DEG2RAD(i*dAlfa)), h, (a)*sin(DEG2RAD(i*dAlfa)));
		glVertex3f((r + a)*cos(DEG2RAD(i*dAlfa)), h, (r + a)*sin(DEG2RAD(i*dAlfa)));

		glNormal3f((a)*cos(DEG2RAD((i + 1)*dAlfa)), h, (a)*sin(DEG2RAD((i + 1)*dAlfa)));
		glVertex3f((r + a)*cos(DEG2RAD((i + 1)*dAlfa)), h, (r + a)*sin(DEG2RAD((i + 1)*dAlfa)));

		glNormal3f((a)*cos(DEG2RAD((i + 1)*dAlfa)), 0.0, (a)*sin(DEG2RAD((i + 1)*dAlfa)));
		glVertex3f((r + a)*cos(DEG2RAD((i + 1)*dAlfa)), 0.0, (r + a)*sin(DEG2RAD((i + 1)*dAlfa)));

		glNormal3f((a)*cos(DEG2RAD(i*dAlfa)), 0.0, (a)*sin(DEG2RAD(i*dAlfa)));
		glVertex3f((r + a)*cos(DEG2RAD(i*dAlfa)), 0.0, (r + a)*sin(DEG2RAD(i*dAlfa)));



	}
	// Œciana miêdzy pkt-mi 1-4
	for (i = 1; i <= 360; i++)
	{
		glColor3f(0.0, 1.0, 1.0);


		glNormal3f(cos(DEG2RAD(i*dAlfa)), h, sin(DEG2RAD(i*dAlfa)));
		glVertex3f((r)*cos(DEG2RAD(i*dAlfa)), h, (r)*sin(DEG2RAD(i*dAlfa)));


		glNormal3f(cos(DEG2RAD((i + 1)*dAlfa)), h, sin(DEG2RAD((i + 1)*dAlfa)));
		glVertex3f((r)*cos(DEG2RAD((i + 1)*dAlfa)), h, (r)*sin(DEG2RAD((i + 1)*dAlfa)));


		glNormal3f(cos(DEG2RAD((i + 1)*dAlfa)), 0.0, sin(DEG2RAD((i + 1)*dAlfa)));
		glVertex3f((r)*cos(DEG2RAD((i + 1)*dAlfa)), 0.0, (r)*sin(DEG2RAD((i + 1)*dAlfa)));


		glNormal3f(cos(DEG2RAD(i*dAlfa)), 0.0, sin(DEG2RAD(i*dAlfa)));
		glVertex3f((r)*cos(DEG2RAD(i*dAlfa)), 0.0, (r)*sin(DEG2RAD(i*dAlfa)));

	}
	// Œciana miêdzy pkt-mi 3-5
	for (i = 1; i <= 360; i++)
	{
		glColor3f(1.0, 0.0, 1.0);
		//glNormal3f((r)*cos(DEG2RAD(i*dAlfa)), h, (r)*sin(DEG2RAD(i*dAlfa)));
		glNormal3f(cos(DEG2RAD(i*dAlfa)), h, sin(DEG2RAD(i*dAlfa)));
		glVertex3f((r)*cos(DEG2RAD(i*dAlfa)), h, (r)*sin(DEG2RAD(i*dAlfa)));

		glNormal3f((a)*cos(DEG2RAD(i*dAlfa)), h, (a)*sin(DEG2RAD(i*dAlfa)));
		glVertex3f((r + a)*cos(DEG2RAD(i*dAlfa)), h, (r + a)*sin(DEG2RAD(i*dAlfa)));

		glNormal3f((a)*cos(DEG2RAD((i + 1)*dAlfa)), h, (a)*sin(DEG2RAD((i + 1)*dAlfa)));
		glVertex3f((r + a)*cos(DEG2RAD((i + 1)*dAlfa)), h, (r + a)*sin(DEG2RAD((i + 1)*dAlfa)));

		glNormal3f(cos(DEG2RAD((i + 1)*dAlfa)), h, sin(DEG2RAD((i + 1)*dAlfa)));
		glVertex3f((r)*cos(DEG2RAD((i + 1)*dAlfa)), h, (r)*sin(DEG2RAD((i + 1)*dAlfa)));

	}
	//glColor3f(0.0, 0.0, 0.0);
	glEnd();
}


void Sfera(double N, double N2) {
	DefiniujMaterial(zmienna);
	glColor3f(0.0, 1.0, 1.0);
	float alfa, beta, x, y, z;
	for (alfa = 0; alfa <= PI - 0.0001; alfa += PI / N2)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (beta = 0.0; beta <= 1.5001*PI; beta += 1.5*PI / N)
		{
			x = promien * sin(alfa + PI / N2) * cos(beta);
			y = promien * cos(alfa + PI / N2);
			z = promien * sin(alfa + PI / N2) * sin(beta);
			glNormal3f((sin(alfa + PI / N2) * cos(beta)), cos(alfa + PI / N2), (sin(alfa + PI / N2) * sin(beta)));
			glVertex3f(x, y, z);
			x = promien * sin(alfa) * cos(beta);
			y = promien * cos(alfa);
			z = promien * sin(alfa) * sin(beta);
			glNormal3f((sin(alfa) * cos(beta)), cos(alfa), (sin(alfa) * sin(beta)));
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}
  void UstawParametryWidoku(int szer, int wys)
{
  szerokoscOkna = szer;
  wysokoscOkna  = wys;
  glViewport(0, 0, szerokoscOkna, wysokoscOkna);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, (float)szerokoscOkna/(float)wysokoscOkna, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


  void RysujNakladke(void) {
	  char buf[255];
	  glMatrixMode(GL_PROJECTION);
	  glPushMatrix();
	  glLoadIdentity();
	  glOrtho(0.0, szerokoscOkna, 0.0, wysokoscOkna, -100.0, 100.0);
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glLoadIdentity();
	  glDisable(GL_LIGHTING);
	  glColor3f(0.0, 0.0, 0.0); //KOLOR TEKSTU

	  sprintf(buf, "Autor: Redkiewicz Damian");
	  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT + 280);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, "Grupa: I4Z7S1");
	  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT + 270);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, "Nr albumu: 59950");
	  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT + 260);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, "3/4 sfery:");
	  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, "KLAWISZ - FUNKCJA");
	  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT + 15);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, "[ ] - podzialy pionowe (%d)", N2);
	  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 20);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, ", . - podzialy poziome (%d)", N);
	  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 35);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, "- + - odleglosc obserwatora (%d)", obsR);
	  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 50);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  /*MENU SWIATLO*/
	  sprintf(buf, "Swiatlo");
	  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, "w s - predkosc katowa X  (%.1f)", RX);
	  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 10);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);


	  sprintf(buf, "z x - predkosc katowa Y  (%.1f)", RY);
	  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 20);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);


	  sprintf(buf, "a d - predkosc katowa Z  (%.1f)", RZ);
	  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 30);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);



	  sprintf(buf, "r f - promien orbity     (%.1f)", rRef);
	  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 40);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	 

	  sprintf(buf, "1 - wlaczony reflektor  (%s)", (lightSwitch[0] == 1) ? "tak" : "nie");
	  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 50);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, "2 - wlaczone swiatlo kierunkowe  (%s)", (lightSwitch[1] == 1) ? "tak" : "nie");
	  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 60);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, "0 9 - zmiana trybu glShadeModel ");
	  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 70);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	 

	  /*MENU MATERIAL*/
	  sprintf(buf, "Material");
	  glRasterPos2i(X_OFFSET_MAT, Y_OFFSET_MAT);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, "4 5 6 - typ materialu (%d)", zmienna);
	  glRasterPos2i(X_OFFSET_MAT, Y_OFFSET_MAT-15);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  sprintf(buf, "4 - zolty blyszczacy");
	  glRasterPos2i(X_OFFSET_MAT, Y_OFFSET_MAT - 30);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);
	  sprintf(buf, "5 - niebieski matowy");
	  glRasterPos2i(X_OFFSET_MAT, Y_OFFSET_MAT - 40);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);
	  sprintf(buf, "6 - braz");
	  glRasterPos2i(X_OFFSET_MAT, Y_OFFSET_MAT - 50);
	  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	  glMatrixMode(GL_PROJECTION);
	  glPopMatrix();
	  glMatrixMode(GL_MODELVIEW);
	  glPopMatrix();
	  glEnable(GL_LIGHTING);
  }


void WyswietlObraz(void)
{
  int j;
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  UstawParametryWidoku(szerokoscOkna, wysokoscOkna);
  WlaczOswietlenie(lightSwitch[0], lightSwitch[1]);
  glTranslatef(0, 0, -obsR);
  glRotatef(katX, 1, 0, 0);
  glRotatef(katY, 0, 1, 0);
  glPushMatrix();
	glTranslatef(0.0, swiatlo[3][1], 0.0);
	glRotatef(katRX, 1,0,0);
	glRotatef(katRZ, 0,0,1);
	glRotatef(katRY, 0,1,0);
    glDisable(GL_LIGHTING);
	//glBegin(GL_LINES);
	glColor3f(0.0,0.0,0.0);
	for(j=0; j<=360.0L; j++)
	{
		glVertex3f(rRef*sin(DEG2RAD(j)), 0.0,rRef*cos(DEG2RAD(j)));
		glVertex3f(rRef*sin(DEG2RAD(j+1)), 0.0, rRef*cos(DEG2RAD(j+1)));
	}
    //glEnd();
    glTranslatef(swiatlo[3][0], 0.0, swiatlo[3][2]);
	glutSolidSphere(0.25, 20, 20); // Tworzenie sfery(wspolrzedne zrodla swiatla)
    glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, swiatlo[3]);  
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, swiatlo[4]);
  glPopMatrix();
  //Przypisania odpowiedzialne za predkosci katowe.
  katRX = katRX+RX;
  katRZ = katRZ+RZ;
  katRY = katRY+RY;
  Sfera(N2, N);
  //piers(N);
  RysujNakladke(); // RYSOWANIE NAKLADKI
  glutSwapBuffers();
}



void ObslugaKlawiatury(unsigned char klawisz, int x, int y)
{
  switch(klawisz)
  {
    //podzialy PIONOWE
	case  ',':
	   if(N<64) 
	   N=N+2;
	   break;
    case  '.':
       if(N>4)
       N=N-2;
	   break;
	//podzialy POZIOME
	case  '[':
	   if(N2<64) 
	   N2=N2+2;
	   break;
    case  ']':
       if(N2>4)
       N2=N2-2;
	   break;
	//Wielkosc sfery
	case  '7':
	   r++;
	   break;
    case  '8':
	   if(r>1.0)
       r=r--;
       break;
	//odleglosc obserwatora
    case '+':
	   obsR++;
	   break;
	case '-':
	   obsR--;
	   break;
	//zmianna promienia orbity dla zrodla swiatla
	case 'r':
		rRef=rRef+0.1;
		break;
	case 'f':
		rRef=rRef-0.1;
		break;
	//wlacznik i wylacznik swiatla1(reflektor)
	case '1':
        if (lightSwitch[0]==0) lightSwitch[0]=1;
	  	else lightSwitch[0]=0;
      	break;
	//wlacznik i wylacznik swiatla2(kierunkowe)
    case '2':
      		if (lightSwitch[1]==0) lightSwitch[1]=1;
	  	else lightSwitch[1]=0;
      	break;
	//zmiana polozenia orbity zrodla swiatla(3 predkosci katowe, rozne osie)
	case 'w':
		if(RX>0.0)
		RX=RX-0.1; 
     	break;
    case 's':
		if(RX<0.3)
		RX=RX+0.1;
      	break;
    case 'a':
		if(RZ>0.0)
		RZ=RZ-0.1;
      	break;
    case 'd':
		if(RZ<0.3)
		RZ=RZ+0.1;
      	break;
	case 'z':
		if(RY>0.0)
		RY=RY-0.1;
		break;
	case 'x':
		if(RY<0.3)
		RY=RY+0.1;
      	break;
//zmiana materialu 
	case '4':
		zmienna = 4;
		break;
	case '5':
		zmienna = 5;
		break;
	case '6':
		zmienna = 6;
		break;
	case '9':
		glShadeModel(GL_FLAT);
		break;
	case '0':
		glShadeModel(GL_SMOOTH);
		break;
    case 27:
      	exit(0);      
    }
}
void ObslugaKlawiszySpecjalnych(int klawisz, int x, int y)
{
  switch(klawisz)
    {
		//zmianna polozenia obserwatora
	case GLUT_KEY_UP:
		katX++;
		break;
    case GLUT_KEY_DOWN:
		katX--;
      	break;
	case GLUT_KEY_RIGHT:
		katY++;
	  	break;
    case GLUT_KEY_LEFT:
		katY--;
      	break;
    }
}



int  main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(szerokoscOkna, wysokoscOkna);
  glutCreateWindow("Oswietlenie dla 3/4 sfery");
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1000.0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor (1.0, 1.0, 1.0, 1.0);
  glutDisplayFunc(WyswietlObraz);
  glutReshapeFunc(UstawParametryWidoku);
  glutIdleFunc(WyswietlObraz);
  glutKeyboardFunc(ObslugaKlawiatury);
  glutSpecialFunc(ObslugaKlawiszySpecjalnych);
  UstawDomyslneWartosciParametrow();
  glutMainLoop();
  return 0;
}

