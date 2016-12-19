#ifndef __LIGHTSOURCE
#define __LIGHTSOURCE

#include <stdlib.h>
#include "vector/GenVector.h"
#include "Material.h"

class Light
{
private:
	Vector3 p;
	Material* m;
	float radius;
public:
	Light() {}
	Light(Vector3 p, Material* m) {
		this->p = p;
		this->m = m;
		this->radius = 10;
	}

	Light(Vector3 p, float r, Material* m) {
		this->p = p;
		this->m = m;
		this->radius = r;
	}

	Material* getMaterial() {
		return this->m;
	}

	Vector3 getP() {
		return this->p;
	}

	Vector3 getRandomP() {
		// return getP();
		int x = -100 + rand() % 200;
		int y = -100 + rand() % 200;
		int z = -100 + rand() % 200;
		Vector3 n = Vector3(x, y, z);
		n.normalize();
		n *= radius * (rand() % 100) * 0.01;
		return this->p + n;
	}

	~Light() {};

	void printLight() {
		printf("Position: (%.2f, %.2f, %.2f).\n", 
			p[0], p[1], p[2]);
	}
	int count;
};

#endif