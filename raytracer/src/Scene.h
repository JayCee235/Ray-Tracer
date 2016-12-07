#ifndef __SCENE
#define __SCENE

#include "Primitive.h"
#include "Sphere.h"
#include "Loader.h"
#include "Ray.h"
#include <vector>

#ifndef __HITPOINT
#define __HITPOINT

struct HitPoint {
	float t;
	Primitive* p;
	Vector3* normal;
};

void printHitPoint(struct HitPoint* hp) {
	printf("t value: %f.\n", hp->t);
}

#endif

class Scene {
private:
	Loader* loader;
	std::vector<Sphere*>* primList;
	Camera* camera;
	int primCount;
public:
	Scene() {}

	void load(Loader* loader) {
		printf("Making Loader\n");
		this->loader = loader;
		printf("Loading prims\n");
		this->primList = this->loader->getSpheres();
		printf("loading camera.\n");
		this->camera = this->loader->getCamera();
		printf("loading count.\n");
		this->primCount = this->loader->getPrimCount();
	}

	float intersect(Ray* r, HitPoint* hp) {
		float out = -1;
		int i;
		std::vector<Sphere*> ls = *this->primList;
		for(i = 0; i < primCount; i++) {
			Primitive* work = ls[i];
 			if(out < 0) {
 				out = work->intersect(r);
 				hp->t = out;
 				hp->p = work;
 			} else {
 				float temp = work->intersect(r);
 				if (temp >= 0 && temp < out) {
 					out = temp;
 				}
 			}
		}
		return out;
	}

};

#endif