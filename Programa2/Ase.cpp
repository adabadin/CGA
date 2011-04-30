#include "Main.h"
#include "Ase.h"


bool CLoadASE::ImportASE(t3DModel *pModel, char *strFileName)
{
	char strMessage[255] = {0};

	if(!pModel || !strFileName) return false;

	m_FilePointer = fopen(strFileName, "r");

	if(!m_FilePointer) {
		sprintf(strMessage, "Unable to find or open the file: %s", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	ReadAseFile(pModel);

	ComputeNormals(pModel);
	ComputeTangents(pModel);

	fclose(m_FilePointer);

	return true;
}

bool CLoadASE::LoadAseFile(char *FileName, t3DModel *pModel, CTga *Textura)
{

	ImportASE(pModel, FileName);

	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		if(strlen(pModel->pMaterials[i].strFile) > 0)
		{
			Textura[i].LoadTGA(pModel->pMaterials[i].strFile);
		}

		pModel->pMaterials[i].texureId = i;
	}

	return TRUE;
}

void CLoadASE::UnLoadASEFile(t3DModel *pModel, CTga *Textura)
{
	for(int i = 0; i < pModel->numOfObjects; i++)
	{
		delete [] pModel->pObject[i].pFaces;
		delete [] pModel->pObject[i].pNormals;
		delete [] pModel->pObject[i].pVerts;
		delete [] pModel->pObject[i].pTexVerts;
	}

	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		glDeleteTextures(1, &Textura[i].texID);
	}

}

void CLoadASE::RenderASEFile(t3DModel *pModel, CTga *Textura, int tipo)
{
	for(int i = 0; i < pModel->numOfObjects; i++)
	{
		if(pModel->pObject.size() <= 0) break;

		t3DObject *pObject = &pModel->pObject[i];
			
		if(pObject->bHasTexture) {

			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
			glBindTexture(GL_TEXTURE_2D, Textura[pObject->materialID].texID);
	
		} else {

			glDisable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
		}

		if(tipo == 1)
			glBegin(GL_TRIANGLES);
		else if(tipo == 2)
		{
			glLineWidth(30.0f);
			glPolygonMode(GL_FRONT, GL_LINE);
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLES);
			glColor3f(0.0f,0.0f,0.0f);
		}
			for(int j = 0; j < pObject->numOfFaces; j++)
			{
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{
					int index = pObject->pFaces[j].vertIndex[whichVertex];
			
					glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
					
					if(pObject->bHasTexture) 
					{
						if(pObject->pTexVerts) 
						{
							int coordIndex = pObject->pFaces[j].coordIndex[whichVertex];
							glTexCoord2f(pObject->pTexVerts[ coordIndex ].x, pObject->pTexVerts[ coordIndex ].y);
						
						}
					} 
					else 
					{
						if(pModel->pMaterials.size() && pObject->materialID >= 0) 
						{
							BYTE *pColor = pModel->pMaterials[pObject->materialID].color;
							glColor3ub(pColor[0], pColor[1], pColor[2]);
						}
					}

					glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
					
				}
			}

		glEnd();

		glDisable(GL_TEXTURE_2D);
		glColor3f(1.0f,1.0f,1.0f);
	}
}

