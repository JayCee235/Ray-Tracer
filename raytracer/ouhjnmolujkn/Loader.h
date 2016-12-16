#include "Loader.h"

#include <vector>
#include <map>

#include "objToGenVec.h"
#include "Primitive.h"
#include "Sphere.h"
#include "Triangle.h"
#include "camera.h"
#include "Light.h"
#include "AABB.h"
#include "BVHTree.h"

class Loader {
private:
	objLoader* data;
	std::vector<Material*>* matList; 
	int numMat;
	bool implicitTriangleCalculation;
public:
	Loader() {
		this->implicitTriangleCalculation = false;
	}
	Loader(bool implicitTriangleCalculation) {
		this->implicitTriangleCalculation = implicitTriangleCalculation;
	}

	void load(const char* filename) {
		printf("Loading loader... ");
		this->data = new objLoader();
		this->data->load(filename);
		printf("%d\n", data->faceCount);

		obj_material **ls = this->data->materialList;

		this->numMat = this->data->materialCount;

		printf("Found %d materials.\n", numMat);

		this->matList = new std::vector<Material*>();

		printf("Loading materials...\n");

		for(int i = 0; i < numMat; i++) {
			obj_material* work = ls[i];
			printf("Incoming material name: %s", work->name);
			Material* newMat = new Material(*work);
			matList->push_back(newMat);
			printf("Material added.\n");
			printf("Material info:\n");
			newMat->printInfo();
			printf("\n");
		}
		matList->push_back(new Material());
		this->numMat = this->numMat + 1;

		// printMaterialTest();
		
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

	BVHTree* getTree() {
		std::vector<Primitive*>* primList = this->getPrimitives();
		BVHTree* out = new BVHTree(primList[0][0]);

		printf("Constructing BVH tree...\n");
		for(int i = 1; i < primList->size(); i++) {
			out = out->insertPrimitive(primList[0][i]);
		}
		out->forceCheck();
		out->printTree();
		return out;
	}


	std::vector<Sphere*>* getSpheres() {
		printf("Getting spheres...\n");
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
			if(matIndex >= numMat || matIndex < 0) {
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
			if(matIndex >= this->numMat || matIndex < 0) {
				matIndex = this->numMat - 1;
			}


			Material* m = this->matList[0][matIndex];
			Light* newLight = new Light(p, m);
			printf("Found light with position (%.2f, %.2f, %.2f) and color (%.2f, %.2f, %.2f)\n",
				p[0], p[1], p[2],
				m->diffuse[0], m->diffuse[1], m->diffuse[2]);
			toReturn->push_back(newLight);
		}

		if(toReturn->size() == 0) {
			Light* l = new Light(Vector3(0, 100, 0), new Material());
			toReturn->push_back(l);
		}
		return toReturn;

	}

	std::vector<AABB*>* getAABBs() {
		std::vector<Primitive*>* prims = getPrimitives();
		std::vector<AABB*>* out = new std::vector<AABB*>();
		for(int i = 0; i < prims->size(); i++) {
			AABB* in = new AABB(prims[0][i]);
			out->push_back(in);
		}
		return out;
	}

	std::vector<Triangle*>* getTriangles() {
		printf("Getting triangles...\n");
		obj_face** faceList = data->faceList;
		int count = data->faceCount;

		obj_vector** vecLs = data->vertexList;
		obj_vector** normLs = data->normalList;

		bool implicitTriangleCalculation = this->implicitTriangleCalculation && 
			(normLs == NULL || data->normalCount <= 1);
		// implicitTriangleCalculation = false;

		std::vector<Triangle*>* toReturn = new std::vector<Triangle*>();


		std::vector<Ray*> vertices = std::vector<Ray*>();
		std::map<Ray*, std::vector<Triangle*> > vertexMap = std::map<Ray*, std::vector<Triangle*> >();
		if(implicitTriangleCalculation) {
			printf("Calculating triangle normals...\n");
			for(int i = 0; i < data->vertexCount; i++) {
				Ray* vert = new Ray();
				vert->setP(objToGenVec(vecLs[i]));
				vertices.push_back(vert);
				vertexMap[vert] = std::vector<Triangle*>();
			}
		}

		int i;
		for(i = 0; i < count; i++) {
			obj_face* faceToCheck = faceList[i];
			if(faceToCheck->vertex_count == 3) {
				int matIndex = faceToCheck->material_index;
				if(matIndex >= numMat || matIndex < 0) {
					matIndex = numMat - 1;
				}

				Material* m = this->matList[0][matIndex];

				// printf("Gathered material.\n");
				// m->printAbbrInfo();

				if(implicitTriangleCalculation) {
					Ray* a = vertices[faceToCheck->vertex_index[0]];
					Ray* b = vertices[faceToCheck->vertex_index[1]];
					Ray* c = vertices[faceToCheck->vertex_index[2]];

					Triangle* toAdd = new Triangle(a, b, c, m);
					vertexMap[a].push_back(toAdd);
					vertexMap[b].push_back(toAdd);
					vertexMap[c].push_back(toAdd);
					toReturn->push_back(toAdd);
				} else {
					Triangle* toAdd = new Triangle(faceToCheck, vecLs, normLs, m);
					toReturn->push_back(toAdd);
				}
			}
		}
		if(implicitTriangleCalculation){
			for(int i = 0; i < vertices.size(); i++) {
				Ray* work = vertices[i];
				std::vector<Triangle*> workList = vertexMap[work];
				Vector3 normal = Vector3(0, 1, 0);
				for(int j = 0; j < workList.size(); j++) {
					normal = normal + workList[j]->getTriangleNormal();
				}
				normal.normalize();
				work->setD(normal);
			}
			for(int i = 0; i < toReturn->size(); i++) {
				toReturn[0][i]->clonePoints();
			}
			for(int i = 0; i < vertices.size(); i++) {
				delete(vertices[i]);
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
		int out = data->lightPointCount;
		if(out == 0) {
			return 1;
		}
		return out;
	}


};