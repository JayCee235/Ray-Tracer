#ifndef __LIGHTSOURCE
#define __LIGHTSOURCE

#include "vector/GenVector.h"

class Light
{
private:
	Vector3 p;
	Color ambient, diffuse, specular;
public:
	Light() {}
	Light(Vector3 p, Color a, Color d, Color s) {
		this->p = p;
		this->ambient = a;
		this->diffuse = d;
		this->specular = s;
	}

	~Light() {};

	/* data */
};

#endif