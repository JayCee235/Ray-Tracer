#ifndef __AABB
#define __AABB

#include "vector/GenVector.h"
#include "Primitive.h"
#include "HitPoint.h"
#include "Loader.h"

class AABB : public Primitive
{
private:
	Vector3 min, max;
public:
	AABB(Primitive* p) {
		this->min = p->getMinimumPoint();
		this->max = p->getMaximumPoint();
	}
	AABB(Vector3 min, Vector3 max) {
		this->min = min;
		this->max = max;
	}

	AABB(AABB* copy) {
		this->min  = copy->min + Vector3(0, 0, 0);
		this->max = copy->max + Vector3(0, 0, 0);
	}
	~AABB() {}

	void resizeToFit(AABB* a) {
		Vector3 newMin;
		Vector3 newMax;
		for(int i = 0; i < 3; i++) {
			if(this->min[i] < a->min[i]) {
				newMin[i] = this->min[i];
			} else {
				newMin[i] = a->min[i];
			}

			if(this->max[i] > a->max[i]) {
				newMax[i] = this->max[i];
			} else {
				newMax[i] = a->max[i];
			}
		}
		this->min = newMin;
		this->max = newMax;
	}

	float getCurrentVolume() {
		Vector3 lwh = max - min;
		float out = lwh[0] * lwh[1] * lwh[2];
		return out;
	}

	float getNewVolume(AABB* a) {
		AABB* toCheck = new AABB(this);
		toCheck->resizeToFit(a);
		return toCheck->getCurrentVolume();
	}

	float intersect(Ray* r) {
		Vector3 t1 = (min - r->getP()) * r->getInverseDirection();
		Vector3 t2 = (max - r->getP()) * r->getInverseDirection();

		Vector3 tMin;
		Vector3 tMax;

		for(int i = 0; i < 3; i++) {
			if(t1[i] < t2[i]) {
				tMin[i] = t1[i];
				tMax[i] = t2[i];
			} else {
				tMin[i] = t2[i];
				tMax[i] = t1[i];
			}
		}

		float lastMin = tMin[0];
		float firstMax = tMax[0];

		for(int i = 1; i < 3; i++) {
			if(tMin[i] > lastMin) {
				lastMin = tMin[i];
			}
			if(tMax[i] < firstMax) {
				firstMax = tMax[i];
			}
		}

		if(lastMin > firstMax || firstMax < -0.00001) {
			return -1;
		}
		if(lastMin > 0) {
			return lastMin;
		}
		return firstMax;
	}
	Vector3 getNormal(Ray* r, HitPoint* hp) {
		Vector3 work = r->getPointAt(hp->t);
		Vector3 out = Vector3(0, 0, 0);
		Vector3 w1 = work - min;
		Vector3 w2 = work - max;
		for(int i = 0; i < 3; i++) {
			if(w1[i] < 0.001 && w1[i] > -0.001) {
				out[i] = -1;
			}
			if(w2[i] < 0.001 && w2[i] > -0.001) {
				out[i] = 1;
			}
		}
		if(out == Vector3(0, 0, 0)) out = Vector3(0, 1, 0);
		return out;
	}

	Material* getMaterial() {
		return new Material();
	}

	Vector3 getMinimumPoint() {
		return min + Vector3(0, 0, 0);
	}

	Vector3 getMaximumPoint() {
		return max + Vector3(0, 0, 0);
	}
};

#endif