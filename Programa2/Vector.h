#ifndef _VECTOR_H
#define _VECTOR_H

#define PI 3.1415926535897932

class CVector3 
{
public:
	float x, y, z;
};

class CVector2 
{
public:
	float x, y;
};

CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2);
CVector3 AddVector(CVector3 vVector1, CVector3 vVector2);
CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler);
CVector3 MultiplieVectorByScaler(CVector3 vVector1, float Scaler);
CVector3 Cross(CVector3 vVector1, CVector3 vVector2);
//float Punto(CVector3 vVector1, CVector3 vVector2);
CVector3 Normalize(CVector3 vNormal);
float Angle(CVector3 vVector1, CVector3 vVector2);

class CVector
{
	public:

		//Constructor
		CVector() {}

		// Este constructor permite inicializar los datos
		CVector(float X, float Y, float Z) 
		{ 
			x = X; y = Y; z = Z;
		}

		CVector operator+(CVector vVector)
		{
			//Regresa el vector resultante de la suma
			return CVector(vVector.x + x, vVector.y + y, vVector.z + z);
		}

		//Aqui se sobrecarga el operador - de modo que se puedan restar vectores
		CVector operator-(CVector vVector)
		{
			//Regresa el vector resultante de la resta
			return CVector(x - vVector.x, y - vVector.y, z - vVector.z);
		}
		
		//Aqui se sobrecarga el operador * de modo que se pueda multiplicar un vector por un escalar
		CVector operator*(float num)
		{
			//Regresa el vector escalado
			return CVector(x * num, y * num, z * num);
		}

		//Aqui se sobrecarga el operador / de modo que se pueda dividir un vector entre un escalar
		CVector operator/(float num)
		{
			//Regresa el vector escalado (reducido)
			return CVector(x / num, y / num, z / num);
		}

		void ExtendVertexPos(CVector lightPos, float Extend);   // Extend point based on light position
                                           // and currend point, saves here.
		void ExtendVertexPos(CVector currentVertex, CVector lightPos, float Extend);    // Takes current point and source and
                                       // extend and saves here.

		float x, y, z;
		
};

//Esto regresa el valor absoluto de "num"
float Absoluto(float num);

//Esto regresa un vector perpendicular a 2 vectors dados, por medio del producto cruz.
CVector Cruz(CVector vVector1, CVector vVector2);

float Punto(CVector vVector1, CVector vVector2);

//Esto regresa la magnitud de una normal (o cualquier otro vector)
float Magnitud(CVector vNormal);

//Esto regresa un vector normalizado (Un vector de longitud 1)
CVector Normaliza(CVector vNormal);

#endif
