/* RayTracer, Blending
 *
 * Written by Jeremiah Caudell
 *
 */

#include "RayTracer.h"

int main(int argc, char* argv[]) {
	
	int RES = 300;
	int SCALE = 3;
	float FOV = 90;
	bool IMPLICITCALC = false;

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
		if(strcmp(argv[i], "-calcNormal") == 0) {
				IMPLICITCALC = true;
			}
	}
	RES *= SCALE;

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

	Loader* loader = new Loader(IMPLICITCALC);
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
		}
	}

	Buffer<Color>* image = new Buffer<Color>(RES/SCALE, RES/SCALE);
	// Buffer<Color>* image2 = new Buffer<Color>(RES/SCALE, RES/SCALE);

	Blender* b = new Blender();

	image = b->bufferToImage(b->blend(buffer, SCALE));
	// image2 = b->bufferToImage(b->scaleDown(buffer, SCALE));



	//Write output buffer to file argv2
	simplePNG_write(argv[2], image->getWidth(), image->getHeight(), (unsigned char*)&image->at(0,0));
	// simplePNG_write(argv[3], image2->getWidth(), image2->getHeight(), (unsigned char*)&image2->at(0,0));
	printf("Finished.\n");

	return 0;
}