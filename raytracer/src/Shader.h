#ifndef __SHADER
#define __SHADER

#include "RayTracer.h"

#define REFLECT_LIMIT 20

#define Kc 1
#define Kl 0.00000000001
#define Kq 0.00001

class Shader
{
private:
	Scene* scene;
	Camera* camera;
	Vector3 clearColor;
public:
	Shader(Scene* scene, Camera* camera, Vector3 clearColor) {
		this->scene = scene;
		this->camera = camera;
		this->clearColor = clearColor;
	}
	~Shader() {

	}

	Camera* getCamera() {
		return this->camera;
	}

	Camera* setCamera(Camera* cam) {
		delete(this->camera);
		this->camera = new Camera(cam);
	}

	Vector3 shadePoint(Ray* ray) {
		return castRay(ray, scene, camera, clearColor, 0, 0, 0, 1, 1);
	}

	Vector3 castRay(Ray* ray, Scene* scene, Camera* camera, Vector3 clearColor, int numReflected, int numTrans, float totalDis, float reflectRat, float transRat) {
	struct HitPoint hp;
	float t = scene->intersect(ray, &hp);
	Vector3 out = clearColor + Vector3(0, 0, 0);
	if(t > 0) {
		hp.normal = hp.p->getNormal(ray, &hp);
		hp.normal.normalize();

		Vector3 pointHit = ray->getPointAt(hp.t);

		Material* m = hp.p->getMaterial();

		Vector3 reflectColor = clearColor + Vector3(0, 0, 0);
		Vector3 transColor = clearColor + Vector3(0, 0, 0);

		float reflectRatio = 0;
		float transRatio = 0;

		if(m->reflect > 0 && numReflected < REFLECT_LIMIT && m->reflect * reflectRat > 0.01) {
			reflectRatio = m->reflect;
			Ray* newReflectRay = ray->reflect(ray->getPointAt(hp.t), hp.normal);
			float dis = 0;
			if(numReflected > 0) {
				dis = t;
			}
			reflectColor = castRay(newReflectRay, scene, camera, clearColor, numReflected+1, numTrans, totalDis+dis, reflectRat * reflectRatio, transRat);
			delete(newReflectRay);
		}

		//TODO: Add transparency.
		if(m->trans < 0.999 && (1-m->trans) * transRat > 0.01) {
			transRatio = 1 - m->trans;
			Ray* transRay = new Ray(ray->getPointAt(hp.t), ray->getD());
			float dis = 0;
			if(numTrans > 0) {
				dis = t;
			}
			transColor = castRay(transRay, scene, camera, clearColor, numReflected, numTrans++, totalDis+dis, reflectRat, transRat * transRatio);
			delete(transRay);
		}


		if(reflectRatio > 1) {
			reflectRatio = 1;
		}
		if(transRatio > 1) {
			transRatio = 1;
		}

		if(reflectRatio + transRatio > 1) {
			transRatio = 1 - reflectRatio;
		}

		float realRatio = 1 - transRatio - reflectRatio;

		std::vector<Light*>* lightsHit = new std::vector<Light*>();
		std::vector<Ray*>* raysHit = new std::vector<Ray*>();
		int numLightsHit;

		numLightsHit = scene->checkForLights(ray->getPointAt(hp.t), lightsHit, raysHit);
		Vector3 dif = Vector3(0, 0, 0);
		Vector3 spec = Vector3(0, 0, 0);
		if(numLightsHit > 0) {
			for(int i = 0; i < numLightsHit; i++) {
				Light* work = lightsHit[0][i];
				Ray* workRay = raysHit[0][i];
				Vector3 temp = work->getP() - workRay->getP();
				float dis = temp.length();
				if(numReflected > 0 || numTrans > 0) {
					dis += t + totalDis;
				}

				float dom = 1 / (Kc + Kl * dis + Kq * dis * dis);

				Vector3 rayDir = workRay->getD() * -1;

				Vector3 hpDir = Vector3(hp.normal);
				hpDir.normalize();

				float dott = rayDir.dot(hpDir);
				if(dott > 1) {
					dott = 1;
				}
				// dott = 0.9;
				if(dott > -0.001) {		
					if(dott < 0) {
						dott = 0;
					}
					// dott = 0.9;
					Vector3 lightDif = work->getMaterial()->diffuse;// *(work->count / 300);
					lightDif = lightDif * dott;
					lightDif = lightDif * dom;
					dif = dif + lightDif;

					Ray* reflection = workRay->reflect(Vector3(0, 0, 0), hp.normal);
					Vector3 reflectionDir = reflection->getD();

					Vector3 hpToCam = camera->getP() - ray->getPointAt(hp.t);
					hpToCam.normalize();

					// Vector3 H = workRay->getD() + hpToCam;
					// H.normalize();
					// float specMult = H.dot(ray->getD()*-1);


					float specMult = reflectionDir.dot(ray->getD()*-1);
					delete(reflection);

					if(specMult > 1) {
						specMult = 1;
					}

					if(specMult > 0) {
						// specMult *= dott;
						specMult = pow(specMult, 4*m->shiny);
						Vector3 lightSpec = work->getMaterial()->specular;// * (work->count / 300);
						lightSpec = lightSpec * specMult;
						lightSpec = lightSpec * dom;
						spec = spec + lightSpec;
					}
				}
					
			}					
		}
		for(int i = 0; i < raysHit->size(); i++) {
			delete(raysHit[0][i]);
		}
		delete(raysHit);
		delete(lightsHit);

		Vector3 ambC = scene->getAmbient();
		Vector3 c = m->ambient + Vector3(0, 0, 0);
		ambC = ambC * c;

		Vector3 difC = m->diffuse + Vector3(0, 0, 0);
		difC = difC * dif;

		Vector3 specC = m->specular + Vector3(0, 0, 0);
		specC = specC * spec;

		Vector3 ambientFinal = Vector3(ambC[0], ambC[1], ambC[2]);
		Vector3 diffuseFinal = Vector3(difC[0], difC[1], difC[2]);
		Vector3 specularFinal = Vector3(specC[0], specC[1], specC[2]);

		out = Vector3(0, 0, 0);
		out = out + ambientFinal;
		out = out + diffuseFinal;
		out = out + specularFinal;

		out = out * realRatio + reflectColor * reflectRatio + transColor * transRatio;

	}
	return out;
}
};

#endif