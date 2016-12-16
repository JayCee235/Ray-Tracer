#include "objToGenVec.h"

Vector3 objToGenVec(obj_vector const * objVec)
{
	Vector3 toReturn;
	toReturn[0] = objVec->e[0];
	toReturn[1] = objVec->e[1];
	toReturn[2] = objVec->e[2];
	return toReturn;
}