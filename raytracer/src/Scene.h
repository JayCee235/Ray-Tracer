#ifndef __SCENE
#define __SCENE

#include "Primitive.h"
#include "Sphere.h"
#include "Loader.h"
#include "Ray.h"
#include <vector>
#include "HitPoint.h"
#include "AABB.h"
#include "BVHTree.h"

class Scene {
private:
	Loader* loader;
	std::vector<Primitive*>* primList;
	std::vector<Light*>* lightList;
	Camera* camera;
	int primCount;
	int lightCount;
	float lightScale;
	BVHTree* tree;
	Vector3 ambient;
public:
	Scene() {}

	void quietLoad(Loader* loader) {
		this->loader = loader;
		// this->primList = this->loader->getPrimitives();
		this->primList = new std::vector<Primitive*>();
		this->lightList = this->loader->getLights();
		this->camera = this->loader->getCamera();
		this->primCount = this->loader->getPrimCount();
		this->lightCount = this->loader->getLightCount();
		this->tree = this->loader->getTree();
		this->ambient = Vector3(0, 0, 0);
		for(int i = 0; i < lightList->size(); i++) {
			this->ambient = this->ambient + lightList[0][i]->getMaterial()->ambient;
		}
		this->primList->push_back(this->tree);
	}

	void load(Loader* loader) {
		printf("Making Loader\n");
		this->loader = loader;
		printf("Loading prims\n");
		// this->primList = this->loader->getPrimitives();
		this->primList = new std::vector<Primitive*>();
		printf("Loading Lights...\n");
		this->lightList = this->loader->getLights();
		printf("loading camera.\n");
		this->camera = this->loader->getCamera();
		printf("loading counts.\n");
		this->primCount = this->loader->getPrimCount();
		this->lightCount = this->loader->getLightCount();
		printf("Found %d primitives and %d lights.\n", primCount, lightCount);

		this->tree = this->loader->getTree();
		this->ambient = Vector3(0, 0, 0);
		for(int i = 0; i < lightList->size(); i++) {
			this->ambient = this->ambient + lightList[0][i]->getMaterial()->ambient;
		}
		this->primList->push_back(this->tree);
	}

	Vector3 getAmbient() {
		return this->ambient;
	}

	void printTree() {
		tree->printTree();
	}

	float intersect(Ray* r, HitPoint* hp) {
		// printf("I am beginning an intersection.\n");
		float out = this->tree->intersect(r);
		if(out > -0.00001) {
			if(out < 0.00001) out = 0.00001;
			hp->t = out;
			hp->p = tree->getPrimitive();
			hp->normal = hp->p->getNormal(r, hp);
			// printf("Hit! %.2f\n", out);
			return out;
		} else {
			return -1;
		}
	}

	float oldIntersect(Ray* r, HitPoint* hp) {
		float out = -1;
		int i;
		std::vector<Primitive*> ls = *this->primList;
		for(i = 0; i < primCount; i++) {
			Primitive* work = ls[i];
 			if(out < 0) {
 				float temp = work->intersect(r);
 				if(temp > 0.001) {
 					out = temp;
 					hp->t = out;
 					hp->p = work;
 					hp->normal = hp->p->getNormal(r, hp);
 				} else {
 					out = -1;
 				}		
 			} else {
 				float temp = work->intersect(r);
 				if (temp >= 0.001 && temp < out) {
 					out = temp;
 					hp->t = out;
 					hp->p = work;
 					hp->normal = hp->p->getNormal(r, hp);
 				}
 			}
		}
		return out;
	}

	int getNumLights() {
		return this->lightCount;
	}

	int checkForLights(Vector3 pos, std::vector<Light*>* lBuffer, std::vector<Ray*>* rBuffer) {
		int count = 0;
		for(int i = 0; i < this->lightCount; i++) {
			struct HitPoint temp;
			// printf("Checking light %d\n", i);
			Light* work = lightList[0][i];
			Vector3 pp = work->getP();
			Vector3 dif = work->getP() - pos;
			Ray* posToLight = new Ray(pos, dif);
			float len = dif.length();
			// printf("Pos:\n");
			// printf("Position: (%.f, %.2f,%.2f)\n", 
			// pos[0], pos[1], pos[2]);
			// printf("Light:\n");
			// work->printLight();
			// printf("posToLight:\n");
			// posToLight->printRay();
			// printf("Getting ray trace...\n");
			float returnValue = intersect(posToLight, &temp);
			// while(returnValue > 0 && temp.p != NULL && temp.p->getMaterial() != NULL && temp.p->getMaterial()->trans < 1) {
			// 	posToLight->setP(posToLight->getPointAt(temp.t));
			// 	returnValue = intersect(posToLight, &temp);
			// }
			if(returnValue > len - 0.001 || returnValue < 0.01) {
				// printf("Hit light!\n");
				lBuffer->push_back(work);
				// printf("Added to light Buffer.\n");
				posToLight->setD(posToLight->getD()*-1);
				rBuffer->push_back(posToLight);
				// printf("Added to ray Buffer.\n");
				count++;
			}
			// printf("Returned %.2f\n", returnValue);

			
		}
		return count;
	}

};

#endif