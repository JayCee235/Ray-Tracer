#ifndef __LOADER
#define __LOADER

#ifndef __OBJTOVEC
#define __OBJTOVEC
Vector3 objToGenVec(obj_vector const * objVec)
{
	Vector3 v;
	v[0] = objVec->e[0];
	v[1] = objVec->e[1];
	v[2] = objVec->e[2];
	return v;
}
#endif

#include "Primitive.h";
#include "Sphere.h";
#include "camera.h"

class Loader {
private:
	objLoader data;
public:
	Loader(const char* filename) {
		data = objLoader();
		data.load();
	}
	objLoader getObjData() {
		return data;
	}
	obj_vector** getObjVertexList() {
		return data.vertexList;
	}
	obj_sphere** getObjSphereList() {
		return data.sphereList;
	}
	obj_camera* getObjCamera() {
		return data.camera;
	}

	Sphere* getSpheres() {
		obj_sphere** ls = data.sphereList;
		int count = data.sphereCount;

		if(count == 0) {
			return null;
		}

		obj_vector** vecLs = data.vertexList;

		Sphere* out = Sphere[count];

		int i;
		for(i = 0; i < count; i++) {
			obj_sphere sp = ls[i];
			Vector3 c = objToGenVec(vecLs[sp.pos_index]);
			Vector3 norm = objToGenVec(vecLs[sp.up_normal_index]);
			float r = norm.length();

			Sphere toAdd = Sphere(c, r);
			out[i] = toAdd;
		}
		return out;
	}

	Camera getCamera() {
		obj_camera* cam = data.camera;
		obj_vector** vecLs = data.vertexList;

		Vector3 c = Camera(cam, vecLs);

		return c;
	}

	Primitive* getPrimitives() {
		int count = data.sphereCount;
		Primitive* out = Primitive[count];
		out = getSpheres();
		return out;
	}

	int getPrimCount() {
		return data.sphereCount;
	}


};

#endif