/* RayTracer
 *
 * Written by Jeremiah Caudell
 *
 */

#include <stdlib.h>
#include "RayTracer.h"
#include <cstring>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int callWindow(Buffer<Vector3>* b, RayGenerator* generator, Shader* shader, int RES);

int main(int argc, char* argv[]) {
	int RES = 500;
	float FOV = 90;

	//Need at least two arguments (obj input and png output)
	if(argc < 2)
	{
		printf("Usage %s input.obj\n", argv[0]);
		exit(1);
	}

	//parse args
	bool implicitCalc = false;
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
		if(strcmp(argv[i], "-calcNormal") == 0) {
			implicitCalc = true;
		}
	}

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

	Loader* loader = new Loader(implicitCalc);
	loader->quietLoad(argv[1]);

	printf("Loader loaded.\n");

	Scene* scene = new Scene();
	printf("New Scene created.\n");
	scene->quietLoad(loader);

	printf("Scene loaded.\n");

	Camera* camera = loader->getCamera();

	printf("Camera loaded.\n");

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
		// if(y%(RES/10) == 0) {
		// 	traceCount++;
		// 	printf("finished %d%%\n", traceCount*10);
		// }
		for(int x=0; x<RES; x++)
		{
			Ray* ray = generator->getRay(x, y);
			buffer->at(x,y) = shader->shadePoint(ray);
		}
	}
	printf("finished.\n");

	callWindow(buffer, generator, shader, RES);

	// // scene->printTree();

	// Blender* b = new Blender();
	// Buffer<Color>* image = b->bufferToImage(buffer);



	//Write output buffer to file argv2
	// simplePNG_write(argv[2], image->getWidth(), image->getHeight(), (unsigned char*)&image->at(0,0));

	return 0;
}

int callWindow(Buffer<Vector3>* buffer, RayGenerator* generator, Shader* shader, int RES) {
	Display *display;
	Window window;
	XEvent event;
	// char* msg = "Hello, World!";
	int s;

	display = XOpenDisplay(NULL);
	s = DefaultScreen(display);

	window = XCreateSimpleWindow(display, RootWindow(display, s), 0, 0, RES, RES, 1,
                           BlackPixel(display, s), WhitePixel(display, s));

	XSelectInput(display, window, KeyPressMask | KeyReleaseMask);

	XMapWindow(display, window);

	bool up=false, down=false, left=false, right=false;

	int lastx=0, lasty=0;
	int x = 0, y = 0;

	// XNextEvent(display, &event);

	for(int yy=0; yy<RES; yy++)
	{
		for(int xx=0; xx<RES; xx++)
		{
			Vector3 ans = buffer->at(xx, yy);
			int red = (int) (255*ans[0]);
			int green = (int) (255*ans[1]);
			int blue = (int) (255*ans[2]);
			int fin = 256*256*red + 256*green + blue;

			XSetForeground(display, DefaultGC(display, s), fin);
			XDrawPoint(display, window, DefaultGC(display, s), xx, yy);
		}
	}

	printf("Displaying...\n");

	while(true) {
		if(XEventsQueued(display, s) > 0) {
			XNextEvent(display, &event);
			if(event.type == KeyPress || event.type == KeyRelease) {
				XKeyEvent e = event.xkey;
				int code = e.keycode;
				if(code == 25) {
					if(event.type == KeyPress) {
						up = true;
					} else {
						up = false;
					}
				}
				if(code == 38) {
					if(event.type == KeyPress) {
						left = true;
					} else {
						left = false;
					}
				}
				if(code == 39) {
					if(event.type == KeyPress) {
						down = true;
					} else {
						down = false;
					}
				}
				if(code == 40) {
					if(event.type == KeyPress) {
						right = true;
					} else {
						right = false;
					}
				}

			}
		}

		int cut = 1;

		float rot = 0.000003 * cut * cut;

		if(up) {
			Camera* cam = generator->getCamera();
			cam = cam->rotateVerticallyAroundFocus(rot);
			generator->setCamera(cam);
		}
		if(down) {
			Camera* cam = generator->getCamera();
			cam = cam->rotateVerticallyAroundFocus(-rot);
			generator->setCamera(cam);
		}
		if(left) {
			Camera* cam = generator->getCamera();
			cam = cam->rotateAroundFocus(-rot);
			generator->setCamera(cam);
		}
		if(right) {
			Camera* cam = generator->getCamera();
			cam = cam->rotateAroundFocus(rot);
			generator->setCamera(cam);
		}

		// int x = 0;
		// int y = 0;
		lastx = rand() % (RES/cut);
		lasty = rand() % (RES/cut);



		// if (lastx < RES/cut) {
		// 	lastx += 1;
		// } else {
		// 	lastx = 0;
		// 	if (lasty < RES/cut) {
		// 		lasty += 1;
		// 	} else {
		// 		lasty = 0;
		// 	}
		// }
		

		x = lastx*cut;
		y = lasty*cut;

		for(int i = 0; i < cut; i++) {
			for(int j = 0; j < cut; j++) {
				Ray* r = generator->getRay(x+i, y+j);
				Vector3 ans = shader->shadePoint(r);
				delete(r);

				int red = (int) (192*ans[0]);
				int green = (int) (192*ans[1]);
				int blue = (int) (192*ans[2]);

				Vector3 old = buffer->at(x+i, y+j);

				int oldRed = (int) (96*old[0]);
				int oldGreen = (int) (96*old[1]);
				int oldBlue = (int) (96*old[2]);

				int fin = 256*256*(red + oldRed) + 256*(green + oldGreen) + blue + oldBlue;

				XSetForeground(display, DefaultGC(display, s), fin);
				XDrawPoint(display, window, DefaultGC(display, s), x+i, y+j);

				buffer->at(x+i, y+j) = Vector3((red+oldRed)/256.0f, (green+oldGreen)/256.0f, (blue+oldBlue)/256.0f);
			}
		}
		// Ray* r = generator->getRay(x, y);
		// Vector3 ans = shader->shadePoint(r);
		// delete(r);

		// int red = (int) (255*ans[0]);
		// int green = (int) (255*ans[1]);
		// int blue = (int) (255*ans[2]);

		// int fin = red + 256*green + 256*256*blue;

		// XSetForeground(display, DefaultGC(display, s), fin);
		// XDrawPoint(display, window, DefaultGC(display, s), x, y);

		// XSetForeground(display, DefaultGC(display, s), 0x00ff0000);
		// // XFillRectangle(display, window, DefaultGC(display, s), 20, 20, 10, 10);
		// XDrawPoint(display, window, DefaultGC(display, s), 20, 20);
		// XDrawString(display, window, DefaultGC(display, s), 50, 50, msg, strlen(msg));
		// if(event.type == KeyPress) break;
	}
	XCloseDisplay(display);

	return 0;
}



