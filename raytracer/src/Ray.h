#ifndef __RAY
#define __RAY

#include "vector/GenVector.h"

class Ray {
private:
	Vector3 p, d;
public:
	Ray(Vector3 p, Vector3 d) {
		this->p = p + Vector3(0, 0, 0);
		this->d = d + Vector3(0, 0, 0);
		this->d.normalize();
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

	Ray* reflect(Vector3 point, Vector3 normal) {
		Vector3 old = this->d + Vector3(0, 0, 0);
		Vector3 correctedNormal = normal * normal.dot(old);
		Vector3 dif = old - correctedNormal;
		Vector3 toReturn = old - dif - dif;
		Ray *out = new Ray(point, toReturn);
		return out;
	}

	void printRay() {
		printf("Position: (%.2f, %.2f, %.2f).\nDirection: (%.2f, %.2f, %.2f).\n", 
			p[0], p[1], p[2],
			d[0], d[1], d[2]);
	}
};

#endif