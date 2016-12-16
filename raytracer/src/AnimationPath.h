#ifndef __ANIMPATH
#define __ANIMPATH

#include <vector>
#include "vector/GenVector.h"

struct AnimPair {
	Camera* from;
	Camera* to; 

	float start;
	float end;
	struct AnimPair* next;
};

class AnimationPath
{
private:
	struct AnimPair* first;
	struct AnimPair* last;
public:
	AnimationPath() {}
	AnimationPath(Camera* start, Camera* next, float t) {
		this->first = (struct AnimPair*) malloc(sizeof(AnimPair));
		this->first->from = start;
		this->first->to = next;
		this->first->start = 0;
		this->first->end = t;
		this->last = this->first;

	}
	~AnimationPath() {}
	void addCamera(Camera* camera, float timeToReach) {
		struct AnimPair* newPair = (struct AnimPair*) malloc(sizeof(struct AnimPair));
		newPair->from = last->to;
		newPair->to = camera;
		newPair->start = last->end;
		newPair->end = last->end + timeToReach;

		last->next = newPair;
		this->last = newPair;
	}

	float getTotalDis() {
		return last->end;
	}

	Camera* getCameraAt(float t) {
		printf("Getting camera at %.2f\n", t);
		if(t < 0) {
			return first->from;
		}
		float time = t*getTotalDis();

		struct AnimPair* cur = first;
		while(cur != NULL) {
			if(time >= cur->start && time < cur->end) {
				float fav = (cur->end - time) / (cur->end - cur->start);
				return new Camera(cur->from, cur->to, fav);
			}
			cur = cur->next;
		}	
		return new Camera(last->to);
	}


};

#endif