void CLoadASE::ComputeTangents(t3DModel *pModel)
{
	CVector3 vVector1, vVector2, vTangent, vBinormal, vPoly[3];
	float vVector1s, vVector1t, vVector2s, vVector2t;
	float v1s, v1t, v2s, v2t, v3s, v3t;

	CVector3 vSum = {0.0, 0.0, 0.0};
	CVector3 vZero = vSum;
	int shared=0;

	if(pModel->numOfObjects <= 0)
		return;

	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		t3DObject *pObject = &(pModel->pObject[index]);

		CVector3 *pTangents		= new CVector3 [pObject->numOfFaces];
		CVector3 *pTempTangents	= new CVector3 [pObject->numOfFaces];
		pObject->pTangents		= new CVector3 [pObject->numOfVerts];
	
		CVector3 *pBinormals	 = new CVector3 [pObject->numOfFaces];
		CVector3 *pTempBinormals = new CVector3 [pObject->numOfFaces];
		pObject->pBinormals		 = new CVector3 [pObject->numOfVerts];
		
		for(int i=0; i < pObject->numOfFaces; i++)
		{												
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			v1s = pObject->pTexVerts[pObject->pFaces[i].coordIndex[0]].x;
			v1t = pObject->pTexVerts[pObject->pFaces[i].coordIndex[0]].y;

			v2s = pObject->pTexVerts[pObject->pFaces[i].coordIndex[1]].x;
			v2t = pObject->pTexVerts[pObject->pFaces[i].coordIndex[1]].y;

			v3s = pObject->pTexVerts[pObject->pFaces[i].coordIndex[2]].x;
			v3t = pObject->pTexVerts[pObject->pFaces[i].coordIndex[2]].y;

			vVector1 = Vector(vPoly[1], vPoly[0]);
			vVector2 = Vector(vPoly[2], vPoly[0]);

			vVector1s = v2s-v1s;
			vVector1t = v2t-v1t;

			vVector2s = v3s-v1s;
			vVector2t = v3t-v1t;

			float denominator = vVector1s * vVector2t - vVector2s * vVector1t;

			if(denominator < 0.0001f && denominator > -0.0001f)
			{
				vTangent.x = 1.0f;
				vTangent.y = 0.0f;
				vTangent.z = 0.0f;

				vBinormal.x = 0.0f;
				vBinormal.y = 1.0f;
				vBinormal.z = 0.0f;

				pTempTangents[i] = vTangent;
				pTempBinormals[i] = vBinormal;
			}
			else
			{
				float scale = 1.0f / denominator;

				CVector3 T, B, N;

				T.x = (vVector2t * vVector1.x - vVector1t * vVector2.x) * scale;
				T.y = (vVector2t * vVector1.y - vVector1t * vVector2.y) * scale;
				T.z = (vVector2t * vVector1.z - vVector1t * vVector2.z) * scale;

				B.x = (-vVector2s * vVector1.x + vVector1s * vVector2.x) * scale;
				B.y = (-vVector2s * vVector1.y + vVector1s * vVector2.y) * scale;
				B.z = (-vVector2s * vVector1.z + vVector1s * vVector2.z) * scale;

				N = pObject->pFaces[i].Normal;

				float scale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) + 
									   (B.x * N.y * T.z - B.z * N.y * T.x) + 
									   (N.x * T.y * B.z - N.z * T.y * B.x));

				vTangent.x =  Cross(B, N).x * scale2;
				vTangent.y = -Cross(N, T).x * scale2;
				vTangent.z =  Cross(T, B).x * scale2;
				pTempTangents[i] = vTangent;
				vTangent=Normalize(vTangent);

				vBinormal.x = -Cross(B, N).y * scale2;
				vBinormal.y =  Cross(N, T).y * scale2;
				vBinormal.z = -Cross(T, B).y * scale2;
				pTempBinormals[i] = vBinormal;
				vBinormal=Normalize(vBinormal);
	
			}
					
		}
		
		vZero = vSum;
		shared=0;

		for(int i = 0; i < pObject->numOfVerts; i++)
		{
			for (int j = 0; j < pObject->numOfFaces; j++)
			{			
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempTangents[j]);
					shared++;
				}
			}      
			
			pObject->pTangents[i] = DivideVectorByScaler(vSum, float(shared));
			pObject->pTangents[i] = Normalize(pObject->pTangents[i]);	

			vSum = vZero;
			shared = 0;
		}
	
		delete [] pTempTangents;
		delete [] pTangents;

		vZero = vSum;
		shared=0;

		for(int i = 0; i < pObject->numOfVerts; i++)
		{
			for (int j = 0; j < pObject->numOfFaces; j++)
			{											
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempBinormals[j]);
					shared++;
				}
			}      
			
			pObject->pBinormals[i] = DivideVectorByScaler(vSum, float(-shared));
			pObject->pBinormals[i] = Normalize(pObject->pBinormals[i]);	

			vSum = vZero;
			shared = 0;
		}
	
		delete [] pTempBinormals;
		delete [] pBinormals;
		
	}
	
}

