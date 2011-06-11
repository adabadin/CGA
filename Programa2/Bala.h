#ifndef BULLET_H
#define BULLET_H

#include "Main.h"

class Bala
{

public:

	// Constructor
	Bala( CVector origen, CVector destino );

	void cPara( float vel, float radius, CVector destino );
	void dBala();
	void moveToTarget();
	void updateTarget( CVector target );
	esfera getSphere();

	float speed;
	esfera bala;
	CVector sTarget;

};

#endif // BULLET_H