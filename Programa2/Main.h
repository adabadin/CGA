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

struct boundingplane
{
	CVector Normal;
	CVector PM;
	CVector A, B, C, D;
	CVector b1, b2, b3, b4;
	CVector b1Pm, b2Pm, b3Pm, b4Pm;
	CVector b1Normal, b2Normal, b3Normal, b4Normal;
	float ancho;
	float alto;
};

struct boundingsphere
{
	CVector Pos;
	float radio;
	bool colision;
	
};

struct spline {
	CVector *puntosctrl;
	int	    totalPuntosControl;
	int	    puntosaDibujar;
	int	    puntosporTramo;
};

struct esfera {
	CVector centro;
	float radio;
	bool colisionesfera;
};

struct rayo {
	CVector origen;
	CVector direccion;
	bool colisionrayo;
};


#endif 