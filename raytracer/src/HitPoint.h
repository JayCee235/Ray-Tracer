#ifndef __HITPOINT
#define __HITPOINT

#include "Primitive.h"
#include "vector/GenVector.h"

struct HitPoint {
	float t;
	Primitive* p;
	Vector3 normal;
};

void printHitPoint(struct HitPoint* hp) {
	printf("t value: %f.\n", hp->t);
}

#endif