void CLoadASE::ReadAseFile(t3DModel *pModel)
{
	tMaterialInfo newMaterial = {0};
	tMaterialInfo newNormalMap = {0};
	t3DObject     newObject   = {0};
				
	int i = 0;	

	pModel->numOfObjects   = GetObjectCount();

	pModel->numOfMaterials = GetMaterialCount();

	for(i = 0; i < pModel->numOfMaterials; i++)
	{
		pModel->pMaterials.push_back(newMaterial);
		GetTextureInfo(&(pModel->pMaterials[i]), i + 1);
	}

	for(i = 0; i < pModel->numOfObjects; i++)
	{	
		pModel->pObject.push_back(newObject);
		pModel->pObject[i].materialID = -1;

		MoveToObject(i + 1);

		ReadObjectInfo(pModel, &(pModel->pObject[i]), i + 1);
		ReadObjectData(pModel, &(pModel->pObject[i]), i + 1);
		
	}

}

int CLoadASE::GetObjectCount()
{
	char strWord[255] = {0};
	int objectCount = 0;

	rewind(m_FilePointer);

	while (!feof(m_FilePointer))
	{
		fscanf(m_FilePointer, "%s", &strWord);

		if (!strcmp(strWord, OBJECT))
		{
			objectCount++;
		}
		else
		{
			fgets(strWord, 100, m_FilePointer);
		}
	}

	return objectCount;
}

int CLoadASE::GetMaterialCount()
{
	char strWord[255] = {0};
	int materialCount = 0;

	rewind(m_FilePointer);

	while (!feof(m_FilePointer))
	{
		fscanf(m_FilePointer, "%s", &strWord);

		if (!strcmp(strWord, MATERIAL_COUNT))
		{
			fscanf(m_FilePointer, "%d", &materialCount);

			return materialCount;
		}
		else
		{
			fgets(strWord, 100, m_FilePointer);
		}
	}

	return 0;
}

void CLoadASE::GetTextureInfo (tMaterialInfo *pTexture, int desiredMaterial)
{
	char strWord[255] = {0};
	int materialCount= 0;
	
	rewind(m_FilePointer);

	while (!feof(m_FilePointer))
	{
		fscanf(m_FilePointer, "%s", &strWord);

		if (!strcmp(strWord, MATERIAL))
		{
			materialCount++;

			if(materialCount == desiredMaterial)
				break;
		}
		else
		{
			fgets(strWord, 100, m_FilePointer);
		}
	}

	while (!feof(m_FilePointer))
	{
		fscanf(m_FilePointer, "%s", &strWord);

		if (!strcmp (strWord, MATERIAL))
		{
			return;
		}
		else if (!strcmp(strWord, MATERIAL_COLOR))
		{
			fscanf(m_FilePointer, " %f %f %f", &(pTexture->fColor[0]), 
											   &(pTexture->fColor[1]), 
											   &(pTexture->fColor[2]));
		}
		else if (!strcmp(strWord, TEXTURE))
		{
			GetTextureName(pTexture);
		}
		else if (!strcmp(strWord, MATERIAL_NAME))
		{
			GetMaterialName(pTexture);
		}
		else if(!strcmp(strWord, UTILE))
		{
			pTexture->uTile = ReadFloat();
		}
		else if(!strcmp(strWord, VTILE))
		{
			pTexture->vTile = ReadFloat();
		}
		else
		{
			fgets (strWord, 100, m_FilePointer);
		}
	}
}

void CLoadASE::MoveToObject (int desiredObject)
{
	char strWord[255] = {0};
	int objectCount = 0;

	rewind(m_FilePointer);

	while(!feof(m_FilePointer))
	{
		fscanf(m_FilePointer, "%s", &strWord);

		if(!strcmp(strWord, OBJECT))
		{
			objectCount++;

			if(objectCount == desiredObject)
				return;
		}
		else
		{
			fgets(strWord, 100, m_FilePointer);
		}
	}
}

