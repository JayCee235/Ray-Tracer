#include "RayTracer.h"

int main(int argc, char* argv[]) {
	Buffer<Color> buffer = Buffer<Color>(RES, RES);


	//Need at least two arguments (obj input and png output)
	if(argc < 3)
	{
		printf("Usage %s input.obj output.png\n", argv[0]);
		exit(0);
	}

	// objLoader objDat = objLoader();
	// objDat.load(argv[1]);

	Loader l = Loader(argv[1]);

	Scene s = new Scene(l);

	Camera camera = Camera(objDat.camera, objDat.vertexList);

	RayGenerator generator = RayGenerator(&camera, RES, RES, FOV);

	//Convert vectors to RGB colors for testing results
	for(int y=0; y<RES; y++)
	{
		for(int x=0; x<RES; x++)
		{
			Ray r = generator.getRay(x, y);
			Vector3 d = r.getDirection()*255.0f;
			Color c = Color( abs(d[0]), abs(d[1]), abs(d[2]) );
			buffer.at(x,y) = c;
		}
	}

	//Write output buffer to file argv2
	simplePNG_write(argv[2], buffer.getWidth(), buffer.getHeight(), (unsigned char*)&buffer.at(0,0));

	return 0;
}