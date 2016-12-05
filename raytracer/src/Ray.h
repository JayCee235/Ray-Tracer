#ifndef __RAY
#define __RAY

#include "vector/GenVector.h"

class Ray {
private:
	Vector3 p, d;
public:
	Ray(Vector3 p, Vector3 d) {
		this->p = p;
		this->d = d;
	}

	Vector3 getP() {
		return this->p;
	}
	Vector3 getD() {
		return this->d;
	}
	Vector3 getDirection() {
		return this->d;
	}
};

#endif