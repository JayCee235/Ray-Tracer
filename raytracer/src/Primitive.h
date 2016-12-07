#ifndef __PRIMSHAPE
#define __PRIMSHAPE

class Primitive;

#include <math.h>
#include "vector/GenVector.h"
#include "RayGenerator.h"
#include "HitPoint.h"

#ifndef CALCQUAD
#define CALCQUAD

struct quadAnswer {
	int numAnswers;
	float ans1;
	float ans2;
};

quadAnswer calculateQuadratic(float a, float b, float c) {
	struct quadAnswer ans;

	float quad = b*b - 4*a*c;
	if(quad < 0) {
		ans.numAnswers = 0;
	} else if(quad == 0) {
		ans.ans1 = -b/(2*a);
		ans.numAnswers = 1;
	} else {
		float ds = sqrt(quad);
		ans.ans1 = (-b-ds)/(2*a);
		ans.ans2 = (-b+ds)/(2*a);
		ans.numAnswers = 2;
	}
	return ans;

}

#endif

class Primitive {
private:

public:
	virtual float intersect(Ray* r) {
		return -1;
	}
	virtual Vector3 getNormal(Ray* r, HitPoint* hp) {
		return Vector3(0, 1, 0);
	}
};
#endif