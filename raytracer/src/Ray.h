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
	Vector3 getPointAt(float t) {
		Vector3 dis = this->d * t;
		return dis + this->p;
	}

	Ray* reflect(Vector3 normal) {
		Vector3 old = this->d + Vector3(0, 0, 0);
		Vector3 dif = old - normal;
		Vector3 toReturn = normal - dif;
		Ray *out = new Ray(this->p, toReturn);
		return out;
	}
};

#endif