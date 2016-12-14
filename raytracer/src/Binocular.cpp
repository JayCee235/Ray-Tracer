/* Binocular RayTracer
 *
 * Written by Jeremiah Caudell
 *
 */

#include "RayTracer.h"



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
	
	int RES = 300;
	int SCALE = 3;
	float FOV = 90;
	float EYEDIS = 0.03;

	//Need at least two arguments (obj input and png output)
	if(argc < 3)
	{
		printf("Usage %s input.obj output.png\n", argv[0]);
		exit(0);
	}

	//parse args
	for(int i = 3; i < argc; i++) {
		if(strcmp(argv[i], "-res") == 0) {
			int check = atoi(argv[i+1]);
			if(check == 0) {
				printf("Size not an int//Cannot use zero.\n");
				exit(2);
			}
			if(check < 0) {
				printf("size must be positive.\n");
				exit(3);
			}
			RES = check;
		}
		if(strcmp(argv[i], "-scale") == 0) {
			int check = atoi(argv[i+1]);
			if(check == 0) {
				printf("scale not an int//Cannot use zero.\n");
				exit(2);
			}
			if(check < 0) {
				printf("scale must be positive.\n");
				exit(3);
			}
			SCALE = check;
		}
	}
	RES *= SCALE;

	Buffer<Vector3>* buffer = new Buffer<Vector3>(2*RES, RES);

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
	float dis = camera->getFocusDistance();
	Camera* leftCam = camera->getOffset(-EYEDIS*dis);
	Camera* rightCam = camera->getOffset(EYEDIS*dis);

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
	int traceCount = -1;
	for(int y=0; y<RES; y++)
	{
		if(y%(RES/10) == 0) {
			traceCount++;
			printf("finished %d%%\n", traceCount*10);
		}
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

	printf("Blending...\n");

	Blender* b = new Blender();
	Buffer<Color>* image = b->bufferToImage(b->blend(buffer, SCALE));



	// Buffer<Color>* image = new Buffer<Color>(buffer->getWidth(), buffer->getHeight());
	// float max = 0;

	// for(int x = 0; x < image->getWidth(); x++) {
	// 	for(int y = 0; y < image->getHeight(); y++) {
	// 		Vector3 work = buffer->at(x, y);
	// 		if(work[0] > max) {
	// 			max = work[0];
	// 		}
	// 		if(work[1] > max) {
	// 			max = work[1];
	// 		}
	// 		if(work[2] > max) {
	// 			max = work[2];
	// 		}
	// 	}
	// }

	// for(int x = 0; x < image->getWidth(); x++) {
	// 	for(int y = 0; y < image->getHeight(); y++) {
	// 		Vector3 ccc = buffer->at(x, y);
	// 		ccc = ccc * 255.0f / max;
	// 		Color fin = Color(ccc[0], ccc[1], ccc[2]);
	// 		image->at(x, y) = fin;
	// 	}
	// }

	//Write output buffer to file argv2
	simplePNG_write(argv[2], image->getWidth(), image->getHeight(), (unsigned char*)&image->at(0,0));

	return 0;
}