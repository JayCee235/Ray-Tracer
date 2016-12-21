#include "caldQuad.h"

struct quadAnswer {
	int numAnswers;
	float ans1;
	float ans2;
};

quadAnswer calculateQuadratic(float a, float b, float c) {
	struct quadAnswer ans;

	float quad = b*b - 4*a*c;
	if(quad > -0.001 && quad < 0.001) {
		quad = 0;
	}
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