#ifndef __CAMERA
#define __CAMERA

#include <stdio.h>
#include "vector/GenVector.h"

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

class Camera {
private:
	Vector3 p, u, v, w;
public:
	Camera() {}
	Camera(obj_camera* cam, obj_vector** vecLs, obj_vector** normLs) {
		obj_vector* atObj = vecLs[cam->camera_pos_index];
		obj_vector* lookingAtObj = vecLs[cam->camera_look_point_index];
		obj_vector* upObj = normLs[cam->camera_up_norm_index];

		Vector3 at = objToGenVec(atObj);
		Vector3 lookingAt = objToGenVec(lookingAtObj);
		Vector3 up = objToGenVec(upObj);

		if(up == Vector3(0, 0, 0)) {
			up = Vector3(0, 1, 0);
		}

		Vector3 looking = lookingAt - at;

		up.normalize();

		printf("At is: (%f, %f, %f).\n", at[0], at[1], at[2]);
		printf("looking is: (%f, %f, %f).\n", looking[0], looking[1], looking[2]);
		printf("up is: (%f, %f, %f).\n\n", up[0], up[1], up[2]);

		this->p = at;
		this->w = -looking;
		this->w.normalize();
		this->u = up.cross(this->w);
		this->u.normalize();
		this->v = this->w.cross(this->u);
		this->v.normalize();
	}

	Vector3 getP() {
		return this->p;
	}

	Vector3 getU() {
		return this->u;
	}

	Vector3 getV() {
		return this->v;
	}

	Vector3 getW() {
		return this->w;
	}
};

#endif