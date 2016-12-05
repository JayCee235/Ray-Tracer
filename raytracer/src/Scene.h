#ifndef __SCENE
#define __SCENE

#include "Primitive.h"
#include "Loader.h"
#include "Ray.h";

#ifndef __HITPOINT
#define __HITPOINT

struct HitPoint {
	float t;
	Primitive p;
	Ray r;
};

#endif

class Scene {
private:
	Loader l;
	Primitive* primList;
	Camera* camera;
	int primCount;
public:
	Scene(Loader l) {
		this->l = l;
		this->primList = l.getPrimitives();
		this->camera = l.getCamera();
		this->primCount = l.getPrimCount();
	}

	float intersect(Ray r, HitPoint* hp) {
		float out = -1;
		int i;
		for(i = 0; i < primCount; i++) {
			Primitive work = primList[i];
 			if(out < 0) {
 				out = work.intersect(r);
 				hp.t = out;
 				hp.p = work;
 			} else {
 				float temp = work.intersect(r);
 				if (temp >= 0 && temp < out) {
 					out = temp;
 				}
 			}
		}
		hp.r = r;
		return out;
	}

};

#endif