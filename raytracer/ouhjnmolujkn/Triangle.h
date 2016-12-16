#include "Triangle.h"

#include "vector/GenVector.h"
#include "Ray.h"
#include "Primitive.h"
#include "HitPoint.h"
#include "Loader.h"

void printVector(Vector3 v) {
	printf("(%.2f, %.2f, %.2f)\n", v[0], v[1], v[2]);
}

class Triangle : public Primitive {
private:
	Ray* a;
	Ray* b;
	Ray* c;
	Vector3 normal;
	bool safeToDelete;
	// Vector3 a, b, c, normal;
	// Vector3 normalA, normalB, normalC;
	Material* m;
public:
	Triangle() {}
	Triangle(Ray* a, Ray* b, Ray* c, Material* m) {
		this->a = a;
		this->b = b;
		this->c = c;

		Vector3 n1 = b->getP() - a->getP();
		Vector3 n2 = c->getP() - a->getP();

		this->normal = n1.cross(n2);
		this->normal.normalize();

		safeToDelete = false;

		this->m = m;
	}
	Triangle(Vector3 a, Vector3 b, Vector3 c) {
		this->a = new Ray();
		this->b = new Ray();
		this->c = new Ray();

		this->a->setP(Vector3(a));
		this->b->setP(Vector3(b));
		this->c->setP(Vector3(c));

		Vector3 n1 = b - a;
		Vector3 n2 = c - a;

		this->normal = n1.cross(n2);
		this->normal.normalize();

		this->a->setD(this->normal);
		this->b->setD(this->normal);
		this->c->setD(this->normal);

		this->m = new Material();

		safeToDelete = true;
	}

	Triangle(Vector3 a, Vector3 b, Vector3 c, Material* m) {
		this->a = new Ray();
		this->b = new Ray();
		this->c = new Ray();

		this->a->setP(Vector3(a));
		this->b->setP(Vector3(b));
		this->c->setP(Vector3(c));

		Vector3 n1 = b - a;
		Vector3 n2 = c - a;

		this->normal = n1.cross(n2);
		this->normal.normalize();

		this->a->setD(this->normal);
		this->b->setD(this->normal);
		this->c->setD(this->normal);

		this->m = m;

		safeToDelete = true;
	}

	Triangle(obj_face* face, obj_vector** vertexList, obj_vector** normalList, Material* m) {
		this->a = new Ray();
		this->b = new Ray();
		this->c = new Ray();

		this->a->setP(objToGenVec(vertexList[face->vertex_index[0]]));
		this->b->setP(objToGenVec(vertexList[face->vertex_index[1]]));
		this->c->setP(objToGenVec(vertexList[face->vertex_index[2]]));

		Vector3 n1 = this->b->getP() - this->a->getP();
		Vector3 n2 = this->c->getP() - this->a->getP();

		this->normal = n1.cross(n2);
		this->normal.normalize();

		if(face->normal_index[0] >= 0) {
			this->a->setD(objToGenVec(normalList[face->normal_index[0]]));
			this->b->setD(objToGenVec(normalList[face->normal_index[1]]));
			this->c->setD(objToGenVec(normalList[face->normal_index[2]]));
		} else {
			this->a->setD(this->normal);
			this->b->setD(this->normal);
			this->c->setD(this->normal);
		}

		this->m = m;

		safeToDelete = true;
	}

	~Triangle() {
		if(this->safeToDelete) {
			delete(a);
			delete(b);
			delete(c);
		}
	}

	void clonePoints() {
		this->a = new Ray(a);
		this->b = new Ray(b);
		this->c = new Ray(c);
		safeToDelete = true;
	}

	Vector3 getTriangleNormal() {
		return Vector3(this->normal);
	}

	float intersect(Ray* r) {
		// printf("Triangle intersect.\n");
		Vector3 ae = this->a->getP() - r->getP();
		float out = ae.dot(this->normal);
		// printf("Access 1 done.\n");
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
		Vector3 ba = this->b->getP() - this->a->getP();
		Vector3 cb = this->c->getP() - this->b->getP();
		Vector3 ac = this->a->getP() - this->c->getP();
		Vector3 xa = x - this->a->getP();
		Vector3 xb = x - this->b->getP();
		Vector3 xc = x - this->c->getP();

		Vector3 baxa = ba.cross(xa);
		Vector3 cbxb = cb.cross(xb);
		Vector3 acxc = ac.cross(xc);

		// printf("Access 2 done.\n");
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

		Vector3 ba = this->b->getP() - this->a->getP();
		Vector3 ca = this->c->getP() - this->a->getP();

		Vector3 baryPoint = pointToCheck - this->a->getP();

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

		Vector3 out = (this->a->getD() * baryA) + (this->b->getD() * baryB) + (this->c->getD() * baryC);

		out.normalize();

		return out;
	}

	Material* getMaterial() {
		return this->m;
	}

	Vector3 getMinimumPoint() {
		Vector3 min = a->getP() + Vector3(0, 0, 0);
		for(int i = 0 ; i < 3; i++) {
			if(b->getP()[i] < min[i]) min[i] = b->getP()[i];
			if(c->getP()[i] < min[i]) min[i] = c->getP()[i];
		}
		return min;
	}

	Vector3 getMaximumPoint() {
		Vector3 max = a->getP() + Vector3(0, 0, 0);
		for(int i = 0 ; i < 3; i++) {
			if(b->getP()[i] > max[i]) max[i] = b->getP()[i];
			if(c->getP()[i] > max[i]) max[i] = c->getP()[i];
		}
		return max;
	}
};