#include "RayTracer.h"

int main(int argc, char* argv[]) {
	Triangle* t = new Triangle(Vector3(3, 0, 0), Vector3(2, 10, 0), Vector3(1, 0, 10));
	Vector3 testPoint = Vector3(0, 1, 9);
	Vector3 outtt = t->barycentric(testPoint);
	printf("%f, %f, %f\n", outtt[0], outtt[1], outtt[2]);

	Ray* testRay = new Ray(Vector3(0, 0, 0), Vector3(0, 1, 0));
	Triangle* tRay = new Triangle(Vector3(-1, 4, -1), Vector3(0, 4, 1), Vector3(1, 3, -1));

	float check = t->intersect(testRay);
	printf("Check: %f\n", check);

	Vector3 ae = tRay->getA() - testRay->getP();
		float out = ae.dot(tRay->getTriangleNormal());
		printf("dot: %f\n", out);
		if(out > -0.0001 && out < 0.0001) {
			//The point is very close to the plane, so to avoid floating point problems, just return -1.
			printf("fail\n");
			// return -1;
		}
		out = out / (testRay->getD().dot(tRay->getTriangleNormal()));
		printf("t: %f\n", out);
		if(out < 0) {
			//Don't bother with intersection calculations, as the ray doesn't intersect above t=0
			printf("fail\n");
			// return -1;
		}
		Vector3 x = testRay->getPointAt(out);
		printf("Point found: (%.2f, %.2f, %.2f)\n", x[0], x[1], x[2]);
		Vector3 bary = tRay->barycentric(x);
		printf("%f, %f, %f\n", bary[0], bary[1], bary[2]);
		if(bary[0] > -0.0001 && bary[1] > -0.0001 && bary[2] > -0.0001) {
			printf("true\n");
			// return out;
		} else {
			printf("fail\n");
			// return -1;
		}

		Vector3 v0 = tRay->getC() - tRay->getA();
		Vector3 v1 = tRay->getB() - tRay->getA();
		printf("%f, %f, %f\n", v0[0], v0[1], v0[2]);
		printf("%f, %f, %f\n", v1[0], v1[1], v1[2]);
		Vector3 v2 = x - tRay->getA();

		printf("pointToCheck: (%.2f, %.2f, %.2f)\n", v2[0], v2[1], v2[2]);

		float dot00 = v0.dot(v0);
		float dot01 = v0.dot(v1);
		float dot02 = v0.dot(v2);
		float dot11 = v1.dot(v1);
		float dot12 = v1.dot(v2);

		printf("d02, d12: %.2f, %.2f\n", dot02, dot12);

		printf("dots: %.2f %.2f %.2f\n", dot11, dot00, dot01);

		float invDom = 1 / (dot00 * dot11 - dot01 * dot01);
		printf("invDom: %f\n", invDom);
		float u = (dot11 * dot02 - dot01 * dot12) * invDom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDom;

		float w = 1 - u - v;

		Vector3 newBary = Vector3(w, u, v);
		printf("BaryCoordinate: %f, %f, %f\n", newBary[0], newBary[1], newBary[2]);

		printf("Triangle u, v: \n");
		Vector3 uu = tRay->u;
		Vector3 vv = tRay->v;

		printf("%f, %f, %f\n", uu[0], uu[1], uu[2]);
		printf("%f, %f, %f\n", vv[0], vv[1], vv[2]);

		printf("Triangle other info: \n");

		printf("BaryDom: %f\n", tRay->baryDom);
		printf("dots: %.2f %.2f %.2f\n", tRay->u2, tRay->v2, tRay->uv);
}
