vecTest.o: src/vector/test.cpp src/vector/GenVector.h
	g++ src/vector/test.cpp -c -o vecTest.o

pngTest.o: src/png/test.c src/png/simplePNG.h
	gcc -std=c99 src/png/test.c -c -o pngTest.o

objLoadTest.o: src/objload/objLoader.hpp src/objload/obj_parser.h src/objload/test.cpp res/test.mtl res/test.obj
	g++ src/objload/test.cpp -c -o objLoadTest.o

bufferTest.o: src/buffer/Buffer.h src/buffer/test.cpp
	g++ src/buffer/test.cpp -c -o bufferTest.o

ioTest.o: src/ioTest.cpp
	g++ src/ioTest.cpp -c -o ioTest.o

ioTest: ioTest.o
	g++ ioTest.o -o ioTest

vecTest: vecTest.o
	g++ vecTest.o -o vecTest

pngTest: pngTest.o
	gcc pngTest.o -o pngTest

objLoadTest: objLoadTest.o
	g++ objLoadTest.o -o objLoadTest

bufferTest: bufferTest.o
	g++ bufferTest.o -o bufferTest

RayTracer.o: src/RayTracer.cpp
	g++ src/RayTracer.cpp -c -o RayTracer.o

RayTracer: RayTracer.o
	g++ RayTracer.o -o RayTracer

SphereTest.o: src/SphereTest.cpp
	g++ src/SphereTest.cpp -c -o SphereTest.o

SphereTest: SphereTest.o
	g++ SphereTest.o -o SphereTest

allTest: vecTest pngTest objLoadTest bufferTest ioTest

runIOTest: ioTest
	./ioTest res/test1.obj res/test1.png
	./ioTest res/test2.obj res/test2.png
	./ioTest res/test3.obj res/test3.png
	./ioTest res/test4.obj res/test4.png
	./ioTest res/test5.obj res/test5.png

runSphereTest: SphereTest
	./SphereTest res/SphereTest1.obj res/SphereTest1.png
	./SphereTest res/SphereTest2.obj res/SphereTest2.png

runTriangleTest: SphereTest
	./SphereTest res/triangleTest1.obj res/triangleTest1.png
	./SphereTest res/triangleTest2.obj res/triangleTest2.png

all: SphereTest

clean: 
	rm -f core *.o  *Test RayTracer