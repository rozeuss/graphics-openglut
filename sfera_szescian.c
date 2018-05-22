//////////////////////////////////////////////////////////////////////////////////////////
//
// Program wyswietlajacy szescian w rzucie perspektywicznym. Dlugosc boku szescianu
// moze byc interaktywnie zmieniana za pomoca klwiszy '+' i '-'.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <GL/glut.h>
#include <math.h>

// Definicja stalych
#define DLUGOSC_BOKU            5.0
#define OBSERWATOR_ODLEGLOSC    20.0
#define OBSERWATOR_OBROT_X      20.0
#define OBSERWATOR_OBROT_Y      20.0
#define OBSERWATOR_FOV_Y        30.0

#define r_dolne 2 // dolna podstawa stozka scietego
#define r_gorne 4 // gorna podstawa stozka scietego
#define h       8 // wysokosc podstawa stozka scietego

#define PI 3.1415926535

int N  = 8;
int M  = 16;

// Parametry kuli
#define r       2

// Zmienne globalne
double  bok           = DLUGOSC_BOKU; // Dlugosc boku szescianu
int     szerokoscOkna = 800;
int     wysokoscOkna  = 600;

// Zmienne do osi ukladu wsporzednych
GLfloat rotObsY     = 40.0;
GLfloat rotObsX     = 40.0;
GLfloat rotObsZ     = 0.0;
GLfloat odleglosc   = -40.0;
GLfloat odlmin      = -100.0;
GLfloat odlmax      = -10.0;

// Prototypy funkcji
void RysujSzescian(double a);
void UstawParametryWidoku(int szer, int wys);
void WyswietlObraz(void);
void ObslugaKlawiatury(unsigned char klawisz, int x, int y);

// Pocztaek tworzenia ukladu wspolrzednych
void tworzUkladWspolrzednych()
{
  glBegin(GL_LINES);

	// Os X
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-20.0, 0.0, 0.0);
    glVertex3f(20.0, 0.0, 0.0);

    // Os Y
    glColor3f(0.0,1.0,0.0);
    glVertex3f(0.0, -20.0, 0.0);
    glVertex3f(0.0, 20.0, 0.0);

    // Os Z
    glColor3f(0.0,0.0,1.0);
    glVertex3f(0.0, 0.0, -20.0);
    glVertex3f(0.0, 0.0, 20.0);

	// Koniec tworzenia ukladu wspolrzednych
  glEnd();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja rysujaca szescian o boku "a" w trybie GL_QUAD_STRIP.
// Srodek szescianu znajduje sie w punkcie (0,0,0).
void RysujSzescian(double a)
{
  // Sciany boczne
  glBegin(GL_QUAD_STRIP);
    glVertex3f(a/2.0, a/2.0, a/2.0);
    glVertex3f(a/2.0, -a/2.0, a/2.0);
    glVertex3f(a/2.0, a/2.0, -a/2.0);
    glVertex3f(a/2.0, -a/2.0, -a/2.0);

    glVertex3f(-a/2.0, a/2.0, -a/2.0);
    glVertex3f(-a/2.0, -a/2.0, -a/2.0);

    glVertex3f(-a/2.0, a/2.0, a/2.0);
    glVertex3f(-a/2.0, -a/2.0, a/2.0);

    glVertex3f(a/2.0, a/2.0, a/2.0);
    glVertex3f(a/2.0, -a/2.0, a/2.0);
  glEnd();

  // Gorna sciana
  glBegin(GL_QUAD_STRIP);
    glVertex3f(-a/2.0, a/2.0, a/2.0);
    glVertex3f(a/2.0, a/2.0, a/2.0);
    glVertex3f(-a/2.0, a/2.0, -a/2.0);
    glVertex3f(a/2.0, a/2.0, -a/2.0);
  glEnd();

  // Dolna sciana
  glBegin(GL_QUAD_STRIP);
    glVertex3f(-a/2.0, -a/2.0, a/2.0);
    glVertex3f(a/2.0, -a/2.0, a/2.0);
    glVertex3f(-a/2.0, -a/2.0, -a/2.0);
    glVertex3f(a/2.0, -a/2.0, -a/2.0);
  glEnd();

}

