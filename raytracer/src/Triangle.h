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

		this->normal = n1.cross(n2);
		this->normal.normalize();

	}
	float intersect(Ray* r) {
		Vector3 ae = this->a - r->getP();
		float out = ae.dot(this->normal);
		out = out / (r->getD().dot(this->normal));
		if(out < 0) {
			//Don't bother with intersection calculations, as the ray doesn't intersect above t=0
			return out;
		}
		Vector3 x = r->getPointAt(out);
		Vector3 ba = this->b - this->a;
		Vector3 cb = this->c - this->b;
		Vector3 ac = this->a - this->c;
		Vector3 xa = x - this->a;
		Vector3 xb = x - this->b;
		Vector3 xc = x - this->c;

		Vector3 baxa = ba.cross(xa);
		Vector3 cbxb = cb.cross(xb);
		Vector3 acxc = ac.cross(xc);

		if(baxa.dot(this->normal) > 0 && cbxb.dot(this->normal) > 0 && acxc.dot(this->normal) > 0) {
			return out;
		} else {
			return -1;
		}
	}
};

#endif