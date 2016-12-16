#Classes
# AABB.o: src/AABB.cpp
# 	g++ src/AABB.cpp -c -O3 -o AABB.o

# AnimationPath.o: src/AnimationPath.cpp
# 	g++ src/AnimationPath.cpp -c -O3 -o AnimationPath.o

# Blender.o: src/Blender.cpp
# 	g++ src/Blender.cpp -c -O3 -o Blender.o

# BVHNode.o: src/BVHNode.cpp
# 	g++ src/BVHNode.cpp -c -O3 -o BVHNode.o

# BVHTree.o: src/BVHTree.cpp
# 	g++ src/BVHTree.cpp -c -O3 -o BVHTree.o

# calcQuad.o: src/calcQuad.cpp
# 	g++ src/calcQuad.cpp -c -O3 -o BVHTree.o

# Camera.o: src/camera.cpp
# 	g++ src/camera.cpp -c -O3 -o Camera.o

# HitPoint.o: src/HitPoint.cpp
# 	g++ src/HitPoint.cpp -c -O3 -o HitPoint.o

# Light.o: src/Light.cpp
# 	g++ src/Light.cpp -c -O3 -o Light.o

# Loader.o: src/Loader.cpp
# 	g++ src/Loader.cpp -c -O3 -o Loader.o

# Material.o: src/Material.cpp
# 	g++ src/Material.cpp -c -O3 -o Material.o

# ObjToGenVec.o: src/ObjToGenVec.cpp
# 	g++ src/ObjToGenVec.cpp -c -O3 -o ObjToGenVec.o

# Primitive.o: src/Primitive.cpp
# 	g++ src/Primitive.cpp -c -O3 -o Primitive.o

# Ray.o: src/Ray.cpp
# 	g++ src/Ray.cpp -c -O3 -o Ray.o

# RayGenerator.o: src/RayGenerator.cpp
# 	g++ src/RayGenerator.cpp -c -O3 -o RayGenerator.o

# Scene.o: src/Scene.cpp
# 	g++ src/Scene.cpp -c -O3 -o Scene.o

# Sphere.o: src/Sphere.cpp
# 	g++ src/Sphere.cpp -c -O3 -o Sphere.o

# Triangle.o: src/Triangle.cpp
# 	g++ src/Triangle.cpp -c -O3 -o Triangle.o



RayTracer.o: src/RayTracer.cpp
	g++ -O3 src/RayTracer.cpp -c -o RayTracer.o

#RayTracer: RayTracer.o AABB.o AnimationPath.o Blender.o BVHTree.o BVHNode.o calcQuad.o Camera.o HitPoint.o Light.o Loader.o Material.o ObjToGenVec.o Primitive.o Ray.o RayGenerator.o Scene.o Sphere.o Triangle.o
#	g++ -O3 RayTracer.o -o RayTracer

RayTracer: RayTracer.o
	g++ -O3 RayTracer.o -o RayTracer

Blend.o: src/Blend.cpp
	g++ -O3 src/Blend.cpp -c -o Blend.o

Blend: Blend.o #AABB.o AnimationPath.o Blender.o BVHTree.o BVHNode.o calcQuad.o Camera.o HitPoint.o Light.o Loader.o Material.o ObjToGenVec.o Primitive.o Ray.o RayGenerator.o Scene.o Sphere.o Triangle.o
	g++ -O3 Blend.o -o Blend

Binocular.o: src/Binocular.cpp
	g++ -O3 src/Binocular.cpp -c -o Binocular.o

Binocular: Binocular.o #AABB.o AnimationPath.o Blender.o BVHTree.o BVHNode.o calcQuad.o Camera.o HitPoint.o Light.o Loader.o Material.o ObjToGenVec.o Primitive.o Ray.o RayGenerator.o Scene.o Sphere.o Triangle.o
	g++ -O3 Binocular.o -o Binocular

Animation.o: src/Animation.cpp
	g++ -O3 src/Animation.cpp -c -o Animation.o

Animation: Animation.o #AABB.o AnimationPath.o Blender.o BVHTree.o BVHNode.o calcQuad.o Camera.o HitPoint.o Light.o Loader.o Material.o ObjToGenVec.o Primitive.o Ray.o RayGenerator.o Scene.o Sphere.o Triangle.o
	g++ -O3 Animation.o -o Animation

AnimPairTest.o: src/AnimPairTest.cpp
	g++ -O3 src/AnimPairTest.cpp -c -o AnimationPath.o

AnimPairTest: AnimPairTest.o #AABB.o AnimationPath.o Blender.o BVHTree.o BVHNode.o calcQuad.o Camera.o HitPoint.o Light.o Loader.o Material.o ObjToGenVec.o Primitive.o Ray.o RayGenerator.o Scene.o Sphere.o Triangle.o
	g++ -O3 AnimationPath.o -o AnimationPath



AnimationPathTest: AnimPairTest
	./AnimationPath res/cornell_box.obj res/testFolder/cornell_ -res 200

runNormalShadingTest: RayTracer
	./RayTracer res/bunny-scene.obj res/bunny-scene_normal1.png -res 1000 -calcNormal
	./RayTracer res/bunny-scene.obj res/bunny-scene_normal2.png -res 1000

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
	./RayTracer res/bunny-scene.obj res/bunny-scene.png -res 1000 -calcNormal
	./Blend res/bunny-scene.obj res/bunny-scene_blended.png res/bunny-scene_unblended.png -res 1000 -scale 3 -calcNormal
	./Binocular res/bunny-scene.obj res/bunny-scene_binocular.png -res 1000 -scale 3 -calcNormal

megaBunny: Blend
	./Blend res/bunny-scene.obj res/bunny-scene_mega.png res/bunny-scene_lessMega.png -res 100 -scale 10

runHappyTest: RayTracer res/happy-scene.obj res/happy-scene.mtl
	./RayTracer res/happy-scene.obj res/happy-scene.png -res 1000

runHappyTestBlend: Blend res/happy-scene.obj res/happy-scene.mtl
	./Blend res/happy-scene.obj res/happy-scene_blended.png res/happy-scene_unblended.png -res 1000 -scale 3

runHappyTestBinocular: Binocular res/happy-scene.obj res/happy-scene.mtl
	./Binocular res/happy-scene.obj res/happy-scene_binocular.png -res 1000 -scale 3 -calcNormal

bunnyIcon: RayTracer
	./RayTracer res/bunny-scene.obj res/bunny_icon.png -res 32

megaResCornell: Blend
	./Blend res/cornell_box.obj res/cornell_box_hd.png -res 100 -scale 10

megaResSpheres: Blend
	./Blend res/spheres.obj res/spheres_hd.png -res 100 -scale 10

HappyTime: RayTracer
	time ./RayTracer res/happy-scene.obj res/happy-time.png -res 1000 -calcNormal

all: RayTracer Binocular Blend

clean: 
	rm -f core *.o  *Test RayTracer Binocular Blend Animation AnimationPath a.out