// Tworzenie stozka scietego
void rysujStozek()
{
  float fi, y, x, z, R1 = r_dolne, R2 = r_gorne, poziom = r_dolne;

  glColor3f(0.0,1.0,0.0);
  // dolna podstawa
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, 0.0, 0.0);
    for(fi = 0.0; fi <= 2.0001*PI; fi += 2*PI/N)
    {
      glVertex3f(r_dolne*cos(fi), 0, r_dolne*sin(fi)); // dolna
    }
  glEnd();

  glTranslatef(0.0, h, 0.0);
  // gorna podstawa
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, 0.0, 0.0);
    for(fi = 0.0; fi <= 2.0001*PI; fi += 2*PI/N)
    {
      glVertex3f(r_gorne*cos(fi), 0, r_gorne*sin(fi));  //gorna
    }
  glEnd();

  glTranslatef(0.0, -h, 0.0);
  // boki
  glBegin(GL_QUAD_STRIP);
    glColor3f(1, 1, 0);
    for(y = 0.0; y < h - 0.0001; y += 1.0 * h / M, poziom+=((R2-R1)/M))
	{
	  for(fi = 0.0; fi <= 2.0001*PI; fi += 2*PI/N)
      {
        x = poziom * cos(fi);
        z = poziom * sin(fi);
        glVertex3f(x, y, z);
        x = (poziom+((R2-R1)/M)) * cos(fi);
        z = (poziom+((R2-R1)/M)) * sin(fi);
        glVertex3f(x, y + 1.0 * h / M, z);
	  }
	}
  glEnd();
}

