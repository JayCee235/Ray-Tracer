#ifndef __LIGHTSOURCE
#define __LIGHTSOURCE

#include "vector/GenVector.h"

class Light
{
private:
	Vector3 p;
	Material* m;
public:
	Light() {}
	Light(Vector3 p, Material* m) {
		this->p = p;
		this->m = m;
	}

	Material* getMaterial() {
		return this->m;
	}

	~Light() {};

	/* data */
};

#endif