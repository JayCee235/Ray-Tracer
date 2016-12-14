#ifndef __ANIMPATH
#define __ANIMPATH

#include <vector>
#include "vector/GenVector.h"

struct AnimPair {
	Camera* from;
	Camera* to; 
	float t;

	float start;
	float end;
	struct AnimPair* next;
};

class AnimationPath
{
private:
	struct AnimPair* first;
public:
	AnimationPath() {}
	AnimationPath(Camera* start, Camera* next, float t) {

	}
	~AnimationPath() {}

	float getTotalDis() {
		struct AnimPair* cur = first;
		while(cur->next != NULL) {
			cur = cur->next;
		}
		return cur->end;
	}

	Camera* getCameraAt(float t) {
		if(t <= 0) {
			return path[0][0]->from;
		}
		if(t >= 1) {
			return path[0][path->size()]->to;
		}
		float time = t*getTotalDis();


	}


};

#endif