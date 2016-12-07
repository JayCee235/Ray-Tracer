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
#include <vector>

class Loader {
private:
	objLoader* data;
public:
	Loader() {}

	void load(const char* filename) {
		printf("Loading loader... ");
		this->data = new objLoader();
		this->data->load(filename);
		printf("%d\n", data->sphereCount);
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

			Sphere* toAdd = new Sphere(center, radius);

			// printf("Ready to push\n");
			toReturn->push_back(toAdd);
		}
		// printf("Ready to return.\n");
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

				Triangle* toAdd = new Triangle(aVec, bVec, cVec);

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


};

#endif