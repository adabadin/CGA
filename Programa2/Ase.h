#ifndef _ASE_H
#define _ASE_H

#include "Main.h"

#define OBJECT				"*GEOMOBJECT"
#define NAME				"*NODE_NAME"
#define SHADEMODEL			"*SHADE_MODEL"
#define GROUP				"*GROUP"
#define PARENT				"*NODE_PARENT"
#define NUM_VERTEX			"*MESH_NUMVERTEX"
#define NUM_FACES			"*MESH_NUMFACES"
#define NUM_TVERTEX			"*MESH_NUMTVERTEX"
#define VERTEX				"*MESH_VERTEX"
#define FACE				"*MESH_FACE"
#define NORMALS				"*MESH_NORMALS"
#define FACE_NORMAL			"*MESH_FACENORMAL"
#define NVERTEX				"*MESH_VERTEXNORMAL"
#define TVERTEX				"*MESH_TVERT"
#define TFACE				"*MESH_TFACE"
#define TEXTURE				"*BITMAP"
#define UTILE				"*UVW_U_TILING"
#define VTILE				"*UVW_V_TILING"
#define UOFFSET				"*UVW_U_OFFSET"
#define VOFFSET				"*UVW_V_OFFSET"
#define MATERIAL_ID			"*MATERIAL_REF"
#define MATERIAL_COUNT      "*MATERIAL_COUNT"
#define MATERIAL			"*MATERIAL"
#define MATERIAL_NAME		"*MATERIAL_NAME"
#define MATERIAL_COLOR		"*MATERIAL_DIFFUSE"
/*
#define NORMALMAP_FLAG		"*NORMALMAP"
#define LIGHTMAP_FLAG		"*LIGHTMAP"
#define INDEX_NORMALMAP		"*INDEXNORMALMAP"
#define INDEX_LIGHTMAP		"*INDEXLIGHTMAP"
#define NORMALMAP_COUNT     "*NORMALMAP_COUNT"
#define NORMALMAP_ID		"*NORMALMAP_ID"
#define NORMALMAPTEXT		"*NORMALMAPBITMAP"
*/

class CLoadASE 
{

public:

	bool ImportASE(t3DModel *pModel, char *strFileName);
	bool LoadAseFile(char *FileName, t3DModel *pModel, CTga *Textura);
	void UnLoadASEFile(t3DModel *pModel, CTga *Textura);
	void RenderASEFile(t3DModel *pModel, CTga *Textura, int tipo);
		
	void ReadAseFile(t3DModel *pModel);
	int GetObjectCount();
	int GetMaterialCount();
	//int GetNormalMapCount();
	void GetTextureInfo(tMaterialInfo *pTexture, int desiredMaterial);
	//void GetNormalMapInfo(tMaterialInfo *pTexture, int desiredNormalMap);
	
	void MoveToObject (int desiredObject);
	float ReadFloat();

	void ReadObjectInfo(t3DModel *pModel, t3DObject *pObject, int desiredObject);

	void GetTextureName (tMaterialInfo  *pTexture);
	void GetMaterialName(tMaterialInfo *pTexture);

	void ReadObjectData(t3DModel *pModel, t3DObject *pObject, int desiredObject);
	void GetData(t3DModel *pModel, t3DObject *pObject, char *strDesiredData, int desiredObject);

	void ReadVertex(t3DObject *pObject);
	
	void ReadTextureVertex(t3DObject *pObject, tMaterialInfo texture);
	void ReadFace(t3DObject *pObject);
	void ReadTextureFace(t3DObject *pObject);

	void ComputeNormals(t3DModel *pModel);
	void ComputeTangents(t3DModel *pModel);

private:
	
	FILE *m_FilePointer;
};

#endif
