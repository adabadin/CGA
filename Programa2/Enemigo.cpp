#include "Enemigo.h"
#include "Bala.h"

Enemigo::Enemigo()
{
	// Just set the index to zero
	i = 0;
	// Set the shooting timer to its default value
	//delay = 115 + rand() % ((200 + 1) - 115 );
	delay = 50;
}

void Enemigo::setPos( float x, float y, float z )
{
	pos.x = x;
	pos.y = y;
	pos.z = z;

	col.Pos = pos;
}

// Reload one bullet
int Enemigo::Reload()
{
	--delay;
	return delay;
}

CVector Enemigo::attack()
{
	// create one bullet vector
	// CVector bull = pos;
	// reset the current delay 
	delay = 90 + rand() % ((110 + 1) - 90 );
	//delay = 100;
	return pos;
}

CVector Enemigo::getPos()
{
	return pos;
}