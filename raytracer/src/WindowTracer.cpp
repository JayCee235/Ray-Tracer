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

int callWindow(Buffer<Vector3>* b, RayGenerator* generator, Shader* shader, 
	int xRes, int yRes, int sections, int fade, bool randompx);

int callDoubleWindow(Buffer<Vector3>* rb, Buffer<Vector3>* lb,
	RayGenerator* lGenerator, RayGenerator* rGenerator,
	Shader* lShader, Shader* rShader, 
	int xRes, int yRes, int sections, int fade, bool randompx);

Vector3 updatePixel(int x, int y, RayGenerator* generator, Shader* shader, 
		float* max, Display* display, int s, Buffer<Vector3>* buffer, 
		Window window, int wx, int wy, int oldScale, int newScale);

void printHelp() {
	printf("----------\n");
	printf("-res <res> sets the generated image resoultion to res*res.\n");
	printf("-scale <scale> samples scale*scale times per pixel.\n");
	printf("-calcNormal will invoke implicit calculation of vertex normals if not defined.\n");
	printf("-pxSize <pxSize> defines the size of the refresh area in the window.\n");
	printf("-fadeFactor <fadeFactor> determines the proportion of the old image to be mixed when refreshing.\n");
	printf("----------\n");
	}

int main(int argc, char* argv[]) {
	int RES = 500;
	float FOV = 90;
	int SCALE = 1;
	float EYEDIS = 0.03;

	bool REDBLUE = false;
	bool IMPLICITCALC = false;
	bool OVERLAP = false;
	bool RANDOM = true;

	int fade = 0;
	int section = 1;

	//Need at least two arguments (obj input and png output)
	if(argc < 2)
	{
		printf("Usage %s input.obj [-res <res>] ", argv[0]);
		printf("[-scale <scale>] [-calcNormal] [-pxSize <pxSize>] [-fadeFactor <fadeFactor>]\n");
		exit(1);
	}

	//parse args
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "?") == 0) {
			printHelp();
			exit(0);
		}
		if(strcmp(argv[i], "-res") == 0) {
			int check = atoi(argv[i+1]);
			if(check == 0) {
				printf("Size not an int//Cannot use zero.\n");
				exit(2);
			}
			if(check <= 0) {
				printf("size must be positive.\n");
				exit(3);
			}
			printf("Set res to %d\n", check);
			RES = check;
		}
		if(strcmp(argv[i], "-scale") == 0) {
			int check = atoi(argv[i+1]);
			if(check == 0) {
				printf("scale not an int//Cannot use zero.\n");
				exit(2);
			}
			if(check <= 0) {
				printf("scale must be positive.\n");
				exit(3);
			}
			printf("Set scale to %d\n", check);
			SCALE = check;
		}
		if(strcmp(argv[i], "-rb") == 0) {
			printf("Set red/blue\n");
			REDBLUE = true;
		}
		if(strcmp(argv[i], "-calcNormal") == 0) {
			printf("Set implicit normal calculation\n");
			IMPLICITCALC = true;
		}
		if(strcmp(argv[i], "-depth") == 0) {
			printf("Set overlap\n");
			OVERLAP = true;
		}
		if(strcmp(argv[i], "-refreshLine") == 0) {
			printf("Set pixel refresh to linear\n");
			RANDOM = true;
		}
		if(strcmp(argv[i], "-pxSize") == 0) {
			int check = atoi(argv[i+1]);
			if(check == 0) {
				printf("pxSize not an int//Cannot use zero.\n");
				exit(2);
			}
			if(check <= 0) {
				printf("pxSize must be positive.\n");
				exit(3);
			}
			printf("Set pxSize to %d\n:", check);
			section = check;
		}
		if(strcmp(argv[i], "-fadeFactor") == 0) {
			int check = atoi(argv[i+1]);
			if(check == 0) {
				printf("fadeFactor not an int//Cannot use zero.\n");
				exit(2);
			}
			if(check < 0) {
				printf("fadeFactor must be 0 - 256");
				exit(3);
			}
			if(check > 256) {
				printf("fadeFactor must be 0 - 256");
				exit(3);
			}
			printf("Set fadeFactor to %d\n:", check);
			fade = check;
		}

	}

	RES *= SCALE;
	int xRes = RES;
	int yRes = RES;
	if(!(REDBLUE||OVERLAP)) {
		// xRes *= 2;
	}

	Buffer<Vector3>* lBuffer = new Buffer<Vector3>(xRes, yRes);
	Buffer<Vector3>* rBuffer = new Buffer<Vector3>(xRes, yRes);

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

	if(IMPLICITCALC) {
		printf("Should calculate Normals.\n");
	}

	Loader* loader = new Loader(IMPLICITCALC);
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

	printf("Camera loaded.\n");

	RayGenerator* lGenerator = new RayGenerator(leftCam, xRes, yRes, FOV);
	RayGenerator* rGenerator = new RayGenerator(rightCam, xRes, yRes, FOV);
	printf("Generator ready.\n");

	Shader* lShader = new Shader(scene, leftCam, Vector3(0, 0, 0));
	Shader* rShader = new Shader(scene, rightCam, Vector3(0, 0, 0));
	printf("Shader ready.\n");

	//Convert vectors to RGB colors for testing results
	Vector3 white = Vector3(255.0f, 255.0f, 255.0f);
	Vector3 black = Vector3(0.0f, 0.0f, 0.0f);

	float max = 1;

	int traceCount = -1;
	for(int y=0; y<yRes; y++)
	{
		// if(y%(RES/10) == 0) {
		// 	traceCount++;
		// 	printf("finished %d%%\n", traceCount*10);
		// }
		for(int x=0; x<xRes; x++)
		{
			Ray* ray = lGenerator->getRay(x, y);
			lBuffer->at(x,y) = lShader->shadePoint(ray);

			delete(ray);

			ray = rGenerator->getRay(x, y);
			rBuffer->at(x,y) = rShader->shadePoint(ray);

			delete(ray);

			for(int q = 0; q < 3; q++) {
				if(max < lBuffer->at(x,y)[q]) {
					max = lBuffer->at(x,y)[q];
				}
				if(max < rBuffer->at(x,y)[q]) {
					max = rBuffer->at(x,y)[q];
				}
			}
		}
	}

	for(int y=0; y<yRes; y++)
	{
		for(int x=0; x<xRes; x++)
		{
			lBuffer->at(x, y) /= max;
			rBuffer->at(x, y) /= max;
		}
	}

	printf("finished.\n");

	// int pid = fork();

	// if(pid == 0) {
		if(REDBLUE || OVERLAP) {
			callWindow(lBuffer, lGenerator, lShader, xRes, yRes, section, fade, RANDOM);
		} else {
			callDoubleWindow(lBuffer, rBuffer, lGenerator, rGenerator, 
				lShader, rShader, xRes, yRes, section, fade, RANDOM);
			printf("Window closed.\n");
			return 0;
		}
	// }

	// pid = fork();

	// if(pid == 0) {
	// 	callWindow(rBuffer, rGenerator, rShader, xRes, yRes);
	// 	printf("Window closed.\n");
	// 	return 0;
	// }

	// printf("Waiting...\n");

	// wait(NULL);
	// wait(NULL);

	// printf("Waited.\n");



	// // scene->printTree();

	// Blender* b = new Blender();
	// Buffer<Color>* image = b->bufferToImage(buffer);



	//Write output buffer to file argv2
	// simplePNG_write(argv[2], image->getWidth(), image->getHeight(), (unsigned char*)&image->at(0,0));

	return 0;
}

