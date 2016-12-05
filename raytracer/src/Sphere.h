#ifndef __SPHERE
#define __SPHERE

#include "vector/GenVector.h"
#include "Primitive.h"

class Sphere : public Primitive {
private:
	Vector3 center;
	float radius;
public:
	Sphere(Vector3 center, float radius) {
		this->center = center;
		this->radius = radius;
	}
	float intersect(Ray r) {
		Vector3 ec = (r.getP() - this->center);

		float a = r.getD().dot(r.getD());
		float b = (2.0f*r.getD()).dot(ec);
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
};

#endif