#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Main.h"
#include "Bala.h"

class Enemigo
{
public: 
	Enemigo();

	void setPos( float x, float y, float z );
	CVector attack();
	int Reload();
	CVector getPos();

	int delay;
	int i;
	CVector pos;
	esfera col;

};

#endif ENEMIGO_H