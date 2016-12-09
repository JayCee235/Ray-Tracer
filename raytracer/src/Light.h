#ifndef __LIGHTSOURCE
#define __LIGHTSOURCE

#include "vector/GenVector.h"
#include "Material.h"

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

	Vector3 getP() {
		return this->p;
	}

	~Light() {};

	void printLight() {
		printf("Position: (%.2f, %.2f, %.2f).\n", 
			p[0], p[1], p[2]);
	}
};

#endif