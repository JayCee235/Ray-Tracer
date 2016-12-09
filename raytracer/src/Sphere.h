#ifndef __SPHERE
#define __SPHERE

#include "vector/GenVector.h"
#include "Primitive.h"

class Sphere : public Primitive {
private:
	Vector3 center;
	float radius;
	Material* m;
public:
	Sphere() {
		radius = 0;
		center = Vector3(0, 0, 0);
		this-> m = new Material();
	}
	Sphere(Vector3 center, float radius) {
		this->center = center;
		this->radius = radius;
	}
	Sphere(Vector3 center, float radius, Material* m) {
		this->center = center;
		this->radius = radius;
		this->m = m;
	}


	float intersect(Ray* r) {
		Vector3 ec = (r->getP() - this->center);
		// ec.normalize();

		float a = r->getD().dot(r->getD());
		float b = (r->getD()*2.0f).dot(ec);
		float c = ec.dot(ec) - this->radius * this->radius;

		struct quadAnswer ans;
		ans = calculateQuadratic(a, b, c);
		if(ans.numAnswers == 2) {
			if(ans.ans1 < 0) {
				return ans.ans2;
			}
			return ans.ans1;
		} else if (ans.numAnswers == 1) {
			return ans.ans1;
		} else {
			return -1;
		}
	}

	Vector3 getNormal(Ray* r, HitPoint* hp) {
		Vector3 x = r->getPointAt(hp->t);
		Vector3 toReturn = x - this->center;
		toReturn.normalize();
		return toReturn;
	}

	Material* getMaterial() {
		return this->m;
	}

};

#endif