// int main(int argc, char* argv[]) {
// 	int RES = 500;
// 	float FOV = 90;

// 	//Need at least two arguments (obj input and png output)
// 	if(argc < 3)
// 	{
// 		printf("Usage %s input.obj output.png\n", argv[0]);
// 		exit(1);
// 	}

// 	//parse args
// 	bool implicitCalc = false;
// 	for(int i = 3; i < argc; i++) {
// 		if(strcmp(argv[i], "-res") == 0) {
// 			int check = atoi(argv[i+1]);
// 			if(check == 0) {
// 				printf("Size not an int//Cannot use zero.\n");
// 				exit(2);
// 			}
// 			if(check < 0) {
// 				printf("size must be positive.\n");
// 				exit(3);
// 			}
// 			RES = check;
// 		}
// 		if(strcmp(argv[i], "-calcNormal") == 0) {
// 			implicitCalc = true;
// 		}
// 	}

// 	Buffer<Vector3>* buffer = new Buffer<Vector3>(RES, RES);

// 	// bool debug = false;

// 	// int i;
// 	// for(i = 0; i < argc; i++) {
// 	// 	char* arg = &argv[i];
// 	// 	if(arg.compare("-d") == 0) {
// 	// 		debug = true;
// 	// 	}
// 	// }

// 	// objLoader objDat = objLoader();
// 	// objDat.load(argv[1]);

// 	Loader* loader = new Loader(implicitCalc);
// 	loader->quietLoad(argv[1]);

// 	printf("Loader loaded.\n");

// 	Scene* scene = new Scene();
// 	printf("New Scene created.\n");
// 	scene->quietLoad(loader);

// 	printf("Scene loaded.\n");

// 	Camera* camera = loader->getCamera();

// 	printf("Camera loaded.\n");

// 	RayGenerator* generator = new RayGenerator(camera, RES, RES, FOV);
// 	printf("Generator ready.\n");

// 	Shader* shader = new Shader(scene, camera, Vector3(0, 0, 0));
// 	printf("Shader ready.\n");

// 	//Convert vectors to RGB colors for testing results
// 	Vector3 white = Vector3(255.0f, 255.0f, 255.0f);
// 	Vector3 black = Vector3(0.0f, 0.0f, 0.0f);
// 	// int traceCount = -1;
// 	for(int y=0; y<RES; y++)
// 	{
// 		// if(y%(RES/10) == 0) {
// 		// 	traceCount++;
// 		// 	printf("finished %d%%\n", traceCount*10);
// 		// }
// 		for(int x=0; x<RES; x++)
// 		{
// 			Ray* ray = generator->getRay(x, y);
// 			buffer->at(x,y) = shader->shadePoint(ray);
// 		}
// 	}
// 	printf("finished.\n");

// 	// scene->printTree();

// 	Blender* b = new Blender();
// 	Buffer<Color>* image = b->bufferToImage(buffer);

// 	//Write output buffer to file argv2
// 	simplePNG_write(argv[2], image->getWidth(), image->getHeight(), (unsigned char*)&image->at(0,0));

// 	return 0;
// }