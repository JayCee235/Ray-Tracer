/* RayTracer
 *
 * Written by Jeremiah Caudell
 *
 */

#include "RayTracer.h"


#define RES 100
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

	//Convert vectors to RGB colors for testing results
	Vector3 white = Vector3(255.0f, 255.0f, 255.0f);
	Vector3 black = Vector3(0.0f, 0.0f, 0.0f);
	for(int y=0; y<RES; y++)
	{
		for(int x=0; x<RES; x++)
		{
			Ray* ray = generator->getRay(x, y);
			// Vector3 dir = ray->getDirection()*255.0f;
			// Vector3 col = Vector3( abs(dir[0]), abs(dir[1]), abs(dir[2]) );
			// buffer->at(x,y) = col;
			buffer->at(x,y) = black;

			struct HitPoint hp;
			float t = scene->intersect(ray, &hp);

			if(t > 0) {
				// printf("Hit!\n");
				hp.normal = hp.p->getNormal(ray, &hp);
				hp.normal.normalize();
				// Vector3 direction = hp.normal;
				// Vector3 dir = direction*255.0f;
				// Vector3 col = Vector3( abs(dir[0]), abs(dir[1]), abs(dir[2]) );
				// printf("Gathering Material from HitPoint...\n");
				Material* m = hp.p->getMaterial();
				// m->printInfo();

				std::vector<Light*>* lightsHit = new std::vector<Light*>();
				std::vector<Ray*>* raysHit = new std::vector<Ray*>();
				int numLightsHit;

				// printf("Getting Lights hit...\n");

				numLightsHit = scene->checkForLights(ray->getPointAt(hp.t), lightsHit, raysHit);
				printf("%d ", numLightsHit);

				// printf("Hit %d lights.\n", numLightsHit);

				Vector3 dif = Vector3(0, 0, 0);
				Vector3 spec = Vector3(0, 0, 0);

				if(numLightsHit > 0) {
					for(int i = 0; i < numLightsHit; i++) {
						Light* work = lightsHit[0][i];
						Ray* workRay = raysHit[0][i];

						float dott = workRay->getD().dot(hp.normal);

						

						if(dott > 0.9999) {
							dott = 1;
						}

						if(dott > 0) {
							Vector3 lightDif = work->getMaterial()->diffuse + Vector3(0, 0, 0);
							lightDif = lightDif * dott;
							dif = dif + lightDif;

							Ray* reflection = workRay->reflect(hp.normal);
							Vector3 reflectionDir = reflection->getD() + Vector3(0, 0, 0);
							reflectionDir.normalize();
							Vector3 hpToCam = camera->getP() - ray->getPointAt(hp.t);
							hpToCam.normalize();
							float specMult = reflectionDir.dot(hpToCam);

							if(specMult > 0.99999) {
								specMult = 1;
							}

							if(specMult > 0) {
								// specMult = pow(specMult, m->shiny);
								Vector3 lightSpec = work->getMaterial()->specular + Vector3(0, 0, 0);
								lightSpec = lightSpec * specMult;
								spec = spec + lightSpec;
							}
							
						}
					}
					// dif = dif / scene->getNumLights();

					// float mx = dif[dif.maxComponent()];
					// if(mx > 1) {
					// 	dif = dif / mx;
					// }
 
					
				}
				Vector3 difC = m->diffuse + Vector3(0, 0, 0);
				difC = difC * dif;

				Vector3 specC = m->specular + Vector3(0, 0, 0);
				specC = specC * spec;

				// printf("%.2f %.2f %.2f   ", difC[0], difC[1], difC[2]);

				// difC = difC / difC.maxComponent();

				Vector3 c = m->ambient + Vector3(0, 0, 0);
				// printf("%f %f %f    ", c[0]*255.0f, c[1]*255.0f, c[2]*255.0f);
				Vector3 ambientFinal = Vector3(c[0], c[1], c[2]);
				Vector3 diffuseFinal = Vector3(difC[0], difC[1], difC[2]);
				Vector3 specularFinal = Vector3(specC[0], specC[1], specC[2]);

				// printf("Specular: (%.2f, %.2f, %.2f)\n", specC[0], specC[1], specC[2]);


				buffer->at(x,y) = ambientFinal + diffuseFinal + specularFinal;
				// buffer->at(x, y) = specularFinal;
				// buffer->at(x, y) = ambientFinal + diffuseFinal;
				buffer->at(x,y) = diffuseFinal + specularFinal;

			}

		}
	}

	Buffer<Color>* image = new Buffer<Color>(RES, RES);
	float max = 1;

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