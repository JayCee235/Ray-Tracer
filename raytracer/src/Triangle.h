#ifndef __TRIANGLE
#define __TRIANGLE

#include "vector/GenVector.h"
#include "Primitive.h"

class Triangle : public Primitive {
private:
	Vector3 a, b, c, normal;
	float d;
public:
	Triangle() {}
	Triangle(Vector3 a, Vector3 b, Vector3 c) {
		this-> a = Vector3(a);
		this->b = Vector3(b);
		this->c = Vector3(c);

		Vector3 n1 = b - a;
		Vector3 n2 = c - a;

		Vector normal = n1.cross(n2);

	}
	float intersect(Ray* r) {
		Vector3 ae = this->a - r->getP();
		float out = ae.dot(this->n)
		out = out / (r->getD().this->n);
		if(out < 0) {
			//Don't bother with intersection calculations, as the ray doesn't intersect above t=0
			return out;
		}
		Vector3 x = r.getPointAt(out);
		
	}
};

#endif