#ifndef __TRIANGLE
#define __TRIANGLE

#include "vector/GenVector.h"
#include "Primitive.h"
#include "HitPoint.h"
#include "Loader.h"

void printVector(Vector3 v) {
	printf("(%.2f, %.2f, %.2f)\n", v[0], v[1], v[2]);
}

class Triangle : public Primitive {
private:
	Vector3 a, b, c, normal;
	Vector3 normalA, normalB, normalC;
	Material* m;
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

		this->normalA = this->normal;
		this->normalB = this->normal;
		this->normalC = this->normal;

		this->m = new Material();
	}

	Triangle(Vector3 a, Vector3 b, Vector3 c, Material* m) {
		this->a = Vector3(a);
		this->b = Vector3(b);
		this->c = Vector3(c);

		Vector3 n1 = b - a;
		Vector3 n2 = c - a;

		this->normal = n1.cross(n2);
		this->normal.normalize();

		this->normalA = this->normal;
		this->normalB = this->normal;
		this->normalC = this->normal;

		this->m = m;
	}

	Triangle(obj_face* face, obj_vector** vertexList, obj_vector** normalList, Material* m) {
		this->a = objToGenVec(vertexList[face->vertex_index[0]]);
		this->b = objToGenVec(vertexList[face->vertex_index[1]]);
		this->c = objToGenVec(vertexList[face->vertex_index[2]]);

		Vector3 n1 = this->b - this->a;
		Vector3 n2 = this->c - this->a;

		this->normal = n1.cross(n2);
		this->normal.normalize();

		if(face->normal_index[0] >= 0) {
			this->normalA = objToGenVec(normalList[face->normal_index[0]]);
			this->normalB = objToGenVec(normalList[face->normal_index[1]]);
			this->normalC = objToGenVec(normalList[face->normal_index[2]]);
		} else {
			this->normalA = this->normal;
			this->normalB = this->normal;
			this->normalC = this->normal;
		}

		this->m = m;

	}

	float intersect(Ray* r) {
		Vector3 ae = this->a - r->getP();
		float out = ae.dot(this->normal);
		if(out > -0.0001 && out < 0.0001) {
			//The point is very close to the plane, so to avoid floating point problems, just return -1.
			return -1;
		}
		out = out / (r->getD().dot(this->normal));
		if(out < 0) {
			//Don't bother with intersection calculations, as the ray doesn't intersect above t=0
			return -1;
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

		if(baxa.dot(this->normal) > -0.000001 && cbxb.dot(this->normal) > -0.000001 && acxc.dot(this->normal) > -0.000001) {
			return out;
		} else {
			return -1;
		}
	}

	Vector3 getNormal(Ray* r, HitPoint* hp) {
		Vector3 pointToCheck = r->getPointAt(hp->t);
		// printf("Point to Check: ");
		// printVector(pointToCheck);

		Vector3 ba = this->b - this->a;
		Vector3 ca = this->c - this->a;

		Vector3 baryPoint = pointToCheck - this->a;

		// printf("BA: ");
		// printVector(ba);
		// printf("CA: ");
		// printVector(ca);
		// printf("Point: ");
		// printVector(baryPoint);

		float dom = 1 / ((ca.dot(ca) * ba.dot(ba) - ca.dot(ba)*ca.dot(ba)));

		float baryC = (ca.dot(baryPoint) * ba.dot(ba) - ca.dot(ba)*ba.dot(baryPoint)) * dom;
		float baryB = (ba.dot(baryPoint) * ca.dot(ca) - ca.dot(ba)*ca.dot(baryPoint)) * dom;
		if(baryB < 0) {
			baryB = 0;
		}
		if(baryB > 1) {
			baryB = 1;
		}
		if(baryC < 0) {
			baryC = 0;
		}
		if(baryC > 1) {
			baryC = 1;
		}

		float baryA = 1 - baryB - baryC;

		if(baryA < 0) {
			baryA = 0;
		}
		if(baryA > 1) {
			baryA = 1;
		}

		// printf("Bary: %.2f, %.2f, %.2f.\n", baryA, baryB, baryC);

		Vector3 out = (normalA * baryA) + (normalB * baryB) + (normalC * baryC);

		return out;
	}

	Material* getMaterial() {
		return this->m;
	}
};

#endif