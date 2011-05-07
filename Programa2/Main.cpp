#include "Main.h"
#include "3ds.h"
#include "Ase.h"

//Libreria que usamos para el timer
#pragma comment( lib, "winmm.lib" )	

HDC			hDC=NULL;		// Dispositivo de contexto GDI
HGLRC		hRC=NULL;		// Contexto de renderizado
HWND		hWnd=NULL;		// Manejador de ventana
HINSTANCE	hInstance;		// Instancia de la aplicacion

bool	keys[256];			// Arreglo para el manejo de teclado
bool	active=TRUE;		// Bandera de ventana activa

//Variables de Windows
HWND g_hMainWindow = NULL;
OPENFILENAME ofn;
char szFileName[MAX_PATH] = "";
char szFileTitle[MAX_PATH] = "";
 
int glWidth;
int glHeight;

GLUquadricObj	*e; 

CMateriales Material;

// variable para atacar
bool Ataque;
// variable para medir el tamano del ataque
float tamano = 0.0f;

//Nombre y ubicación del modelo y trompa
#define FILE_NAVE  "Modelos/nave.ase"
#define FILE_TROMPA  "Modelos/trompa.ase"

//Variables para el calculo de colisiones
#define maxPersonajes 11

// Variables para el calculo de colisiones
esfera esferas[maxPersonajes];

//Nombre y ubicacion del primer tipo de nave enemiga
#define FILE_ENEMIGO1 "Modelos/nave_mala1.ase"

// Variables de prueba para el movimiento de la nave :P
GLfloat incremento = 0.0f;
GLfloat posnavex = -5.0f;
GLfloat posnavey = 4.0f;
GLfloat posnavez = 0.0f;

GLfloat postrompax = posnavex + 5.0f;
GLfloat postrompay = posnavey - 3.9f;
GLfloat postrompaz = 0.0f;
GLfloat angulotrompa = 0.0f;

int limiteancho = 0;
int limitelargo = 320;

//Valor que permitira detener el avance del escenario
int detenerse = 15;

//Contenedor de texturas de los modelos
CTga texturaNave[30];
CTga texturaTrompa[30];
CTga texturaNaveMala[30];

CLoadASE g_LoadAse;

//Acceso a la estructura que almacena los datos del modelo
t3DModel ModeloNave;
t3DModel ModeloTrompa; 
t3DModel ModeloEnemigo1;

//Contenedor de texturas para el escenario
CTga textura[30];

//Variables de multitexturas y shaders
CMultitexturas Multitext;

//Constantes de iluminación y materiales
GLfloat LightPos[] = { 1.0f, 1.0f, 1.0f, 1.0f };			// Posición de la luz
GLfloat LightAmb[] = { 1.0f, 1.0f, 1.0f, 1.0f};			// Valores de la componente ambiente
GLfloat LightDif[] = { 1.0f, 1.0f, 1.0f, 1.0f};			// Valores de la componente difusa
GLfloat LightSpc[] = { 1.0f, 1.0f, 1.0f, 1.0f};			// Valores de la componente especular
static float exponent = 2.0f;

//Variables de control para la Trayectoria1 del Spline
GLfloat PuntoInicialSP1, IncrementoSP1;
GLfloat PuntoInicialSP2, IncrementoSP2;

int Comenzar1;
int Comenzar2;

CVector lightPosition;

//Variables de la cámara
CVector PosicionCam;
CVector DireccionCam;
CVector VistaCam;
CVector SuperiorCam;
CVector PosicionCamAnt;
float AngCam;
float VelCam;

//Variables auxiliares para animación de texturas
float aText1;
float aText2;
float aText3;

//Acceso a la clase CFont
CFont Font;

//Variables para splines 1 y 2
spline Trayectoria1;
spline Trayectoria2;

CVector PosicionSpline1;
CVector PosicionSpline2;

CVector PuntosSpline1[] = {
	CVector( 20.0f, -2.0f, 0.0f),
	CVector( 20.0f,  3.0f, 0.0f),
	CVector( 20.0f,  8.0f, 0.0f),
	CVector( 20.0f,  3.0f, 0.0f),
	CVector( 20.0f, -2.0f, 0.0f),
	CVector( 15.0f,  3.0f, 0.0f),
	CVector( 10.0f,  8.0f, 0.0f),
	CVector(  5.0f,  3.0f, 0.0f),
	CVector(  0.0f, -2.0f, 0.0f),
	CVector(  0.0f,  3.0f, 0.0f),
	CVector(  0.0f,  8.0f, 0.0f),
	CVector(  0.0f,  3.0f, 0.0f),
	CVector(  0.0f, -2.0f, 0.0f),
	CVector(  5.0f,  3.0f, 0.0f),
	CVector( 10.0f,  8.0f, 0.0f),
	CVector( 15.0f,  3.0f, 0.0f),
	CVector( 20.0f, -2.0f, 0.0f),
};

