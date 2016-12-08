#ifndef __SCENE
#define __SCENE

#include "Primitive.h"
#include "Sphere.h"
#include "Loader.h"
#include "Ray.h"
#include <vector>
#include "HitPoint.h"

class Scene {
private:
	Loader* loader;
	std::vector<Primitive*>* primList;
	std::vector<Light*>* lightList;
	Camera* camera;
	int primCount;
	int lightCount;
public:
	Scene() {}

	void load(Loader* loader) {
		printf("Making Loader\n");
		this->loader = loader;
		printf("Loading prims\n");
		this->primList = this->loader->getPrimitives();
		printf("Loading Lights,,,\n");
		this->lightList = this->loader->getLights();
		printf("loading camera.\n");
		this->camera = this->loader->getCamera();
		printf("loading counts.\n");
		this->primCount = this->loader->getPrimCount();
		this->lightCount = this->loader->getLightCount();

	}

	float intersect(Ray* r, HitPoint* hp) {
		float out = -1;
		int i;
		std::vector<Primitive*> ls = *this->primList;
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

	int checkForLights(Vector3 pos, std::vector<Light*>* lBuffer, std::vector<Ray*>* rBuffer) {
		std::vector<Light*>* out = new std::vector<Light*>();
		std::vector<Ray*>* rOut = new std::vector<Ray*>();
		struct HitPoint temp;
		int count = 0;
		for(int i = 0; i < this->lightCount; i++) {
			Light* work = lightList[0][i];
			Vector3 dif = work->getP() - pos;
			Ray* posToLight = new Ray(pos, dif);
			float len = dif.length();

			float return = intersect(posToLight, temp);

			if(return >= len) {
				out->push_back(work);
				rOut->push_back(posToLight);
				count++;
			}
		}
		*lBuffer = *out;
		*rBuffer = *rOut;
		return count;
	}

};

#endif