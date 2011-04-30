#include "Main.h"

float Absoluto(float num)
{
	// Si es menor que cero, se regresa su valor absoluto.
	// Se resta de 0 para invertir su signo.
	if(num < 0)
		return (0 - num);

	// Regresa el numero sin cambios si el signo era positivo
	return num;
}
								
CVector Cruz(CVector vVector1, CVector vVector2)
{
	CVector vNormal;									// Es el vector que guardara el producto cruz

	// El valor en X para el vector es:  (V1.y * V2.z) - (V1.z * V2.y)													// Get the X value
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
														
	// El valor en Y para el vector es:  (V1.z * V2.x) - (V1.x * V2.z)
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
														
	// El valor en Z para el vector es:  (V1.x * V2.y) - (V1.y * V2.x)
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vNormal;										// Regresa el producto cruz (Normal)
}

float Punto(CVector vVector1, CVector vVector2)
{
	return(vVector1.x*vVector2.x + vVector1.y*vVector2.y + vVector1.z*vVector2.z);
}

float Magnitud(CVector vNormal)
{
	// Esto regresara la magnitud o "norma" del vector.
	// Esta es la ecuación:  magnitud = sqrt(V.x^2 + V.y^2 + V.z^2)  Donde V is el vector

	return (float)sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z) );
}

CVector Normaliza(CVector vNormal)
{
	float magnitud = Magnitud(vNormal);				// Se obtiene la magnitud del vector

	// Ahora que se tiene la magnitud se puede dividir el vector entre esa magnitud.
	// Esto hara que el vector (p. ejem. una normal) tenga una longitud de 1.  Esto hace mas facil trabajar con el.

	vNormal.x /= magnitud;								// Se divide el valor en X del vector entre su magnitud
	vNormal.y /= magnitud;								// Se divide el valor en Y del vector entre su magnitud
	vNormal.z /= magnitud;								// Se divide el valor en Z del vector entre su magnitud

	// Finalmente, se regresa el vector normalizado.

	return vNormal;										// Se regresa el nuevo vector de longitud 1.
}

void CVector::ExtendVertexPos(CVector currentVertex, CVector lightPos, float Extend)
{
    CVector lightDir;  // Direction of the light to the vertex position.
    CVector newPos;    // New extended vertex position to make up the shadow volume.

    // Get the light direction from the vertex position and the light position.
    lightDir = currentVertex - lightPos;

    // Now that we know where its going we add it to the position of the light so
    // we get the correct, new position.  We multiply it by a passed it value to
    // give the volume some distance or things won't come out quite as we want.
    newPos = lightPos + lightDir * Extend;

    x = newPos.x;
    y = newPos.y;
    z = newPos.z;
}


void CVector::ExtendVertexPos(CVector lightPos, float Extend)
{
    CVector lightDir;  // Dirección de la luz hacia la posición del vértice.
    CVector newPos;    // La posición proyectada de ese vértice en la dirección de la luz.

    // Se obtiene un vector que va de la fuente de luz al vértice.
    lightDir = CVector(x, y, z) - lightPos;

    // Ahora se suma el vector de dirección a la posición de la luz y que gracias
    // al cálculo anterior pasará por el vértice. Antes de hacer la suma el vector
    // se multiplica por la cantidad extendida de la luz. Esto determinará que tan
	// lejos va a estar la posición del pixel proyectado.
    newPos = lightPos + lightDir * Extend;

	// Finalmente se obtiene la posición del pixel proyectado
    x = newPos.x;
    y = newPos.y;
    z = newPos.z;
    
}

#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

// vector negation
CVector3 operator-(CVector3 vVector)
{
	CVector3 vVector1;

	vVector1.x = -vVector.x;
	vVector1.y = -vVector.y;
	vVector1.z = -vVector.z;
	
	return vVector1;
}

/*
//Aqui se sobrecarga el operador * de modo que se pueda multiplicar un vector por un escalar
CVector3 operator*(float num)
{
	CVector3 vVector1;

	vVector1.x = vVector.x*num;
	vVector1.y = vVector.y*num;
	vVector1.z = vVector.z*num;
	
	//Regresa el vector escalado
	return vVector1;
}
*/

// This calculates a vector between 2 points and returns the result
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
	CVector3 vVector;							// The variable to hold the resultant vector

	vVector.x = vPoint1.x - vPoint2.x;			// Subtract point1 and point2 x's
	vVector.y = vPoint1.y - vPoint2.y;			// Subtract point1 and point2 y's
	vVector.z = vPoint1.z - vPoint2.z;			// Subtract point1 and point2 z's

	return vVector;								// Return the resultant vector
}

// This adds 2 vectors together and returns the result
CVector3 AddVector(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vResult;							// The variable to hold the resultant vector
	
	vResult.x = vVector2.x + vVector1.x;		// Add Vector1 and Vector2 x's
	vResult.y = vVector2.y + vVector1.y;		// Add Vector1 and Vector2 y's
	vResult.z = vVector2.z + vVector1.z;		// Add Vector1 and Vector2 z's

	return vResult;								// Return the resultant vector
}

// This divides a vector by a single number (scalar) and returns the result
CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler)
{
	CVector3 vResult;							// The variable to hold the resultant vector
	
	vResult.x = vVector1.x / Scaler;			// Divide Vector1's x value by the scaler
	vResult.y = vVector1.y / Scaler;			// Divide Vector1's y value by the scaler
	vResult.z = vVector1.z / Scaler;			// Divide Vector1's z value by the scaler

	return vResult;								// Return the resultant vector
}

CVector3 MultiplieVectorByScaler(CVector3 vVector1, float Scaler)
{
	CVector3 vResult;							// The variable to hold the resultant vector
	
	vResult.x = vVector1.x * Scaler;			// Divide Vector1's x value by the scaler
	vResult.y = vVector1.y * Scaler;			// Divide Vector1's y value by the scaler
	vResult.z = vVector1.z * Scaler;			// Divide Vector1's z value by the scaler

	return vResult;								// Return the resultant vector
}

// This returns the cross product between 2 vectors
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vCross;								// The vector to hold the cross product
												// Get the X value
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
												// Get the Y value
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
												// Get the Z value
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;								// Return the cross product
}

CVector3 Normalize(CVector3 vNormal)
{
	double Magnitude;							// This holds the magitude			

	Magnitude = Mag(vNormal);					// Get the magnitude

	vNormal.x /= (float)Magnitude;				// Divide the vector's X by the magnitude
	vNormal.y /= (float)Magnitude;				// Divide the vector's Y by the magnitude
	vNormal.z /= (float)Magnitude;				// Divide the vector's Z by the magnitude

	return vNormal;								// Return the normal
}