CVector PuntosSpline2[] = {
	CVector( 25.0f,  1.0f, 0.0f),
	CVector( 25.0f,  3.0f, 0.0f),
	CVector( 25.0f,  8.0f, 0.0f),
	CVector( 25.0f,  3.0f, 0.0f),
	CVector( 25.0f, -2.0f, 0.0f),
	CVector( 20.0f,  3.0f, 0.0f),
	CVector( 15.0f,  8.0f, 0.0f),
	CVector( 10.0f,  3.0f, 0.0f),
	CVector(  5.0f, -2.0f, 0.0f),
	CVector(  5.0f,  3.0f, 0.0f),
	CVector(  5.0f,  8.0f, 0.0f),
	CVector(  5.0f,  3.0f, 0.0f),
	CVector(  5.0f, -2.0f, 0.0f),
	CVector( 10.0f,  3.0f, 0.0f),
	CVector( 15.0f,  8.0f, 0.0f),
	CVector( 20.0f,  3.0f, 0.0f),
	CVector( 25.0f, -2.0f, 0.0f),
};

#define totalPC1 (sizeof(PuntosSpline1)/sizeof(CVector))
#define totalPC2 (sizeof(PuntosSpline2)/sizeof(CVector))

//Variables para el control de fps
GLfloat Time1;
GLfloat Time2;
GLfloat DiffTime;
static int FPS = 0;

bool  framerate_limit = true;

int	  adjust=2;											// Speed Adjustment For Really Slow Video Cards

struct			 										// Create A Structure For The Timer Information
{
  __int64       frequency;								// Timer Frequency
  float         resolution;								// Timer Resolution
  unsigned long mm_timer_start;							// Multimedia Timer Start Value
  unsigned long mm_timer_elapsed;						// Multimedia Timer Elapsed Time
  bool			performance_timer;						// Using The Performance Timer?
  __int64       performance_timer_start;				// Performance Timer Start Value
  __int64       performance_timer_elapsed;				// Performance Timer Elapsed Time
} timer;												// Structure Is Named timer

int		steps[6]={ 1, 2, 4, 5, 10, 20 };				// Stepping Values For Slow Video Adjustment

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaracion de WndProc (Procedimiento de ventana)

void TimerInit(void)
{
	memset(&timer, 0, sizeof(timer));

	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
	{
		timer.performance_timer	= FALSE;
		timer.mm_timer_start	= timeGetTime();
		timer.resolution		= 1.0f/1000.0f;
		timer.frequency			= 1000;
		timer.mm_timer_elapsed	= timer.mm_timer_start;
	}
	else
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
		timer.performance_timer			= TRUE;
		timer.resolution				= (float) (((double)1.0f)/((double)timer.frequency));
		timer.performance_timer_elapsed	= timer.performance_timer_start;
	}
}

float TimerGetTime()
{
	__int64 time;

	if (timer.performance_timer)
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);
		return ( (float) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
	}
	else
	{
		return( (float) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
	}
}

void CalculateFrameRate()
{
	static float framesPerSecond    = 0.0f;
    static float lastTime			= 0.0f;
	static char strFrameRate[50] = {0};

	float currentTime = GetTickCount() * 0.001f;				

	++framesPerSecond;

	if( currentTime - lastTime > 1.0f )
    {
	    lastTime = currentTime;
		
		FPS=int(framesPerSecond);
		
	    framesPerSecond = 0;
    }
}

void inicializaSpline(spline &sp, CVector * ctrl, int tot, int res)
{
	sp.puntosctrl = ctrl;
	sp.puntosporTramo = res;
	sp.totalPuntosControl = tot;
	sp.puntosaDibujar = (tot-3)*res;
}

void calculaPuntoenSpline(spline &sp, int indice,  CVector *P)
{ 
    int i, j;
	GLfloat t, t3, t2, c1, c2, c3, c4, _1_t;
	CVector Pj3, Pj2, Pj1, Pj0;

    indice = indice % sp.puntosaDibujar;
	j = indice / sp.puntosporTramo + 3;
	i = indice % sp.puntosporTramo;
	Pj3=sp.puntosctrl[j-3];
	Pj2=sp.puntosctrl[j-2];
	Pj1=sp.puntosctrl[j-1];
	Pj0=sp.puntosctrl[j-0];
	t = i/(double)sp.puntosporTramo;
	t2 = t*t; 
	t3 = t2*t;
	_1_t = 1-t;
	c1 = (_1_t*_1_t*_1_t) / 6;
	c2 = (3*t3-6*t2+4) / 6;
	c3 = (-3*t3+3*t2+3*t+1) / 6;
	c4 = t3 / 6;
	P->x = c1* Pj3.x + c2*Pj2.x + c3*Pj1.x + c4*Pj0.x;
	P->y = c1* Pj3.y + c2*Pj2.y + c3*Pj1.y + c4*Pj0.y;
	P->z = c1* Pj3.z + c2*Pj2.z + c3*Pj1.z + c4*Pj0.z;
}



GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Redimensiona e inicializa la ventana
{
	if (height==0)							// Para que no se presente una division por cero
	{
		height=1;							// la altura se iguala a 1
	}

	glViewport(0,0,width,height);					// Resetea el puerto de vista

	glMatrixMode(GL_PROJECTION);					// Selecciona la Matriz de Proyeccion
	glLoadIdentity();								// Resetea la Matriz de Proyeccion

	// Calcula el radio de aspecto o proporcion de medidas de la ventana
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,2000.0f);
	
	glMatrixMode(GL_MODELVIEW);							// Selecciona la Matriz de Vista de Modelo
	glLoadIdentity();									// Resetea la Matriz de Vista de Modelo

	glWidth=width;
	glHeight=height;
}

void CargaTexturas()
{
	textura[0].LoadTGA("Texturas/agua.tga");
	
}

void DescargaTexturas()
{
	textura[0].Elimina();
}

int CargaModelosASE()
{
	if(!g_LoadAse.LoadAseFile(FILE_NAVE, &ModeloNave, texturaNave))
		return 0;

	if(!g_LoadAse.LoadAseFile(FILE_TROMPA, &ModeloTrompa, texturaTrompa))
		return 0;

	if(!g_LoadAse.LoadAseFile(FILE_ENEMIGO1, &ModeloEnemigo1, texturaNaveMala))
		return 0;

	return TRUE;
}

void DescargaModelosASE()
{
	g_LoadAse.UnLoadASEFile(&ModeloNave, texturaNave);
	g_LoadAse.UnLoadASEFile(&ModeloTrompa, texturaTrompa);
	g_LoadAse.UnLoadASEFile(&ModeloEnemigo1, texturaNaveMala);
}

void Camara(int funcion)
{	
	float alt, alt1;

	alt = VistaCam.y;
	alt1 = VistaCam.x;
	
	if(funcion == 0) //Inicializa la camara
	{
		//Inicia Posición
		PosicionCam.x = 0.0f;
		PosicionCam.y = 5.0f;
		PosicionCam.z = 10.0f;
		//Inicia Dirección
		AngCam = 270.0f;
		DireccionCam.x = (float)cos(AngCam*PI/180.0f);
		DireccionCam.y = 0.0f;
		DireccionCam.z = (float)sin(AngCam*PI/180.0f);
		//Inicia Punto de Vista
		VistaCam = PosicionCam+DireccionCam*10.0f;
		VistaCam.y=4.0f;
		alt=10.0f;
		//Inicia Vector Superior
		SuperiorCam.x = 0.0f;
		SuperiorCam.y = 1.0f;
		SuperiorCam.z = 0.0f;
		//Inicia Velocidad
		VelCam = 0.01f;
		
	}
	else if(funcion == 1) //Cambia direccion de la camara
	{
		DireccionCam.x = (float)cos(AngCam*PI/180.0f);
		DireccionCam.y = 0.0f;
		DireccionCam.z = (float)sin(AngCam*PI/180.0f);

		VistaCam = PosicionCam+DireccionCam*10.0f;
		VistaCam.y = alt;
	}
	else if(funcion == 2) //Desplaza la camara hacia adelante de su direccion
	{
		PosicionCam = PosicionCam+DireccionCam*VelCam;
		VistaCam = PosicionCam+DireccionCam*10.0f;
		VistaCam.y = alt;

		PosicionCamAnt = PosicionCam;
	}
	else if(funcion == 3) //Desplaza la camara hacia atras de su direccion
	{
		PosicionCam = PosicionCam-DireccionCam*VelCam;
		VistaCam = PosicionCam+DireccionCam*10.0f;
		VistaCam.y = alt;

		PosicionCamAnt = PosicionCam;
	}
	else if(funcion == 4) //Desplaza la camara hacia la izquierda de su direccion
	{
		PosicionCam.y+=0.5f;
		VistaCam.y = alt;
		VistaCam.y+=0.5f;
	}
	else if(funcion == 5) //Desplaza la camara hacia la derecha de su direccion
	{
		PosicionCam.x+=0.02f;
		VistaCam.x = alt1;
		VistaCam.x+=0.02f;
		incremento +=0.02f;
	}
		
}