typedef GenVector<2, int> Pair;

int callWindow(Buffer<Vector3>* buffer, RayGenerator* generator, Shader* shader, 
	int xRes, int yRes, int sections, int fade, bool randompx) {

	int cut = sections;

	int oldScale = fade;
	int newScale = 256 - oldScale;

	float max = 1;

	Display *display;
	Window window;
	XEvent event;
	int s;

	display = XOpenDisplay(NULL);
	s = DefaultScreen(display);

	window = XCreateSimpleWindow(display, RootWindow(display, s), 0, 0, xRes, yRes, 1,
                           BlackPixel(display, s), WhitePixel(display, s));

	XSelectInput(display, window, KeyPressMask | KeyReleaseMask);

	XMapWindow(display, window);

	bool up=false, down=false, left=false, right=false; 
	bool forward = false, backward = false;
	bool goUp = false, goDown = false, goLeft = false, goRight = false;
	bool resetCam = false;

	int lastx=0, lasty=0;
	int x = 0, y = 0;

	Camera* init = new Camera(generator->getCamera());

	Pair* screen = (Pair*) malloc(sizeof(Pair)*xRes*yRes);

	for(int i = 0; i < xRes / cut; i++) {
		for(int j = 0; j < yRes / cut; j++) {
			Pair p;
			p[0] = i;
			p[1] = j;
			screen[i*(yRes/cut) + j] = p;
			// Pair p = screen[i*(yRes / cut) + j];
			// p[0] = i;
			// p[1] = j;
		}
	}

	for(int i = 0; i < (xRes * yRes) / (cut * cut); i++) {
		int index = i + (rand() % ((xRes * yRes)/(cut * cut) - i));
		Pair temp = screen[index];
		screen[index] = screen[i];
		screen[i] = temp;
	}

	XNextEvent(display, &event);

	for(int yy=0; yy<yRes; yy++)
	{
		for(int xx=0; xx<xRes; xx++)
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

	bool run = true;

	int pxIndex = 0;

	float rot = 0.000003 * cut * cut;
	float move = 0.0000003 * cut * cut;

	while(run) {
		if(XEventsQueued(display, s) > 0) {
			XNextEvent(display, &event);
			if(event.type == KeyPress || event.type == KeyRelease) {
				XKeyEvent e = event.xkey;
				int code = e.keycode;
				printf("code: %d\n", code);
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
				if(code == 26) {
					if(event.type == KeyPress) {
						forward = true;
					} else {
						forward = false;
					}
				}
				if(code == 24) {
					if(event.type == KeyPress) {
						backward = true;
					} else {
						backward = false;
					}
				}
				if(code == 113) {
					if(event.type == KeyPress) {
						goLeft = true;
					} else {
						goLeft = false;
					}
				}
				if(code == 111) {
					if(event.type == KeyPress) {
						goUp = true;
					} else {
						goUp = false;
					}
				}
				if(code == 116) {
					if(event.type == KeyPress) {
						goDown = true;
					} else {
						goDown = false;
					}
				}
				if(code == 114) {
					if(event.type == KeyPress) {
						goRight = true;
					} else {
						goRight = false;
					}
				}
				if(code == 41) {
					randompx = false;
				}
				if(code == 42) {
					randompx = true;
				}
				if(code == 9) {
					run = false;
				}
				if(code == 65) {
					if(event.type == KeyPress) {
						resetCam = true;
					}
				}
			}
		}

		if(resetCam) {
			generator->setCamera(init);
			shader->setCamera(init);
			resetCam = false;
		}

		if(up) {
			float toRotate = rot;
			Camera* cam = generator->getCamera();
			cam = cam->rotateVerticallyAroundFocus(toRotate);
			generator->setCamera(cam);
			shader->setCamera(cam);
			delete(cam);
		}
		if(down) {
			float toRotate = -rot;
			Camera* cam = generator->getCamera();
			cam = cam->rotateVerticallyAroundFocus(toRotate);
			generator->setCamera(cam);
			shader->setCamera(cam);
			delete(cam);
		}
		if(left) {
			float toRotate = -rot;
			Camera* cam = generator->getCamera();
			cam = cam->rotateAroundFocus(toRotate);
			generator->setCamera(cam);
			shader->setCamera(cam);
			delete(cam);
		}
		if(right) {
			float toRotate = rot;
			Camera* cam = generator->getCamera();
			cam = cam->rotateAroundFocus(toRotate);
			generator->setCamera(cam);
			shader->setCamera(cam);
			delete(cam);
		}
		if(forward) {
			Camera* cam = generator->getCamera();
			cam = cam->zoomIn(move);
			generator->setCamera(cam);
			shader->setCamera(cam);
			delete(cam);
		}
		if(backward) {
			Camera* cam = generator->getCamera();
			cam = cam->zoomIn(-move);
			generator->setCamera(cam);
			shader->setCamera(cam);
			delete(cam);
		}

		if(goUp) {
			Camera* cam = generator->getCamera();
			cam = cam->travelUp(-move);
			generator->setCamera(cam);
			shader->setCamera(cam);
			delete(cam);
		}
		if(goRight) {
			Camera* cam = generator->getCamera();
			cam = cam->travelRight(-move);
			generator->setCamera(cam);
			shader->setCamera(cam);
			delete(cam);
		}
		if(goLeft) {
			Camera* cam = generator->getCamera();
			cam = cam->travelRight(move);
			generator->setCamera(cam);
			shader->setCamera(cam);
			delete(cam);
		}
		if(goDown) {
			Camera* cam = generator->getCamera();
			cam = cam->travelUp(move);
			generator->setCamera(cam);
			shader->setCamera(cam);
			delete(cam);
		}

		if(randompx) {
			Pair p = screen[pxIndex];
			lastx = p[0];
			lasty = p[1];
			pxIndex++;
			if(pxIndex >= (xRes / cut) * (yRes / cut)) pxIndex = 0;
			// printf("%d: (%d, %d)\n", pxIndex, p[0], p[1]);
			// lastx = rand() % (xRes/cut);
			// lasty = rand() % (yRes/cut);
		} else {
			if (lastx < xRes/cut) {
				lastx += 1;
			} else {
				lastx = 0;
				if (lasty < yRes/cut) {
					lasty += 1;
				} else {
					lasty = 0;
				}
			}
		}

		// lastx = rand() % (xRes/cut);
		// lasty = rand() % (yRes/cut);

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

		for(int i = 0; i < cut && x+i < xRes; i++) {
			for(int j = 0; j < cut && y+j < yRes; j++) {
				buffer->at(x+i, y+j) = updatePixel(x+i, y+j, generator, shader, 
					&max, display, s, buffer, window, x+i, y+j, oldScale, newScale);
			}
		}

		// max = (max + 1) / 2;
		if(!up && !down && !left && !right) {
			// max = (9999*max+1) / 10000;
		}
	}
	XCloseDisplay(display);

	return 0;
}

int callDoubleWindow(Buffer<Vector3>* rBuffer, Buffer<Vector3>* lBuffer,
	RayGenerator* lGenerator, RayGenerator* rGenerator,
	Shader* lShader, Shader* rShader, int xRes, int yRes, int sections, int fade, bool randompx) {

	int cut = sections;

	int oldScale = fade;
	int newScale = 256 - oldScale;

	float max = 1;

	Display *display;
	Window window;
	XEvent event;
	int s;

	display = XOpenDisplay(NULL);
	s = DefaultScreen(display);

	window = XCreateSimpleWindow(display, RootWindow(display, s), 0, 0, 2*xRes, yRes, 1,
                           BlackPixel(display, s), WhitePixel(display, s));

	XSelectInput(display, window, KeyPressMask | KeyReleaseMask);

	XMapWindow(display, window);

	bool up=false, down=false, left=false, right=false;

	int lastx=0, lasty=0;
	int x = 0, y = 0;

	Pair* screen = (Pair*) malloc(sizeof(Pair)*xRes*yRes);

	for(int i = 0; i < xRes / cut; i++) {
		for(int j = 0; j < yRes / cut; j++) {
			Pair p;
			p[0] = i;
			p[1] = j;
			screen[i*(yRes/cut) + j] = p;
			// Pair p = screen[i*(yRes / cut) + j];
			// p[0] = i;
			// p[1] = j;
		}
	}

	for(int i = 0; i < (xRes * yRes) / (cut * cut); i++) {
		int index = i + (rand() % ((xRes * yRes)/(cut * cut) - i));
		Pair temp = screen[index];
		screen[index] = screen[i];
		screen[i] = temp;
	}

	XNextEvent(display, &event);

	for(int yy=0; yy<yRes; yy++)
	{
		for(int xx=0; xx<xRes; xx++)
		{
			Vector3 ans = lBuffer->at(xx, yy);
			int red = (int) (255*ans[0]);
			int green = (int) (255*ans[1]);
			int blue = (int) (255*ans[2]);
			int fin = 256*256*red + 256*green + blue;

			XSetForeground(display, DefaultGC(display, s), fin);
			XDrawPoint(display, window, DefaultGC(display, s), xx, yy);

			ans = rBuffer->at(xx, yy);
			red = (int) (255*ans[0]);
			green = (int) (255*ans[1]);
			blue = (int) (255*ans[2]);
			fin = 256*256*red + 256*green + blue;

			XSetForeground(display, DefaultGC(display, s), fin);
			XDrawPoint(display, window, DefaultGC(display, s), xx + xRes, yy);
		}
	}

	printf("Displaying...\n");

	bool run = true;

	int pxIndex = 0;

	while(run) {
		if(XEventsQueued(display, s) > 0) {
			XNextEvent(display, &event);
			if(event.type == KeyPress || event.type == KeyRelease) {
				XKeyEvent e = event.xkey;
				int code = e.keycode;
				printf("code: %d\n", code);
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
				if(code == 41) {
					randompx = false;
				}
				if(code == 42) {
					randompx = true;
				}
				if(code == 9) {
					run = false;
				}

			}
		}

		float rot = 0.000003 * cut * cut;

		if(up) {
			float toRotate = rot;
			Camera* cam = lGenerator->getCamera();
			cam = cam->rotateVerticallyAroundFocus(toRotate);
			lGenerator->setCamera(cam);
			lShader->setCamera(cam);
			delete(cam);

			cam = rGenerator->getCamera();
			cam = cam->rotateVerticallyAroundFocus(toRotate);
			rGenerator->setCamera(cam);
			rShader->setCamera(cam);
			delete(cam);
		}
		if(down) {
			float toRotate = -rot;
			Camera* cam = lGenerator->getCamera();
			cam = cam->rotateVerticallyAroundFocus(toRotate);
			lGenerator->setCamera(cam);
			lShader->setCamera(cam);
			delete(cam);

			cam = rGenerator->getCamera();
			cam = cam->rotateVerticallyAroundFocus(toRotate);
			rGenerator->setCamera(cam);
			rShader->setCamera(cam);
			delete(cam);
		}
		if(left) {
			float toRotate = -rot;
			Camera* cam = lGenerator->getCamera();
			cam = cam->rotateAroundFocus(toRotate);
			lGenerator->setCamera(cam);
			lShader->setCamera(cam);
			delete(cam);

			cam = rGenerator->getCamera();
			cam = cam->rotateAroundFocus(toRotate);
			rGenerator->setCamera(cam);
			rShader->setCamera(cam);
			delete(cam);
		}
		if(right) {
			float toRotate = rot;
			Camera* cam = lGenerator->getCamera();
			cam = cam->rotateAroundFocus(toRotate);
			lGenerator->setCamera(cam);
			lShader->setCamera(cam);
			delete(cam);

			cam = rGenerator->getCamera();
			cam = cam->rotateAroundFocus(toRotate);
			rGenerator->setCamera(cam);
			rShader->setCamera(cam);
			delete(cam);
		}

		if(randompx) {
			Pair p = screen[pxIndex];
			lastx = p[0];
			lasty = p[1];
			pxIndex++;
			if(pxIndex >= (xRes / cut) * (yRes / cut)) pxIndex = 0;
			// printf("%d: (%d, %d)\n", pxIndex, p[0], p[1]);
			// lastx = rand() % (xRes/cut);
			// lasty = rand() % (yRes/cut);
		} else {
			if (lastx < xRes/cut) {
				lastx += 1;
			} else {
				lastx = 0;
				if (lasty < yRes/cut) {
					lasty += 1;
				} else {
					lasty = 0;
				}
			}
		}
		
		x = lastx*cut;
		y = lasty*cut;

		for(int i = 0; i < cut && x+i < xRes; i++) {
			for(int j = 0; j < cut && y+j < yRes; j++) {
				lBuffer->at(x+i, y+j) = updatePixel(x+i, y+j, lGenerator, lShader, 
					&max, display, s, lBuffer, window, x+i, y+j, oldScale, newScale);
				rBuffer->at(x+i, y+j) = updatePixel(x+i, y+j, rGenerator, rShader, 
					&max, display, s, rBuffer, window, x+i+xRes, y+j, oldScale, newScale);
			}
		}

		// max = (max + 1) / 2;
		if(!up && !down && !left && !right) {
			// max = (9999*max+1) / 10000;
		}
	}
	XCloseDisplay(display);

	return 0;
}

Vector3 updatePixel(int x, int y, RayGenerator* generator, Shader* shader, 
		float* max, Display* display, int s, Buffer<Vector3>* buffer, 
		Window window, int wx, int wy, int oldScale, int newScale) {
	Ray* r = generator->getRay(x, y);
	Vector3 ans = shader->shadePoint(r);
	delete(r);

	for(int q = 0; q < 3; q++) {
		if(ans[q] > *max) {
			*max = ans[q];
		}
	}

	int red = (int) (newScale*ans[0] / *max);
	int green = (int) (newScale*ans[1] / *max);
	int blue = (int) (newScale*ans[2] / *max);

	Vector3 old = buffer->at(x, y);

	int oldRed = (int) (oldScale*old[0]);
	int oldGreen = (int) (oldScale*old[1]);
	int oldBlue = (int) (oldScale*old[2]);

	int fin = 256*256*(red + oldRed) + 256*(green + oldGreen) + blue + oldBlue;

	XSetForeground(display, DefaultGC(display, s), fin);
	XDrawPoint(display, window, DefaultGC(display, s), wx, wy);

	return Vector3(
		(red+oldRed)/256.0f, 
		(green+oldGreen)/256.0f, 
		(blue+oldBlue)/256.0f);
}