#include "Ray.h"

class Ray {
private:
	Vector3 p, d;
	Vector3 invD;
public:
	Ray(Vector3 p, Vector3 d) {
		this->p = p + Vector3(0, 0, 0);
		this->d = d + Vector3(0, 0, 0);
		this->d.normalize();

		this->invD = Vector3(1, 1, 1)/this->d;
	}

	Ray() {
		this->p = Vector3(0, 0, 0);
		this->d = Vector3(0, 1, 0);
		this->invD = Vector3(1, 1, 1)/this->d;
	}

	Ray(Ray* copy) {
		this->p = copy->p + Vector3(0, 0, 0);
		this->d = copy->d + Vector3(0, 0, 0);
		this->invD = copy->invD + Vector3(0, 0, 0);
	}

	void setP(Vector3 p) {
		this->p = p + Vector3(0, 0, 0);
	}

	void setD(Vector3 d) {
		this->d = d + Vector3(0, 0, 0);
		this->d.normalize();

		this->invD = Vector3(1, 1, 1)/this->d;
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

	Vector3 getInverseDirection() {
		return this->invD + Vector3(0, 0, 0);
	}
};