int InitGL(GLvoid)										// Aqui se configuran los parametros iniciales de OpenGL
{
	Multitext.InitMultitext(hWnd);

	glShadeModel(GL_SMOOTH);							// Activa Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Fondo de la Pantalla
	//glClearColor(0.1f, 0.2f, 0.3f, 0.5f);				// Fondo de la Pantalla
	glClearDepth(1.0f);									// Valor para el Depth Buffer
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);							// Activa Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Correccion de cálculos de perspectiva

	glCullFace(GL_BACK);								// Set Culling Face To Back Face
	glEnable(GL_CULL_FACE);								// Enable Culling

	//Iluminación
	//Inicializa la luz
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz0
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular
	/*
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.05f);   //Brillo de la luz
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f); //Iluminacion general de la escena
	*/
	
	glEnable(GL_LIGHT0);								// Activa luz0
	glEnable(GL_LIGHTING);								// Habilita la iluminación

	//CargaTexturas();

	CargaModelosASE();
	Font.BuildFont();

	//InitGLSL();
		
	e=gluNewQuadric();
	Camara(0);

	//Comienza la inicializacion de variables para Splines
	Comenzar1 = 1;
	Comenzar2 = 1;
	
	inicializaSpline(Trayectoria1, PuntosSpline1, totalPC1, 5);
	inicializaSpline(Trayectoria2, PuntosSpline2, totalPC2, 5);

	PuntoInicialSP1 = 1;
	PuntoInicialSP2 = 1;
	IncrementoSP1 = 0.25f;
	IncrementoSP2 = 0.25f;

	return TRUE;									
}

void DibujaEjes()
{
	glColor3f(1.0f,0.0f,0.0f);

	glBegin(GL_LINES);
		//Eje X
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);

		//Eje Y
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f,  100.0f, 0.0f);

		//Eje Z
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f,  100.0f);
	glEnd();

	glColor3f(1.0f,1.0f,1.0f);
}

void DibujaLuz(CVector l)
{
	//Dibuja una esfera que representa la fuente luminosa
	glDisable(GL_LIGHTING);									// Deshabilita iluminación
	glPushMatrix();
		glTranslatef(l.x, l.y, l.z);// Traslada a la posicion de la luz
		glColor4f(1.0f, 0.9f, 0.0f, 1.0f);					// Color amarillo
		gluSphere(e, 0.5f, 16, 8);							// Dibuja la esfera
	glPopMatrix();
	glEnable(GL_LIGHTING);									// Habilita Iluminación

	glColor3f(1.0f, 1.0f, 1.0f);
}

void DibujaTextos()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva la prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyección
	glPushMatrix();										// Guarda la matriz de proyección
	glLoadIdentity();									// Limpia la matriz de proyección
	glOrtho(0,glWidth,0,glHeight,-1,1);					// Crea una proyección ortogonal
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda la matriz de modelo de vista
	
	glDisable(GL_LIGHTING);
	glLoadIdentity();

	glColor3f(1.0f,1.0f,1.0f);
	
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.6f);
	glEnable(GL_ALPHA_TEST);

	//Font.glPrint((0.8f/640.0f)*glWidth, glWidth*0.85f,glHeight*0.95f,".FPS %d",FPS);
	
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f,1.0f,1.0f);

	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyección
	glPopMatrix();										// Recupera la anterior matriz de proyección
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Recupera la anterior matriz de modelo de vista
	glEnable(GL_DEPTH_TEST);							// Activa la prueba de profundidad
		
}

void ActualizaAnimText()
{
	//Variables para las flechas del piso
	if(aText1 < 1.0f)
		aText1+=0.005f;
	else
		aText1=0.0f;

	if(aText2 < 1.2f)
		aText2+=0.006f;
	else
		aText2=0.0f;

	if(aText3 < 1.6f)
		aText3+=0.008f;
	else
		aText3=0.0f;
			
}

void DibujaSpline(spline &sp)
{
	CVector P;

	glDisable(GL_LIGHTING);

	// Trayectoria1 del spline
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
		for(int i=0; i < sp.puntosaDibujar; i++ ) 
		{
			calculaPuntoenSpline(sp, i, &P);
			glVertex3f(P.x, P.y, P.z);
		}
	glEnd();

	//Puntos en el spline
	glPointSize(4.0f);
	glColor3f(0.0f,0.0f,1.0f);
	
	for(int i=0; i < sp.puntosaDibujar; i++ ) 
	{
		calculaPuntoenSpline(sp, i, &P);
		glBegin(GL_POINTS);
			glVertex3f(P.x, P.y, P.z);
		glEnd();
	}
			
	//Puntos de control
	glPointSize(10.0f);
	glColor3f(1.0f,0.0f,0.0f);

	for (int i=0; i<sp.totalPuntosControl; i++ )
	{
		glBegin(GL_POINTS);
			glVertex3f(sp.puntosctrl[i].x, sp.puntosctrl[i].y, sp.puntosctrl[i].z);
		glEnd();
		
	}

	glColor3f(1.0f,1.0f,1.0f);
	glPointSize(1.0f);

	glEnable(GL_LIGHTING);
	
}



