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
	g++ -O3 src/RayTracer.cpp -c -o RayTracer.o

RayTracer: RayTracer.o
	g++ -O3 RayTracer.o -o RayTracer

Blend.o: src/Blend.cpp
	g++ -O3 src/Blend.cpp -c -o Blend.o

Blend: Blend.o
	g++ -O3 Blend.o -o Blend

Binocular.o: src/Binocular.cpp
	g++ -O3 src/Binocular.cpp -c -o Binocular.o

Binocular: Binocular.o
	g++ -O3 Binocular.o -o Binocular

Animation.o: src/Animation.cpp
	g++ -O3 src/Animation.cpp -c -o Animation.o

Animation: Animation.o
	g++ -O3 Animation.o -o Animation

SphereTest.o: src/SphereTest.cpp
	g++ src/SphereTest.cpp -c -o SphereTest.o

SphereTest: SphereTest.o
	g++ SphereTest.o -o SphereTest

allTest: vecTest pngTest objLoadTest bufferTest ioTest SphereTest

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

runNormalShadingTest: RayTracer
	./RayTracer res/NormalShadingTest1.obj res/NormalShadingTest1.png
	./RayTracer res/NormalShadingTest2.obj res/NormalShadingTest2.png

runBlueSphereTest: RayTracer
	./RayTracer res/blue_sphere.obj res/blue_sphere.png

runCornellBoxTest: RayTracer
	./RayTracer res/cornell_box.obj res/cornell_box.png

runMultiSphereTest: RayTracer
	./RayTracer res/spheres.obj res/spheres.png

runTestNewCode: RayTracer
	./RayTracer res/cornell_box.obj res/cornell_box_test.png

runBinocularTest: Binocular
	./Binocular res/cornell_box.obj res/cornell_box_binocular.png
	./Binocular res/spheres.obj res/spheres_binocular.png
	./Binocular res/blue_sphere.obj res/blue_sphere_binocular.png

runBlendTest: Blend
	./Blend res/cornell_box.obj res/cornell_box_blended.png res/cornell_box_unblended.png
	./Blend res/spheres.obj res/spheres_blended.png res/spheres_unblended.png
	./Blend res/blue_sphere.obj res/blue_sphere_blended.png res/blue_sphere_unblended.png

runBunnyTests: RayTracer Blend Binocular
	./RayTracer res/bunny-scene.obj res/bunny-scene.png -res 1000
	./Blend res/bunny-scene.obj res/bunny-scene_blended.png res/bunny-scene_unblended.png -res 1000 -scale 3
	./Binocular res/bunny-scene.obj res/bunny-scene_binocular.png -res 1000 -scale 3	

megaBunny: Blend
	./Blend res/bunny-scene.obj res/bunny-scene_mega.png res/bunny-scene_lessMega.png -res 100 -scale 100

runHappyTest: RayTracer res/happy-scene.obj res/happy-scene.mtl
	./RayTracer res/happy-scene.obj res/happy-scene.png -res 1000

runHappyTestBlend: Blend res/happy-scene.obj res/happy-scene.mtl
	./Blend res/happy-scene.obj res/happy-scene_blended.png res/happy-scene_unblended.png -res 1000 -scale 3

runHappyTestBinocular: Binocular res/happy-scene.obj res/happy-scene.mtl
	./Binocular res/happy-scene.obj res/happy-scene_binocular.png -res 1000 -scale 3

bunnyIcon: RayTracer
	./RayTracer res/bunny-scene.obj res/bunny_icon.png -res 32

megaResCornell: Blend
	./Blend res/cornell_box.obj res/cornell_box_hd.png -res 100 -scale 10

megaResSpheres: Blend
	./Blend res/spheres.obj res/spheres_hd.png -res 100 -scale 10


all: RayTracer Binocular Blend

clean: 
	rm -f core *.o  *Test RayTracer Binocular Blend