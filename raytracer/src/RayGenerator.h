#ifndef __RAY_GEN
#define __RAY_GEN

#define PI 3.141592653589793238

#include <stdio.h>
#include <math.h>
#include "objload/objLoader.hpp"
#include "vector/GenVector.h"
#include "camera.h"
#include "Ray.h"

float cot(float rad) {
	float c = cos(rad);
	float s = sin(rad);
	return c/s;
}

class RayGenerator {
private:
	Camera* camera;
	int width, height;
	float fov, dis;
	float w2, h2;

public:
	RayGenerator(Camera* cam, int width, int height, float fov) {
		this->camera = cam;
		this->width = width;
		this->height = height;
		this->fov = fov;

		float fovRad = fov * PI / 180.0;

		float fovCot = cot(fovRad/2);


		this->dis = fovCot * this->width / 2;

		this->w2 = width/2.0;
		this->h2 = width/2.0;

	}

	Ray getRay(int x, int y) {
		Vector3 out;
		float xC = (x+0.5) - this->w2;
		Vector3 xDir = -this->camera->getU();
		float yC = (y+0.5) - this->h2;
		Vector3 yDir = this->camera->getV();
		float zC = this->dis;
		Vector3 zDir = this->camera->getW();

		out = xC*xDir + yC*yDir + zC*zDir;
		out.normalize();

		if(x == 43 && y == 22) {
			printf("Coefficients are: %f, %f, %f.\n Camera u is: (%f, %f, %f).\n Out direction is: (%f, %f, %f)\n\n", 
				xC, yC, zC, 
				xDir[0], xDir[1], xDir[2], 
				out[0], out[1], out[2]);
		}
		return Ray(this->camera->getP(), out);
	}
};

#endif