void ActualizaIluminacion()
{
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz0
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular

	lightPosition.x=LightPos[0];
	lightPosition.y=LightPos[1];
	lightPosition.z=LightPos[2];
}

// Funcion que determina si existe o no colision
int ColisionEsferaEsfera(esfera a, esfera b)
{
	// Calculamos la distancia entre los centros
	CVector d = a.centro - b.centro;
	float distancia = Punto(d,d);
	// Si las esferas colisionan es por que la distancia es menor o igual que la suma de sus cuadrados.
	float Sumaradios = a.radio + b.radio;
	return distancia <= Sumaradios * Sumaradios;
}

void DibujaNave()
{
	GLUquadricObj *q;

	q = gluNewQuadric();
	glPushMatrix();	
		//Comienza a dibujar la nave y la coloca en una posición inicial dentro del escenario =D
		glTranslatef(posnavex + incremento, posnavey, posnavez);
		g_LoadAse.RenderASEFile(&ModeloNave, texturaNave, 1);
		// Modelado jerarquico, para colocar el disparador enfrente de la nave y que depende de la posicion que
		// tiene la nave =D
		glPushMatrix();
			glTranslatef(postrompax, postrompay, postrompaz);
			// Incrementa el angulo de inclinación de la nave en un solo eje :D
			glRotatef(angulotrompa, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, 0.0f, 0.0f);
			g_LoadAse.RenderASEFile(&ModeloTrompa, texturaTrompa, 1);
			gluSphere(q,0.4f,16,8);						// Dibuja la esfera
		glPopMatrix();
	glPopMatrix();
}

//void InicializaObjetosColision()
//{
//	//Esfera en la nave (proyectil)
//	esferas[0].radio = 1.6; 
//	esferas[0].centro = CVector(posnavex + incremento, posnavey, posnavez);
//	esferas[0].colisionesfera = false;
//	CVector drawsphere = CVector(posnavex + incremento, posnavey, posnavez);
//	DibujaLuz(drawsphere);
//}

void DibujaPlanoGradiente()
{
     glBegin(GL_QUADS);
		 glColor3f(0.0f, 1.0f, 1.0f); 
		 glVertex2d(-8.0f, -8.0f);
		 glColor3f(1.0f, 0.4f, 0.3f); 
		 glVertex2d(8.0f, -8.0f);
		 glColor3f(0.2f, 1.0f, 0.8f); 
		 glVertex2d(8.0f, 8.0f);
		 glColor3f(1.0f, 1.0f, 1.0f); 
		 glVertex2d(-8.0f, 8.0f);
     glEnd();
}

