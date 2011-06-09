#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>		// Archivo de cabecera para Windows
#include <math.h>			// Archivo de cabecera para Funciones Matemáticas
#include <stdio.h>			// Header File For Standard Input/Output
#include <stdlib.h>			// Header File For Standard Library
#include <commctrl.h>
#include <commdlg.h>
#include <direct.h>
#include <string>			// Used for our STL string objects
#include <fstream>
#include <vector>
#include <crtdbg.h>
#include "GL/gl.h"				// Archivo de cabecera para la libreria OpenGL32
#include "GL/glu.h"			// Archivo de cabecera para la libreria GLu32
#include "GL/glaux.h"			// Archivo de cabecera para la libreria Glaux
#include "GL/glext.h"			// Archivo de cabecera para la libreria Glext
#include "Vector.h"
#include "cargadorTGA.h"
#include "Shaders.h"
#include "Multitexturas.h"
#include "Materiales.h"
#include "Font.h"
#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

int ProcessKeys(FMOD_SYSTEM *system, FMOD_RESULT result);

using namespace std;

#define GL_CLAMP_TO_EDGE	0x812F

extern HWND hWnd;

void Camara(int funcion);
int  ManejaTeclado();

struct tLine
{
	unsigned short a,b;
};

//Estructura para un plano de la forma: ax + by + cz + d = 0
struct Plane
{
	float a, b, c, d;
};

struct tFace
{
	int vertIndex[3];
	int coordIndex[3];
	CVector3 Normal;
	Plane planeEquation;		// Ecuación del plano que contiene el poligono
	bool isFacingLight;
	int neighbourIndices[3];	// Indice de cada poligono adyacente a los lados del poligono revisado
	bool isSilhouetteEdge[3];
	bool visible;				// Poligonos visibles a la fuente de luz
	
};

struct tMaterialInfo
{
	char  strName[255];
	char  strFile[255];
	BYTE  color[3];
	float fColor[3];
	int   texureId;
	float uTile;
	float vTile;
	float uOffset;
	float vOffset;
	int  hasNormalMap;
	int  indexNM;
};

struct t3DObject 
{
	int  numOfVerts;
	int  numOfFaces;
	int  numTexVertex;
	int  materialID;
	bool bHasTexture;
	bool isInGroup;
	int  inGroupNum;
	int  shadeModel;
	char strName[255];
	CVector3  *pVerts;
	CVector3  *pNormals;
	CVector3  *pTangents;
	CVector3  *pBinormals;
	CVector2  *pTexVerts;
	vector<tLine> silhouette;
	
	tFace *pFaces;

};

struct t3DModel 
{
	int numOfObjects;
	int numOfMaterials;
	int numOfNormalMaps;
	int numOfLightMaps;
	int numOfGroups;
	vector<tMaterialInfo> pMaterials;
	vector<tMaterialInfo> pNormalMaps;
	vector<tMaterialInfo> pLightMaps;
	vector<t3DObject> pObject;

	int contPol;
	int contVert;
	int contMat;
	int contGroups;
	
};

struct parametros
{
	CVector PosicionObj;	//La posición (x,y,z) del objeto
	CVector PosicionObjA;	//La posición (x,y,z) del objeto
	CVector Direccion;		//La dirección en que se dirige el objeto en forma de vector=(x,y,z)
	CVector PosicionCam;    //La posición de la cámara que sigue al objeto
	CVector ObjetivoCam;	//El punto (x,y,z) que está viendo la cámara; en este caso al objeto
	CVector PosAntObj;	//La posición (x,y,z) del objeto

	float VelocidadObj;		//La velocidad a la que se mueve el objeto
	float DistanciaCam;		//La distancia que la cámara está separada del objeto
	float AngDir;			//Se usa para llevar control del angulo para el cálculo del vector de dirección
	float AngObj;			//Se usa para llevar control del ángulo de rotación para el modelo del objeto

	float CamaraPosAlt;		//Se usa para definir y cambiar si es necesario la altura de la cámara
	float CamaraObjAlt;		//Se usa para definir y cambiar si es necesario la altura del objetivo de la cámara

	float CamaraObjAltE;

	float escalaX;
	float escalaY;
	float escalaZ;

	int Dir;				//1: Hacia adelante; 2: hacia atras; 3: hacia la izquierda; 4: hacia la derecha
	int DirAnt;

	bool visible;
	bool caminando;
	bool saltando;
	bool disparando;
	
};

struct spline {
	CVector *puntosctrl;
	int	    totalPuntosControl;
	int	    puntosaDibujar;
	int	    puntosporTramo;
};

//Colisiones
struct esfera
{
	CVector Pos;
	float radio;
	bool colision;
	
};

struct rayo
{
	CVector origen;
	CVector dir;
	bool colision;
};


#endif 