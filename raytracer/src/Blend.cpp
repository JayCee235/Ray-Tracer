/* RayTracer
 *
 * Written by Jeremiah Caudell
 *
 */

#include "RayTracer.h"


#define RES 1000
#define FOV 90
#define FAVOUR 0.5
#define EYELENGTH 3

Vector3 filterRed(Vector3 in) {
	return Vector3(0, in[1], in[2]);
}

Vector3 filterGreen(Vector3 in) {
	return Vector3(in[0], 0, in[2]);
}

Vector3 filterBlue(Vector3 in) {
	return Vector3(in[0], in[1], 0);
}


int main(int argc, char* argv[]) {
	// Buffer<Vector3>* lbuffer = new Buffer<Vector3>(RES, RES);
	// Buffer<Vector3>* rbuffer = new Buffer<Vector3>(RES, RES);
	Buffer<Vector3>* buffer = new Buffer<Vector3>(2*RES, RES);


	//Need at least two arguments (obj input and png output)
	if(argc < 3)
	{
		printf("Usage %s input.obj output.png\n [-d]", argv[0]);
		exit(0);
	}

	// bool debug = false;

	// int i;
	// for(i = 0; i < argc; i++) {
	// 	char* arg = &argv[i];
	// 	if(arg.compare("-d") == 0) {
	// 		debug = true;
	// 	}
	// }

	// objLoader objDat = objLoader();
	// objDat.load(argv[1]);

	Loader* loader = new Loader();
	loader->load(argv[1]);

	printf("Loader loaded.\n");

	Scene* scene = new Scene();
	printf("New Scene created.\n");
	scene->load(loader);

	printf("Scene loaded.\n");

	Camera* camera = loader->getCamera();
	Camera* leftCam = camera->getOffset(-EYELENGTH);
	Camera* rightCam = camera->getOffset(EYELENGTH);

	printVector(leftCam->getW());
	printVector(rightCam->getW());

	printf("Camera loaded.\n");

	RayGenerator* leftGen = new RayGenerator(leftCam, RES, RES, FOV);
	RayGenerator* rightGen = new RayGenerator(rightCam, RES, RES, FOV);

	printf("Generator ready.\n");

	Shader* leftShader = new Shader(scene, leftCam, Vector3(0, 0, 0));
	Shader* rightShader = new Shader(scene, rightCam, Vector3(0, 0, 0));

	//Convert vectors to RGB colors for testing results
	Vector3 white = Vector3(255.0f, 255.0f, 255.0f);
	Vector3 black = Vector3(0.0f, 0.0f, 0.0f);
	for(int y=0; y<RES; y++)
	{
		for(int x=0; x<RES; x++)
		{
			Ray* lray = leftGen->getRay(x, y);
			Ray* rray = rightGen->getRay(x, y);
			Vector3 left = leftShader->shadePoint(lray);
			Vector3 right = rightShader->shadePoint(rray);
			// left = filterRed(left);
			// right = filterBlue(right);
			// right = filterGreen(right);
			// buffer->at(x, y) = right + left;
			buffer->at(x, y) = left;
			buffer->at(x+RES, y) = right;
		}
	}

	Buffer<Color>* image = new Buffer<Color>(buffer->getWidth(), buffer->getHeight());
	float max = 0;

	for(int x = 0; x < image->getWidth(); x++) {
		for(int y = 0; y < image->getHeight(); y++) {
			Vector3 work = buffer->at(x, y);
			if(work[0] > max) {
				max = work[0];
			}
			if(work[1] > max) {
				max = work[1];
			}
			if(work[2] > max) {
				max = work[2];
			}
		}
	}

	for(int x = 0; x < image->getWidth(); x++) {
		for(int y = 0; y < image->getHeight(); y++) {
			Vector3 ccc = buffer->at(x, y);
			ccc = ccc * 255.0f / max;
			Color fin = Color(ccc[0], ccc[1], ccc[2]);
			image->at(x, y) = fin;
		}
	}

	//Write output buffer to file argv2
	simplePNG_write(argv[2], image->getWidth(), image->getHeight(), (unsigned char*)&image->at(0,0));

	return 0;
}