int RenderizaEscena(GLvoid)								// Aqui se dibuja todo lo que aparecera en la ventana
{
	GLUquadricObj *q;

q = gluNewQuadric();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(PosicionCam.x,PosicionCam.y,PosicionCam.z,VistaCam.x,VistaCam.y,VistaCam.z,SuperiorCam.x,SuperiorCam.y,SuperiorCam.z);
	//DibujaEjes();
	//Se actualizan los parámetros de iluminación
	ActualizaIluminacion();
	// Dibujamos el Plano Gradiente (Negro-Blanco)
	DibujaPlanoGradiente();
	// Invoca a la función que dibuja la nave XD
	DibujaNave();
	//Funcion que dibujara el Spline1
	if(incremento > 5.3)
	{
		if(PuntoInicialSP1 < Trayectoria1.puntosaDibujar/1.3)
		{		
			PuntoInicialSP1 += IncrementoSP1;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria1, PuntoInicialSP1, &PosicionSpline1);
			//Dibujamos la nave enemiga 1 por todo el Spline 1
			glTranslatef(PosicionSpline1.x, PosicionSpline1.y, PosicionSpline1.z);
			glScalef(0.03f,0.03f,0.03f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
			glPushMatrix();
				glTranslatef( PosicionSpline1.x, PosicionSpline1.y, PosicionSpline1.z );
				gluSphere(q, 4.0f, 16, 8);
			glPopMatrix();
			//Dibujamos la nave enemiga 2 por todo el Spline 1
			glPushMatrix();
				glTranslatef( PosicionSpline1.x + 25.0f, PosicionSpline1.y + 10.0f, PosicionSpline1.z );
				g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
				
				// Dibujar esfera
			glPopMatrix();
			//Dibujamos la nave enemiga 3 por todo el Spline 1
			glPushMatrix();
				glTranslatef( PosicionSpline1.x + 50.0f, PosicionSpline1.y + 20.0f, PosicionSpline1.z );
				g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
				// Dibujar esfera
			glPopMatrix();
			//Dibujamos la nave enemiga 4 por todo el Spline 1
			glPushMatrix();
				glTranslatef( PosicionSpline1.x + 75.0f, PosicionSpline1.y + 30.0f, PosicionSpline1.z );
				g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
				// Dibujar esfera
			glPopMatrix();
			//Dibujamos la nave enemiga 5 por todo el Spline 1
			glPushMatrix();
				glTranslatef( PosicionSpline1.x + 100.0f, PosicionSpline1.y + 40.0f, PosicionSpline1.z );
				g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
				// Dibujar esfera
			glPopMatrix();

		}
	}
	if(incremento > 10.2)
	{
		if(PuntoInicialSP2 < Trayectoria2.puntosaDibujar/1.3)
		{		
			PuntoInicialSP2 += IncrementoSP2;
			//Dibujamos la nave enemiga 1 por todo el Spline 2
			calculaPuntoenSpline(Trayectoria2, PuntoInicialSP2, &PosicionSpline2);
			// Dibujamos la nave por todo el Spline 2
			glTranslatef(PosicionSpline2.x, PosicionSpline2.y, PosicionSpline2.z);
			glScalef(0.03f,0.03f,0.03f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
			//Dibujamos la nave enemiga 2 por todo el Spline 2
			glPushMatrix();
				glTranslatef( PosicionSpline1.x + 25.0f, PosicionSpline1.y + 10.0f, PosicionSpline1.z );
				g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
				// Dibujar esfera
			glPopMatrix();
			//Dibujamos la nave enemiga 3 por todo el Spline 2
			glPushMatrix();
				glTranslatef( PosicionSpline1.x + 50.0f, PosicionSpline1.y + 20.0f, PosicionSpline1.z );
				g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
				// Dibujar esfera
			glPopMatrix();
			//Dibujamos la nave enemiga 4 por todo el Spline 2
			glPushMatrix();
				glTranslatef( PosicionSpline1.x + 75.0f, PosicionSpline1.y + 30.0f, PosicionSpline1.z );
				g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
				// Dibujar esfera
			glPopMatrix();
			//Dibujamos la nave enemiga 5 por todo el Spline 2
			glPushMatrix();
				glTranslatef( PosicionSpline1.x + 100.0f, PosicionSpline1.y + 40.0f, PosicionSpline1.z );
				g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
				// Dibujar esfera
			glPopMatrix();
		}
	}
	
	if(incremento < detenerse)
		Camara(5);
	else
	{
		//Dibujamos la trayectoria que recorrera la nave
		DibujaSpline(Trayectoria1);
		//Dibujamos la trayectoria que recorrera la nave
		DibujaSpline(Trayectoria2);
	}
	
	//DibujaTextos();
	//CalculateFrameRate();

	return TRUE;
}

GLvoid KillGLWindow(GLvoid)								// Elimina la ventana apropiadamente
{
	if (hRC)											// Si existe un contexto de renderizado...
	{
		if (!wglMakeCurrent(NULL,NULL))					// Si no se pueden liberar los contextos DC y RC...
		{
			MessageBox(NULL,"Falla al liberar DC y RC.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Si no se puede eliminar el RC?
		{
			MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Se pone RC en NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Si no se puede eliminar el DC
	{
		MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Se pone DC en NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Si no se puede destruir la ventana
	{
		MessageBox(NULL,"No se pudo liberar hWnd.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Se pone hWnd en NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Si no se puede eliminar el registro de la clase
	{
		MessageBox(NULL,"No se pudo eliminar el registro de la clase.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Se pone hInstance en NULL
	}
}

//	Este código crea la ventana de OpenGL.  Parámetros:					
//	title			- Titulo en la parte superior de la ventana			
//	width			- Ancho de la ventana								
//	height			- Alto de la ventana								
//	bits			- Número de bits a usar para el color (8/16/24/32)	
  
BOOL CreateGLWindow(char* title, int width, int height, int bits)
{
	GLuint	PixelFormat;				// Guarda el resultado despues de determinar el formato a usar
	WNDCLASS	wc;						// Estructura de la clase ventana
	DWORD		dwExStyle;				// Estilo extendido de ventana
	DWORD		dwStyle;				// Estilo de ventana
	RECT		WindowRect;				// Guarda los valores Superior Izquierdo / Inferior Derecho del rectángulo
	WindowRect.left=(long)0;			// Inicia el valor Izquierdo a 0
	WindowRect.right=(long)width;		// Inicia el valor Derecho al ancho especificado
	WindowRect.top=(long)0;				// Inicia el valor Superior a 0
	WindowRect.bottom=(long)height;		// Inicia el valor Inferior al alto especificado

	hInstance			= GetModuleHandle(NULL);				// Guarda una instancia de la ventana
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redibuja el contenido de la ventana al redimensionarla
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// Maneja los mensajes para WndProc
	wc.cbClsExtra		= 0;									// Ningun dato extra para la clase
	wc.cbWndExtra		= 0;									// Ningun dato extra para la ventana
	wc.hInstance		= hInstance;							// Inicia la instancia
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Carga el ícono por defecto
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Carga el puntero de flecha
	wc.hbrBackground	= NULL;									// No se requiere ningun fondo
	wc.lpszMenuName		= NULL;									// No hay menú en la ventana
	wc.lpszClassName	= "OpenGL";								// Fija el nombre de la clase.

	if (!RegisterClass(&wc))									// Intenta registrar la clase de ventana
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
		
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;					// Estilo extendido de ventana
	dwStyle=WS_OVERLAPPEDWINDOW;									// Estilo de ventana

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Ajusta la ventana al tamaño especificado

	// Crea la ventana
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Estilo extendido para la ventana
								"OpenGL",						// Nombre de la clase
								title,								// Título de la ventana
								dwStyle |							// Definición del estilo de la ventana
								WS_CLIPSIBLINGS |					// Estilo requerido de la ventana
								WS_CLIPCHILDREN,					// Estilo requerido de la ventana
								0, 0,								// Posición de la ventana
								WindowRect.right-WindowRect.left,	// Calcula el ancho de la ventana
								WindowRect.bottom-WindowRect.top,	// Calcula el alto de la ventana
								NULL,								// No hay ventana superior
								NULL,								// No hay menú
								hInstance,							// Instancia
								NULL)))								// No se pasa nada a WM_CREATE
	{
		KillGLWindow();												// Resetea el despliegue
		MessageBox(NULL,"Error al crear la ventana.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		1,											// 16Bit Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Si no se creo el contexto de dispositivo...
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de dispositivo GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Si Windows no encontró un formato de pixel compatible
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede encontrar un formato de pixel compatible.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Si no se pudo habilitar el formato de pixel
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el formato de pixel.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				// Si no se creo el contexto de renderizado
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					// Si no se puede activar el contexto de renderizado
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);				// Muestra la ventana
	SetForegroundWindow(hWnd);				// Le da la prioridad mas alta
	SetFocus(hWnd);							// Pasa el foco del teclado a la ventana
	ReSizeGLScene(width, height);			// Inicia la perspectiva para la ventana OGL

	if (!InitGL())							// Si no se inicializa la ventana creada
	{
		KillGLWindow();						// Resetea el despliegue
		MessageBox(NULL,"Falla en la inicialización.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;							// Todo correcto
}

LRESULT CALLBACK WndProc(	HWND	hWnd,	// Manejador para esta ventana
							UINT	uMsg,	// Mensaje para esta ventana
							WPARAM	wParam,	// Información adicional del mensaje
							LPARAM	lParam)	// Información adicional del mensaje
{
	switch (uMsg)							// Revisa los mensajes de la ventana
	{
		case WM_ACTIVATE:					// Revisa el mensaje de activación de ventana
		{
			if (!HIWORD(wParam))			// Revisa el estado de minimización
			{
				active=TRUE;				// El programa está activo
			}
			else
			{
				active=FALSE;				// El programa no está activo
			}

			return 0;						// Regresa al ciclo de mensajes
		}

		case WM_SYSCOMMAND:					// Intercepta comandos del sistema
		{
			switch (wParam)					// Revisa llamadas del sistema
			{
				case SC_SCREENSAVE:			// ¿Screensaver tratando de iniciar?
				case SC_MONITORPOWER:		// ¿Monitor tratando de entrar a modo de ahorro de energía?
				return 0;					// Evita que suceda
			}
			break;							// Sale del caso
		}

		case WM_CLOSE:						// Si se recibe un mensaje de cerrar...
		{
			PostQuitMessage(0);				// Se manda el mensaje de salida
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYDOWN:					// Si se está presionando una tecla...
		{
			keys[wParam] = TRUE;			// Si es así, se marca como TRUE
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYUP:						// ¿Se ha soltado una tecla?
		{
			keys[wParam] = FALSE;			// Si es así, se marca como FALSE
			return 0;						// y se regresa al ciclo
		}

		case WM_SIZE:						// Si se redimensiona la ventana...
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  	// LoWord=Width, HiWord=Height
			return 0;						// y se regresa al ciclo
		}
	}

	// Pasa todos los mensajes no considerados a DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

// Este es el punto de entrada al programa; la función principal 
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instancia
					HINSTANCE	hPrevInstance,		// Instancia previa
					LPSTR		lpCmdLine,			// Parametros de la linea de comandos
					int			nCmdShow)			// Muestra el estado de la ventana
{
	MSG		msg;									// Estructura de mensajes de la ventana
	BOOL	done=FALSE;								// Variable booleana para salir del ciclo

	// Crea la ventana OpenGL
	if (!CreateGLWindow("Computación Gráfica - Programa 2",640,480,32))
	{
		return 0;									// Salir del programa si la ventana no fue creada
	}

	TimerInit();

	while(!done)									// Mientras done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Revisa si hay mensajes en espera
		{
			if (msg.message==WM_QUIT)				// Si se ha recibido el mensje de salir...
			{
				done=TRUE;							// Entonces done=TRUE
			}
			else									// Si no, Procesa los mensajes de la ventana
			{
				TranslateMessage(&msg);				// Traduce el mensaje
				DispatchMessage(&msg);				// Envia el mensaje
			}
		}
		else										// Si no hay mensajes...
		{
		
			// Dibuja la escena. 
			if (active)								// Si está activo el programa...
			{
				float start=TimerGetTime();

				if (keys[VK_ESCAPE])				// Si se ha presionado ESC
				{
					done=TRUE;						// ESC indica el termino del programa
				}
				else								// De lo contrario, actualiza la pantalla
				{
					RenderizaEscena();				// Dibuja la escena
					SwapBuffers(hDC);				// Intercambia los Buffers (Double Buffering)
				}

				if(!ManejaTeclado()) return 0;
		
				while(TimerGetTime()<start+float(steps[adjust]*2.0f)) {}	// Desperdicia ciclos si es demasiado rápido
			}
			
		}
	}

	// Finalización del programa
	//DescargaTexturas();
	DescargaModelosASE();
	Font.DestroyFont();
	KillGLWindow();									// Destruye la ventana
	return (msg.wParam);							// Sale del programa
}

int ManejaTeclado()
{
	if(keys[VK_UP])
	{	if(limitelargo <= 356)
		{
			posnavey += 0.1f;
			//Camara(2);
			limitelargo++;
		}
	}
	if(keys[VK_DOWN])
	{
		if(limitelargo >= 280)
		{
			posnavey -= 0.1f;
			/*Camara(3);*/
			limitelargo--;
		}
	}
	if(keys[VK_LEFT])
	{
		if(limiteancho >= -1)
		{
			posnavex -= 0.1f;
			/*AngCam-=1.0f;
			if(AngCam < 0.0f)
				AngCam+=360.0f;

			Camara(1);*/
			limiteancho--;
		}
	}

	if(keys[VK_RIGHT])
	{
		if(limiteancho <= 100)
		{
			posnavex += 0.1f;
			/*AngCam+=1.0f;
			if(AngCam > 360.0f)
				AngCam-=360.0f;
			
			Camara(1);*/
			limiteancho++;
		}
	}

	// para disparar
	if (GetKeyState(VK_CONTROL) & 0x80)
	{
		if (false == Ataque)
		{
			Ataque = true;
		}
	}
	else
		Ataque = false;

	if(keys['D'])
	{
		angulotrompa += 1.0f;
		if(angulotrompa > 360.0f)
		{
			angulotrompa = 0.0f;
		}
	}
	if(keys['F'])
	{
		angulotrompa -= 1.0f;
		if(angulotrompa < 0.0f)
		{
			angulotrompa = 360.0f;
			//angulotrompa += 1.0f;
		}
	}
	/*if(keys[VK_PRIOR])
	{
		Camara(4);
	}
	if(keys[VK_NEXT])
	{
		Camara(5);
	}

	if(keys[VK_HOME])
	{
		VistaCam.y+=0.5f;
	}

	if(keys[VK_END])
	{
		VistaCam.y-=0.5f;
	}

	if((GetAsyncKeyState(VK_F1)&1) ==1)
	{
		VistaCam.y=PosicionCam.y;
	}

	//Controles de la iluminación
	if (keys['Z'])
		LightPos[0] -= 1.0f; //Hacia la derecha

	if (keys['X'])
		LightPos[0] += 1.0f; //Hacia la izquierda

	if (keys['C'])
		LightPos[1] += 1.0f; //Hacia arriba

	if (keys['V'])
		LightPos[1] -= 1.0f; //Hacia abajo

	if (keys['B'])
		LightPos[2] += 1.0f; //Hacia adelante

	if (keys['N'])
		LightPos[2] -= 1.0f; //Hacia atrás
	*/

	return TRUE;
}
