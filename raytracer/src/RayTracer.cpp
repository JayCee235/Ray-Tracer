/* RayTracer
 *
 * Written by Jeremiah Caudell
 *
 */

#include "RayTracer.h"


#define RES 200
#define FOV 90

int main(int argc, char* argv[]) {
	Buffer<Color>* buffer = new Buffer<Color>(RES, RES);


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

	//Convert vectors to RGB colors for testing results
	Color white = Color(255.0f, 255.0f, 255.0f);
	Color black = Color(0.0f, 0.0f, 0.0f);
	for(int y=0; y<RES; y++)
	{
		for(int x=0; x<RES; x++)
		{
			Ray* ray = generator->getRay(x, y);
			// Vector3 dir = ray->getDirection()*255.0f;
			// Color col = Color( abs(dir[0]), abs(dir[1]), abs(dir[2]) );
			// buffer->at(x,y) = col;
			buffer->at(x,y) = black;

			struct HitPoint hp;
			float t = scene->intersect(ray, &hp);

			if(t > 0) {
				hp.normal = hp.p->getNormal(ray, &hp);
				Vector3 direction = hp.normal;
				Vector3 dir = direction*255.0f;
				Color col = Color( abs(dir[0]), abs(dir[1]), abs(dir[2]) );


				buffer->at(x,y) = col;
			}

		}
	}

	//Write output buffer to file argv2
	simplePNG_write(argv[2], buffer->getWidth(), buffer->getHeight(), (unsigned char*)&buffer->at(0,0));

	return 0;
}