float CLoadASE::ReadFloat()
{
	float v = 0.0f;

	fscanf(m_FilePointer, " %f", &v);
	return v;
}

void CLoadASE::ReadObjectInfo(t3DModel *pModel, t3DObject *pObject, int desiredObject)
{
	char strWord[255] = {0};

	MoveToObject(desiredObject);
	while (!feof(m_FilePointer))
	{
		fscanf(m_FilePointer, "%s", &strWord);

		if (!strcmp(strWord, NAME))
		{
			fscanf(m_FilePointer, "%s", &pObject->strName);

		}
		else if (!strcmp(strWord, PARENT))
		{
			pObject->isInGroup=TRUE;

		}
		else if (!strcmp(strWord, NUM_VERTEX))
		{
			fscanf(m_FilePointer, "%d", &pObject->numOfVerts);

			pObject->pVerts = new CVector3 [pObject->numOfVerts];
		}
		else if (!strcmp(strWord, NUM_FACES))
		{
			fscanf(m_FilePointer, "%d", &pObject->numOfFaces);

			pObject->pFaces = new tFace [pObject->numOfFaces];
		}
		else if (!strcmp(strWord, NUM_TVERTEX))
		{
			fscanf(m_FilePointer, "%d", &pObject->numTexVertex);

			pObject->pTexVerts = new CVector2 [pObject->numTexVertex];
			
		}
		else if (!strcmp(strWord, OBJECT))	
		{
			return;
		}
		else 
		{
			fgets(strWord, 100, m_FilePointer);
		}
	}	
}

void CLoadASE::GetTextureName(tMaterialInfo *pTexture)
{
	fscanf (m_FilePointer, " \"%s", &(pTexture->strFile));
	
	pTexture->strFile[strlen (pTexture->strFile) - 1] = '\0';
}

void CLoadASE::GetMaterialName(tMaterialInfo *pTexture)
{
	fscanf (m_FilePointer, " \"%s", &(pTexture->strName));
	
	pTexture->strName[strlen (pTexture->strName)] = '\0';
}

void CLoadASE::ReadObjectData(t3DModel *pModel, t3DObject *pObject, int desiredObject)
{
	GetData(pModel, pObject, MATERIAL_ID, desiredObject);

	GetData(pModel, pObject, SHADEMODEL, desiredObject);
	
	GetData(pModel, pObject, VERTEX,		 desiredObject);

	GetData(pModel, pObject, TVERTEX,	 desiredObject);

	GetData(pModel, pObject, FACE,		 desiredObject);

	GetData(pModel, pObject, TFACE,		 desiredObject);

	GetData(pModel, pObject, TEXTURE,	 desiredObject);

	GetData(pModel, pObject, UTILE,		 desiredObject);

	GetData(pModel, pObject, VTILE,		 desiredObject);
}

void CLoadASE::GetData(t3DModel *pModel, t3DObject *pObject, char *strDesiredData, int desiredObject)
{
	char strWord[255] = {0};
	static int faceNum=0;

	MoveToObject(desiredObject);

	while(!feof(m_FilePointer))
	{
		fscanf(m_FilePointer, "%s", &strWord);

		if(!strcmp(strWord, OBJECT))	
		{
			return;
		}
		else if(!strcmp(strWord, VERTEX))
		{
			if(!strcmp(strDesiredData, VERTEX)) 
			{
				ReadVertex(pObject);
			}
		}
		else if(!strcmp(strWord, TVERTEX))
		{
			if(!strcmp(strDesiredData, TVERTEX)) 
			{
				ReadTextureVertex(pObject, pModel->pMaterials[pObject->materialID]);
			}
		}
		else if(!strcmp(strWord, FACE))
		{
			if(!strcmp(strDesiredData, FACE)) 
			{
				ReadFace(pObject);
			}
		}
		else if(!strcmp(strWord, TFACE))
		{
			if(!strcmp(strDesiredData, TFACE))
			{
				ReadTextureFace(pObject);
			}
		}
		else if(!strcmp(strWord, SHADEMODEL))
		{
			if(!strcmp(strDesiredData, SHADEMODEL))
			{
				pObject->shadeModel = (int)ReadFloat();
				return;
			}				
		}
		else if(!strcmp(strWord, MATERIAL_ID))
		{
			if(!strcmp(strDesiredData, MATERIAL_ID))
			{
				pObject->materialID = (int)ReadFloat();
				return;
			}				
		}
		else 
		{
			fgets(strWord, 100, m_FilePointer);
		}
		
	}
}

