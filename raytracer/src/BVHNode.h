#ifndef __BVHNODE
#define __BVHNODE

#include "vector/GenVector.h"
#include "Primitive.h"
#include "HitPoint.h"
#include "Loader.h"
#include "AABB.h"

#include "Triangle.h"
#include "Sphere.h"

class BVHNode : public Primitive
{
private:
	Primitive* p;
	AABB* box;
public:
	BVHNode() {}
	~BVHNode() {}

	BVHNode(BVHNode* copy) {
		this->p = copy->p;
		this->box = new AABB(copy->box);
	}

	BVHNode(Primitive* p) {
		this->p = p;
		this->box = new AABB(p);
	}

	BVHNode(Vector3 min, Vector3 max) {
		this->box = new AABB(min, max);
		this->p = NULL;
	}

	Primitive* getPrimitive() {
		return this->p;
	}

	void setPrimitive(Primitive* p) {
		this->p = p;
	}

	AABB* getBox() {
		return this->box;
	}

	void setBox(AABB* b) {
		this->box = b;
	}

	void resizeToFit(AABB* a) {
		this->box->resizeToFit(a);
	}

	float intersect(Ray* r) {
		if(this->p == NULL) return -1;
		float check = box->intersect(r);
		if(check < 0) {
			return -1;
		}
		check = p->intersect(r);
		if(check < 0) {
			return -1;
		}
		// printf("I returned %.2f\n", check);
		return check;
	}
	Vector3 getNormal(Ray* r, HitPoint* hp) {
		if(this->p == NULL) return Vector3(0, 1, 0);
		return p->getNormal(r, hp);
	}

	Material* getMaterial() {
		if(this->p == NULL) return new Material();
		return p->getMaterial();
	}

	Vector3 getMinimumPoint() {
		return box->getMinimumPoint();
	}

	Vector3 getMaximumPoint() {
		return box->getMinimumPoint();
	}
};

#endif