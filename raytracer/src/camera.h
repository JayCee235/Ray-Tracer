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
	Vector3 at, lookingAt, up;
	Vector3 p, u, v, w;
public:
	Camera() {}
	Camera(Vector3 p, Vector3 u, Vector3 v, Vector3 w) {
		this->p = p + Vector3(0, 0, 0);
		this->u = u + Vector3(0, 0, 0);
		this->v = v + Vector3(0, 0, 0);
		this->w = w + Vector3(0, 0, 0);
	}

	Camera(Camera* start, Camera* end, float time) {
		float s = time;
		float t = 1-time;

		Vector3 newAt = start->at * s + end->at * t;
		Vector3 newLookingAt = start->lookingAt * s + end->lookingAt * t;
		Vector3 newUp = start->up * s + end->up * t;

		if(newUp == Vector3(0, 0, 0)) {
			newUp = Vector3(0, 1, 0);
		}

		Vector3 looking = newLookingAt - newAt;

		if(looking == Vector3(0, 0, 0)) {
			looking = Vector3(0, 0, -1);
		}

		newUp.normalize();

		this->at = newAt;
		this->lookingAt = newLookingAt;
		this->up = newUp;

		this->p = newAt;
		this->w = -looking;
		this->w.normalize();
		this->u = newUp.cross(this->w);
		this->u.normalize();
		this->v = this->w.cross(this->u);
		this->v.normalize();
	}

	Camera(Camera* copy) {
		this->at = copy->at;
		this->lookingAt = copy->lookingAt;
		this->up = copy->up;
		this->p = copy->p;
		this->u = copy->u;
		this->v = copy->v;
		this->w = copy->w;
	}

	Camera(Vector3 at, Vector3 lookingAt, Vector3 up) {
		if(up == Vector3(0, 0, 0)) {
			up = Vector3(0, 1, 0);
		}

		Vector3 looking = lookingAt - at;

		if(looking == Vector3(0, 0, 0)) {
			looking = Vector3(0, 0, -1);
		}

		up.normalize();

		this->at = at;
		this->lookingAt = lookingAt;
		this->up = up;

		this->p = at;
		this->w = -looking;
		this->w.normalize();
		this->u = up.cross(this->w);
		this->u.normalize();
		this->v = this->w.cross(this->u);
		this->v.normalize();
	}

	Camera(obj_camera* cam, obj_vector** vecLs, obj_vector** normLs) {
		if(vecLs != NULL) {
			obj_vector* atObj = vecLs[cam->camera_pos_index];
			obj_vector* lookingAtObj = vecLs[cam->camera_look_point_index];
			this->at = objToGenVec(atObj);
			this->lookingAt = objToGenVec(lookingAtObj);
		} else {
			this->at = Vector3(0, 0, 0);
			this->lookingAt = Vector3(0, 0, -100);
		}
		if(normLs!=NULL) {
			obj_vector* upObj = normLs[cam->camera_up_norm_index];
			this->up = objToGenVec(upObj);
		} else {
			this->up = Vector3(0, 1, 0);
		}
		printf("Loaded vectors.\n");

		
		// this->up = objToGenVec(upObj);

		if(up == Vector3(0, 0, 0)) {
			up = Vector3(0, 1, 0);
		}

		Vector3 looking = lookingAt - at;

		if(looking == Vector3(0, 0, 0)) {
			looking = Vector3(0, 0, -1);
		}

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

	float getFocusDistance() {
		Vector3 looking = lookingAt - at;
		return looking.length();
	}

	Camera* rotateVerticallyAroundFocus(float rad) {
		Vector3 newLookingAt = this->lookingAt;
		Vector3 newUp = this->up;
		Vector3 newAt = this->at;
		newAt -= newLookingAt;

		float radius = this->getFocusDistance();

		float x1 = cos(rad)*radius;
		float y1 = sin(rad)*radius;

		// printf("%.2f, %.2f, %.2f\n", radius, x1, y1);

		newAt -= this->w * radius;
		newAt += this->w * x1;
		newAt += this->v * y1;

		newAt += newLookingAt;

		return new Camera(newAt, newLookingAt, newUp);
	}

	Camera* rotateAroundFocus(float rad) {
		Vector3 newLookingAt = this->lookingAt;
		Vector3 newUp = this->up;
		Vector3 newAt = this->at;
		newAt -= newLookingAt;

		float radius = this->getFocusDistance();

		float x1 = cos(rad)*radius;
		float y1 = sin(rad)*radius;

		// printf("%.2f, %.2f, %.2f\n", radius, x1, y1);

		newAt -= this->w * radius;
		newAt += this->w * x1;
		newAt += this->u * y1;

		newAt += newLookingAt;

		return new Camera(newAt, newLookingAt, newUp);

	}

	Camera* focus(Scene* s) {
		Ray* ray = new Ray(this->at, this->w * -1);
		HitPoint hp;
		float foc = s->intersect(r, &hp);
		if(foc > 0) {
			this->lookingAt = ray->getPointAt(hp.t);
		}
	}

	Camera* zoomIn(float dis) {
		Vector3 newAt = this->at - this->w * dis;
		Vector3 newUp = Vector3(this->up);
		Vector3 newLookingAt = Vector3(this->lookingAt);

		return new Camera(newAt, newLookingAt, newUp);
	}

	Camera* travelForward(float dis) {
		Vector3 newAt = this->at - this->w * dis;
		Vector3 newUp = Vector3(this->up);
		Vector3 newLookingAt = this->lookingAt - this->w * dis;

		return new Camera(newAt, newLookingAt, newUp);
	}

	Camera* travelRight(float dis) {
		Vector3 newAt = this->at - this->u * dis;
		Vector3 newUp = Vector3(this->up);
		Vector3 newLookingAt = this->lookingAt - this->u * dis;

		return new Camera(newAt, newLookingAt, newUp);
	}

	Camera* travelUp(float dis) {
		Vector3 newAt = this->at - this->v * dis;
		Vector3 newUp = Vector3(this->up);
		Vector3 newLookingAt = this->lookingAt - this->v * dis;

		return new Camera(newAt, newLookingAt, newUp);
	}

	Camera* changeLookingAt(Vector3 newLookingAt) {
		Vector3 newAt = this->at;
		Vector3 newUp = this->up;
		return new Camera(newAt, newLookingAt, newUp);
	}

	Camera* getOffset(float dis) {
		Vector3 at = this->at + Vector3(0, 0, 0);
		Vector3 lookingAt = this->lookingAt + Vector3(0, 0, 0);
		Vector3 up = this->up + Vector3(0, 0, 0);

		if(up == Vector3(0, 0, 0)) {
			up = Vector3(0, 1, 0);
		}

		Vector3 looking = lookingAt - at;

		if(looking == Vector3(0, 0, 0)) {
			looking = Vector3(0, 0, -1);
		}

		up.normalize();

		Vector3 right = looking.cross(up);
		right.normalize();

		at = at - right*dis;

		looking = lookingAt - at;

		if(looking == Vector3(0, 0, 0)) {
			looking = Vector3(0, 0, -1);
		}

		Vector3 p = at;
		Vector3 w = -looking;
		w.normalize();
		Vector3 u = up.cross(w);
		u.normalize();
		Vector3 v = w.cross(u);
		v.normalize();

		Camera* out = new Camera(p, u, v, w);
		out->up = up;
		out->lookingAt = lookingAt;
		out->at = at;

		return out;

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

	Vector3 getAt() {
		return this->at * 1;
	}

	Vector3 getLookingAt() {
		return this->lookingAt * 1;
	}

	Vector3 getUp() {
		return this->up * 1;
	}
};

#endif