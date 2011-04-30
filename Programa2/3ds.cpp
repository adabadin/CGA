#include "main.h"
#include "3ds.h"
#include <assert.h>

int gBuffer[50000] = {0};

CLoad3DS::CLoad3DS()
{
	m_FilePointer = NULL;
}

bool CLoad3DS::Import3DS(t3DModel *pModel, char *strFileName)
{
	char strMessage[255] = {0};
	tChunk currentChunk = {0};

	m_FilePointer = fopen(strFileName, "rb");

	if(!m_FilePointer) 
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	ReadChunk(&currentChunk);

	if (currentChunk.ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	ProcessNextChunk(pModel, &currentChunk);

	ComputeNormals(pModel);

	CleanUp();

	return true;
}

void CLoad3DS::CleanUp()
{
	if (m_FilePointer) {
		fclose(m_FilePointer);
		m_FilePointer = NULL;
	}
}

void CLoad3DS::ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	t3DObject newObject = {0};
	tMaterialInfo newTexture = {0};	

	tChunk currentChunk = {0};
	tChunk tempChunk = {0};		

	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		ReadChunk(&currentChunk);

		switch (currentChunk.ID)
		{
		case VERSION:
			
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);

			if ((currentChunk.length - currentChunk.bytesRead == 4) && (gBuffer[0] > 0x03)) {
				MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
			}
			break;

		case OBJECTINFO:
			{	
			ReadChunk(&tempChunk);

			tempChunk.bytesRead += fread(gBuffer, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

			currentChunk.bytesRead += tempChunk.bytesRead;

			ProcessNextChunk(pModel, &currentChunk);
			break;
		}
		case MATERIAL:

			pModel->numOfMaterials++;

			pModel->pMaterials.push_back(newTexture);

			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case OBJECT:
				
			pModel->numOfObjects++;
		
			pModel->pObject.push_back(newObject);
			
			memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));

			currentChunk.bytesRead += GetString(pModel->pObject[pModel->numOfObjects - 1].strName);
			
			ProcessNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), &currentChunk);
			break;

		case EDITKEYFRAME:

			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;

		default: 
			
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}

void CLoad3DS::ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	tChunk currentChunk = {0};

	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		ReadChunk(&currentChunk);

		switch (currentChunk.ID)
		{
		case OBJECT_MESH:
		
			ProcessNextObjectChunk(pModel, pObject, &currentChunk);
			break;

		case OBJECT_VERTICES:
			ReadVertices(pObject, &currentChunk);
			break;

		case OBJECT_FACES:
			ReadVertexIndices(pObject, &currentChunk);
			break;

		case OBJECT_MATERIAL:
						
			ReadObjectMaterial(pModel, pObject, &currentChunk);			
			break;

		case OBJECT_UV:

			ReadUVCoordinates(pObject, &currentChunk);
			break;

		default:  

			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}

void CLoad3DS::ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	tChunk currentChunk = {0};

	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		ReadChunk(&currentChunk);

		switch (currentChunk.ID)
		{
		case MATNAME:
			
			currentChunk.bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;

		case MATDIFFUSE:
			ReadColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), &currentChunk);
			break;
		
		case MATMAP:
			
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case MATMAPFILE:

			currentChunk.bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		
		default:  

			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}

void CLoad3DS::ReadChunk(tChunk *pChunk)
{
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);

	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

int CLoad3DS::GetString(char *pBuffer)
{
	int index = 0;

	fread(pBuffer, 1, 1, m_FilePointer);

	while (*(pBuffer + index++) != 0) {

		fread(pBuffer + index, 1, 1, m_FilePointer);
	}

	return strlen(pBuffer) + 1;
}

void CLoad3DS::ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk)
{
	tChunk tempChunk = {0};

	ReadChunk(&tempChunk);

	tempChunk.bytesRead += fread(pMaterial->color, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

	pChunk->bytesRead += tempChunk.bytesRead;
}

void CLoad3DS::ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	unsigned short index = 0;

	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);

	for(int i = 0; i < pObject->numOfFaces; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

			if(j < 3)
				pObject->pFaces[i].vertIndex[j] = index;
			
		}
	}
}

void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);

	pObject->pTexVerts = new CVector2 [pObject->numTexVertex];

	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}

void CLoad3DS::ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

	pObject->pVerts = new CVector3 [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(CVector3) * pObject->numOfVerts);

	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);

	for(int i = 0; i < pObject->numOfVerts; i++)
	{
		float fTempY = pObject->pVerts[i].y;

		pObject->pVerts[i].y = pObject->pVerts[i].z;

		pObject->pVerts[i].z = -fTempY;
	}

	
}

void CLoad3DS::ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	char strMaterial[255] = {0};

	pPreviousChunk->bytesRead += GetString(strMaterial);

	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
		{
			if(strlen(pModel->pMaterials[i].strFile) > 0) 
			{				
				pObject->materialID = i;

				pObject->bHasTexture = true;
			}	
			break;
		}
		else
		{
			if(pObject->bHasTexture != true)
			{
				pObject->materialID = -1;
			}
		}
	}

	pPreviousChunk->bytesRead += fread(gBuffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}			

bool CLoad3DS::Load3DSFile(char *FileName, t3DModel *pModel, CTga *Textura)
{
	char sZTexturePath[256];
	char sZTextureName[30];

	strcpy(sZTexturePath,"Texturas/");

	if(!Import3DS(pModel, FileName))
		return 0;

	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		if(strlen(pModel->pMaterials[i].strFile) > 0)
		{
			strcpy(sZTextureName,pModel->pMaterials[i].strFile);
			strcat(sZTexturePath,sZTextureName);

			Textura[i].LoadTGA(sZTexturePath);

			strcpy(sZTexturePath,"");
			strcpy(sZTextureName,"");
			strcpy(sZTexturePath,"Texturas/");
		}

		pModel->pMaterials[i].texureId = i;
	}

	return TRUE;
}

void CLoad3DS::Render3DSFile(t3DModel *pModel, CTga *Textura, int tipo)
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
				
					if(pObject->bHasTexture) {

						if(pObject->pTexVerts) {
							glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
						}
					} else {

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

void CLoad3DS::UnLoad3DSFile(t3DModel *pModel, CTga *Textura)
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

void CLoad3DS::ComputeNormals(t3DModel *pModel)
{
	CVector3 vVector1, vVector2, vNormal, vPoly[3];

	if(pModel->numOfObjects <= 0)
	
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

			//Normales por cara

			vVector1 = Vector(vPoly[0], vPoly[2]);
			vVector2 = Vector(vPoly[2], vPoly[1]);

			vNormal  = Cross(vVector1, vVector2);
			pTempNormals[i] = vNormal;	
			vNormal  = Normalize(vNormal);

			pNormals[i] = vNormal;	
		}

		//Normales por vértice

		CVector3 vSum = {0.0, 0.0, 0.0};
		CVector3 vZero = vSum;
		int shared=0;

		for (int i = 0; i < pObject->numOfVerts; i++)
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
