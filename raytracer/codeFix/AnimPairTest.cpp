/* RayTracer Animation
 *
 * Written by Jeremiah Caudell
 *
 */

#include "RayTracer.h"
#include "AnimationPath.h"
#include <cstring>
#include <cstdlib>

#ifndef PI
#define PI 3.14159265358
#endif

int main(int argc, char* argv[]) {
	int RES = 500;
	float FOV = 90;
	int SCALE = 3;

	int frames = 100;

	//Need at least two arguments (obj input and png output)
	if(argc < 3)
	{
		printf("Usage %s input.obj output\nThe file will attach [a-p].png on the 16 images.\n", argv[0]);
		exit(1);
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
	}



	RES = RES*SCALE;

	char* dest = (char*) malloc(200);
	char* cur = dest;
	char* from = argv[2];

	for(int i = 0; i < 194; i++) {
		if(*from == '\0') break;
		*cur = *from;
		cur++;
		from++;
	}

	*(cur+2) = '.';
	*(cur+3) = 'p';
	*(cur+4) = 'n';
	*(cur+5) = 'g';
	*(cur+6) = '\0';


	Buffer<Vector3>* buffer = new Buffer<Vector3>(RES, RES);

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

	Camera* init = loader->getCamera();
	Camera* mid = init->changeLookingAt(init->getLookingAt() + Vector3(100, 0, 0));
	Camera* finalCamera = mid->travelForward(mid->getFocusDistance() * 0.9);
	Camera* actualFinalCamera = finalCamera->changeLookingAt(finalCamera->getLookingAt() + Vector3(0, 10, 0));

	delete(loader);

	printf("Generating animation path...\n");
	AnimationPath* ap = new AnimationPath(init, mid, 1);
	ap->addCamera(finalCamera, 1);
	ap->addCamera(actualFinalCamera, 2);
	ap->addCamera(init, 2);

	printf("Camera loaded.\n");

	char* tmp = (char*) malloc(100);

	for(int i = 0; i < frames; i++) {

		sprintf(tmp, "%02d", i);

		*cur = *tmp;
		*(cur+1) = *(tmp+1);
		float time = (i+0.0f) / (frames + 0.0f);
		Camera* camera = ap->getCameraAt(time);
		RayGenerator* generator = new RayGenerator(camera, RES, RES, FOV);
		printf("Generator ready.\n");

		Shader* shader = new Shader(scene, camera, Vector3(0, 0, 0));
		printf("Shader ready.\n");

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
				Ray* ray = generator->getRay(x, y);
				buffer->at(x,y) = shader->shadePoint(ray);
				delete(ray);
			}
		}
		printf("finished 100%%\n");

	// scene->printTree();

		Blender* b = new Blender();
		Buffer<Color>* image = b->bufferToImage(b->blend(buffer, SCALE));

		//Write output buffer to file argv2
		printf("%s \n", dest);
		simplePNG_write(dest, image->getWidth(), image->getHeight(), (unsigned char*)&image->at(0,0));

		delete(camera);
		delete(generator);
		delete(shader);
		delete(b);
		delete(image);
	}	

	return 0;
}