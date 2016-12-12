/* RayTracer
 *
 * Written by Jeremiah Caudell
 *
 */

#include "RayTracer.h"


#define RES 300
#define FOV 90


int main(int argc, char* argv[]) {
	Buffer<Vector3>* buffer = new Buffer<Vector3>(RES, RES);


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

	printf("Camera loaded.\n");

	RayGenerator* generator = new RayGenerator(camera, RES, RES, FOV);

	printf("Generator ready.\n");

	Shader* shader = new Shader(scene, camera, Vector3(0, 0, 0));

	//Convert vectors to RGB colors for testing results
	Vector3 white = Vector3(255.0f, 255.0f, 255.0f);
	Vector3 black = Vector3(0.0f, 0.0f, 0.0f);
	for(int y=0; y<RES; y++)
	{
		for(int x=0; x<RES; x++)
		{
			Ray* ray = generator->getRay(x, y);
			buffer->at(x,y) = shader->shadePoint(ray);
		}
	}

	Buffer<Color>* image = new Buffer<Color>(RES, RES);
	float max = 0;

	for(int x = 0; x < RES; x++) {
		for(int y = 0; y < RES; y++) {
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

	for(int x = 0; x < RES; x++) {
		for(int y = 0; y < RES; y++) {
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