// Tworzenie sfery o promieniu = 2
void rysujSfere() {
  float teta, fi, x, y, z;

  glColor3f(1.0,1.0,1.0);
  for(teta = 0; teta <= PI-0.0001; teta += PI/M)
  {
    glBegin(GL_QUAD_STRIP);
    for(fi = 0.0; fi <= 1.5001*PI; fi += 1.5*PI/N)
    {
        x = r * sin(teta+PI/M) * cos(fi);
        z = r * sin(teta+PI/M) * sin(fi);
        y = r * cos(teta+PI/M);
        glVertex3f(x, y, z);
        x = r * sin(teta) * cos(fi);
        z = r * sin(teta) * sin(fi);
        y = r * cos(teta);
        glVertex3f(x, y, z);
    }
    glEnd();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawiajaca parametry rzutu perspektywicznego i rozmiary viewportu. Powinna
// byc wywolywana kazdorazowo po zmianie rozmiarow okna programu.
void UstawParametryWidoku(int szer, int wys)
{
  // Zapamietanie wielkosci widoku
  szerokoscOkna = szer;
  wysokoscOkna  = wys;

  // Ustawienie parametrow viewportu
  glViewport(0, 0, szerokoscOkna, wysokoscOkna);

  // Przejscie w tryb modyfikacji macierzy rzutowania
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(OBSERWATOR_FOV_Y, (float)szerokoscOkna/(float)wysokoscOkna, 1.0, 1000.0);
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja wyswietlajaca pojedyncza klatke animacji
void WyswietlObraz(void)
{
  // Wyczyszczenie bufora koloru i bufora glebokosci
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // Przejscie w tryb modyfikacji macierzy przeksztalcen geometrycznych
  glMatrixMode(GL_MODELVIEW);

  // Zastapienie aktywnej macierzy macierza jednostkowa
  glLoadIdentity();

  // Ustalenie polozenia obserwatora
  glTranslatef(0, 0, odleglosc);
  glRotatef(rotObsX, 1, 0, 0);
  glRotatef(rotObsY, 0, 1, 0);
  glRotatef(rotObsZ, 0, 0, 1);

  // Rysowanie osi ukladu wspolrzednych
  tworzUkladWspolrzednych();
  // Narysowanie szescianu
  //RysujSzescian(bok);
  // Narysowanie stozka scietego
  rysujStozek();
  //rysujSfere();

  // Przelaczenie buforow ramki
  glutSwapBuffers();
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja obslugi klawiatury
void ObslugaKlawiatury(unsigned char klawisz, int x, int y)
{
  switch (klawisz)
  {
      case '=':
        bok *= 2.0;
        break;

      case '-':
        bok /= 2.0;
        break;

      case 'w':
        odleglosc = (odleglosc < odlmax) ? odleglosc + 5 : odleglosc;
        break;

      case 's':
        odleglosc = (odleglosc > odlmin) ? odleglosc - 5 : odleglosc;
        break;

      case 'x':
        N = (N < 64) ? N + 1 : N;
        break;

      case 'z':
        N = (N > 4) ? N - 1 : N;
        break;

      case 'q':
        M = (M < 64) ? M + 1 : M;
        break;

      case 'a':
        M = (M > 4) ? M - 1 : M;
        break;
  }
}

void ObslugaKlawiszySpecjalnych(int klawisz, int x, int y)
{
	switch (klawisz)
	{
	case GLUT_KEY_UP:
		rotObsX = rotObsX + 1.0;
		break;

	case GLUT_KEY_DOWN:
		rotObsX = rotObsX - 1.0;
		break;

	case GLUT_KEY_LEFT:
		rotObsY = rotObsY - 1.0;
		break;

	case GLUT_KEY_RIGHT:
		rotObsY = rotObsY + 1.0;
		break;

	case GLUT_KEY_PAGE_UP:
		rotObsZ = rotObsZ + 1.0;
		break;

	case GLUT_KEY_PAGE_DOWN:
		rotObsZ = rotObsZ - 1.0;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Glowna funkcja programu
int  main(int argc, char **argv)
{
  // Zainicjowanie biblioteki GLUT
  glutInit(&argc, argv);

  // Ustawienie trybu wyswietlania
  glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);

  // Ustawienie polozenia dolenego lewego rogu okna
  glutInitWindowPosition(100, 100);

  // Ustawienie rozmiarow okna
  glutInitWindowSize(szerokoscOkna, wysokoscOkna);

  // Utworzenie okna
  glutCreateWindow("Szescian");

  // Odblokowanie bufora glebokosci
  glEnable(GL_DEPTH_TEST);

  // Ustawienie wartosci czyszczacej zawartosc bufora glebokosci
  glClearDepth(1000.0);

  // Ustawienie koloru czyszczenia bufora ramki
  glClearColor (0.3f, 0.3f, 0.3f, 0.0f);

  // Wlaczenie wyswietlania wielokatow w postaci obrysow (przydatne w celach diagnostycznych).
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Zarejestrowanie funkcji (callback) odpowiedzialnej za
  glutDisplayFunc(WyswietlObraz);

  // Zarejestrowanie funkcji (callback) wywolywanej za kazdym razem kiedy
  // zmieniane sa rozmiary okna
  glutReshapeFunc(UstawParametryWidoku);

  // Zarejestrowanie funkcji wykonywanej gdy okno nie obsluguje
  // zadnych zadan
  glutIdleFunc(WyswietlObraz);

  // Zarejestrowanie funkcji obslugi klawiatury
  glutKeyboardFunc(ObslugaKlawiatury);

  //obracanie wokol osi ukladu wspolrzednych
  glutSpecialFunc(ObslugaKlawiszySpecjalnych);

  // Obsluga glownej petli programu (wywolywanie zarejestrowanych callbackow
  // w odpowiedzi na odbierane zdarzenia lub obsluga stanu bezczynnosci)
  glutMainLoop();

  return 0;
}

