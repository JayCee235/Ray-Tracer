#ifndef __LOADER
#define __LOADER

#ifndef __OBJTOVEC
#define __OBJTOVEC
Vector3 objToGenVec(obj_vector const * objVec)
{
	Vector3 toReturn;
	toReturn[0] = objVec->e[0];
	toReturn[1] = objVec->e[1];
	toReturn[2] = objVec->e[2];
	return toReturn;
}
#endif

#include "Primitive.h"
#include "Sphere.h"
#include "Triangle.h"
#include "camera.h"
#include "Light.h"
#include <vector>

class Loader {
private:
	objLoader* data;
	std::vector<Material*>* matList; 
	int numMat;
public:
	Loader() {}

	void load(const char* filename) {
		printf("Loading loader... ");
		this->data = new objLoader();
		this->data->load(filename);
		printf("%d\n", data->faceCount);

		obj_material **ls = this->data->materialList;

		numMat = this->data->materialCount;

		printf("Found %d materials.\n", numMat);

		this->matList = new std::vector<Material*>();

		printf("Loading materials...\n");

		for(int i = 0; i < numMat; i++) {
			obj_material* work = ls[i];
			printf("Incoming material name: %s\n", work->name);
			Material* newMat = new Material(*work);
			matList->push_back(newMat);
			printf("Material added.\n");
			printf("Material info:\n");
			newMat->printInfo();
		}

		printMaterialTest();
		
	}

	void printMaterialTest() {
		printf("Testing Materials loaded...\n");
		for(int i = 0; i < this->data->materialCount; i++) {
			matList[0][i]->printInfo();
		}
	}

	objLoader* getObjData() {
		return data;
	}
	obj_vector** getObjVertexList() {
		return data->vertexList;
	}
	obj_sphere** getObjSphereList() {
		return data->sphereList;
	}
	obj_camera* getObjCamera() {
		return data->camera;
	}

	std::vector<Material*>* getMatList() {
		return this->matList;
	}


	std::vector<Sphere*>* getSpheres() {
		obj_sphere** sphereList = data->sphereList;
		int count = data->sphereCount;

		obj_vector** vecLs = data->vertexList;
		obj_vector** normLs = data->normalList;

		std::vector<Sphere*>* toReturn = new std::vector<Sphere*>();

		// printf("Prepared to run.\n");

		int i;
		for(i = 0; i < count; i++) {
			obj_sphere* sphereToCalc = sphereList[i];
			// printf("Sphere center: %d\n", sphereToCalc->pos_index);
			Vector3 center = objToGenVec(vecLs[sphereToCalc->pos_index]);
			// printf("Sphere center calculated.\n");
			int upNormIndex = sphereToCalc->up_normal_index;
			// printf("Normal index gathered.\n");
			obj_vector* upNorm = normLs[upNormIndex];
			// printf("Normal vector gathered. Result: (%f, %f, %f)\n", upNorm->e[0], upNorm->e[1], upNorm->e[2]);
			Vector3 norm = objToGenVec(upNorm);
			// printf("Sphere normal calculated.\n");
			float radius = norm.length();

			int matIndex = sphereToCalc->material_index;
			if(matIndex >= numMat) {
				matIndex = numMat - 1;
			}

			Material* m = this->matList[0][matIndex];

			Sphere* toAdd = new Sphere(center, radius, m);

			// printf("Ready to push\n");
			toReturn->push_back(toAdd);
		}
		// printf("Ready to return.\n");
		return toReturn;
	}

	std::vector<Light*>* getLights() {
		obj_light_point **lightList = data->lightPointList;
		int count = data->lightPointCount;

		obj_vector** vecLs = data->vertexList;

		std::vector<Light*>* toReturn = new std::vector<Light*>();

		for(int i = 0; i < count; i++) {
			printf("Getting light...\n");
			obj_light_point* objLight = lightList[i];
			Vector3 p = objToGenVec(vecLs[objLight->pos_index]);
			int matIndex = objLight->material_index;
			Material* m = this->matList[0][matIndex];
			Light* newLight = new Light(p, m);
			printf("Found light with position (%.2f, %.2f, %.2f) and color (%.2f, %.2f, %.2f)\n",
				p[0], p[1], p[2],
				m->diffuse[0], m->diffuse[1], m->diffuse[2]);
			toReturn->push_back(newLight);
		}
		return toReturn;

	}

	std::vector<Triangle*>* getTriangles() {
		obj_face** faceList = data->faceList;
		int count = data->faceCount;

		obj_vector** vecLs = data->vertexList;
		obj_vector** normLs = data->normalList;

		std::vector<Triangle*>* toReturn = new std::vector<Triangle*>();

		int i;
		for(i = 0; i < count; i++) {
			obj_face* faceToCheck = faceList[i];
			if(faceToCheck->vertex_count == 3) {
				int ai = faceToCheck->vertex_index[0];
				int bi = faceToCheck->vertex_index[1];
				int ci = faceToCheck->vertex_index[2];

				Vector3 aVec = objToGenVec(vecLs[ai]);
				Vector3 bVec = objToGenVec(vecLs[bi]);
				Vector3 cVec = objToGenVec(vecLs[ci]);

				// printf("Getting material for triangle...\n");
				// printf("Triangle uses index %d.\n", faceToCheck->material_index);

				int matIndex = faceToCheck->material_index;
				if(matIndex >= numMat) {
					matIndex = numMat - 1;
				}

				Material* m = this->matList[0][matIndex];

				// printf("Gathered material.\n");
				// m->printAbbrInfo();

				Triangle* toAdd = new Triangle(aVec, bVec, cVec, m);

				toReturn->push_back(toAdd);
			}
		}

		return toReturn;
	}

	Camera* getCamera() {
		obj_camera* cam = data->camera;
		obj_vector** vecLs = data->vertexList;
		obj_vector** normLs = data->normalList;

		Camera* camera = new Camera(cam, vecLs, normLs);

		return camera;
	}

	std::vector<Primitive*>* getPrimitives() {
		std::vector<Primitive*>* toReturn = new std::vector<Primitive*>();
		printf("Starting getPrimitives()...\n");

		std::vector<Sphere*>* spheres = getSpheres();
		std::vector<Triangle*>* triangles = getTriangles();


		for(int i = 0; i < spheres->size(); i++) {
			toReturn->push_back((Primitive*) (spheres[0][i]));
		}

		for(int i = 0; i < triangles->size(); i++) {
			toReturn->push_back((Primitive*) (triangles[0][i]));
		}

		return toReturn;
	}

	int getTriangleCount() {
		obj_face** ls = data->faceList;
		int out = 0;
		for(int i = 0; i < data->faceCount; i++) {
			if(ls[i]->vertex_count == 3) {
				out++;
			}
		}
		return out;
	}

	int getPrimCount() {
		return data->sphereCount + getTriangleCount();
	}

	int getLightCount() {
		return data->lightPointCount;
	}


};

#endif