void CLoadASE::ReadVertex(t3DObject *pObject)
{
	int index = 0;

	fscanf(m_FilePointer, "%d", &index);
	
	fscanf(m_FilePointer, "%f %f %f", &pObject->pVerts[index].x, 
									  &pObject->pVerts[index].z,
									  &pObject->pVerts[index].y);
		
	pObject->pVerts[index].z = -pObject->pVerts[index].z;
}

void CLoadASE::ReadTextureVertex(t3DObject *pObject, tMaterialInfo texture)
{
	int index = 0;

	fscanf(m_FilePointer, "%d", &index);

	fscanf(m_FilePointer, "%f %f", &(pObject->pTexVerts[index].x), &(pObject->pTexVerts[index].y));

	pObject->pTexVerts[index].x *= texture.uTile;
	pObject->pTexVerts[index].y *= texture.vTile;

	pObject->bHasTexture = true;
}

void CLoadASE::ReadFace(t3DObject *pObject)
{
	int index = 0;

	fscanf(m_FilePointer, "%d:", &index);

	fscanf(m_FilePointer, "\tA:\t%d B:\t%d C:\t%d", &(pObject->pFaces[index].vertIndex[0]), 
													&(pObject->pFaces[index].vertIndex[1]), 
													&(pObject->pFaces[index].vertIndex[2])); 
}

void CLoadASE::ReadTextureFace(t3DObject *pObject)
{
	int index = 0;

	fscanf(m_FilePointer, "%d:", &index);

	fscanf(m_FilePointer, "%d %d %d", &pObject->pFaces[index].coordIndex[0], 
									  &pObject->pFaces[index].coordIndex[1], 
									  &pObject->pFaces[index].coordIndex[2]);
}

void CLoadASE::ComputeNormals(t3DModel *pModel)
{
	CVector3 vVector1, vVector2, vNormal, vPoly[3];

	if(pModel->numOfObjects <= 0)
		return;

	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		t3DObject *pObject = &(pModel->pObject[index]);

		CVector3 *pNormals		= new CVector3 [pObject->numOfFaces];
		CVector3 *pTempNormals	= new CVector3 [pObject->numOfFaces];
		pObject->pNormals		= new CVector3 [pObject->numOfVerts];

		for(int i=0; i < pObject->numOfFaces; i++)
		{												
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			vVector1 = Vector(vPoly[0], vPoly[2]);
			vVector2 = Vector(vPoly[2], vPoly[1]);

			vNormal  = Cross(vVector1, vVector2);
			pTempNormals[i] = vNormal;			
			vNormal  = Normalize(vNormal);
			pObject->pFaces[i].Normal=MultiplieVectorByScaler(vNormal,-1.0f);
			pNormals[i] = vNormal;
		}

		CVector3 vSum = {0.0, 0.0, 0.0};
		CVector3 vZero = vSum;
		int shared=0;

		for(int i = 0; i < pObject->numOfVerts; i++)
		{
			for (int j = 0; j < pObject->numOfFaces; j++)
			{											
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);
					shared++;								
				}
			}      
			
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));

			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);	

			vSum = vZero;									
			shared = 0;										
		}
	
		delete [] pTempNormals;
		delete [] pNormals;
	}
}
