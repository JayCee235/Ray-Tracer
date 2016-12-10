#ifndef __MATERIAL
#define __MATERIAL

#include <string>

#include "vector/GenVector.h"
#include "objload/obj_parser.h"

class Material
{
public:
	std::string name;
	Vector3 ambient, diffuse, specular;
	float reflect, refract, trans, shiny, glossy, refractIndex;
	Material() {
		this->name = "Default material";
		this->ambient = Vector3(1, 0, 1);
		this->diffuse = Vector3(0, 0, 0);
		this->specular = Vector3(0, 0, 0);
		this->reflect = 0;
		this->refract = 0;
		this->trans = 0;
		this->shiny = 0;
		this->glossy = 0;
		this->refractIndex = 0;
	};
	Material(char* name, Vector3 ambient, Vector3 diffuse, Vector3 specular, 
		float reflect, float refract, float trans, float shiny, float glossy, float refractIndex) {
		this->name = std::string(name);
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->reflect = reflect;
		this->refract = refract;
		this->trans = trans;
		this->shiny = shiny;
		this->glossy = glossy;
		this->refractIndex = refractIndex;
	}

	Material(obj_material om) {
		this->name = std::string(om.name);
		this->ambient = Vector3(om.amb[0], om.amb[1], om.amb[2]);
		this->diffuse = Vector3(om.diff[0], om.diff[1], om.diff[2]);
		this->specular = Vector3(om.spec[0], om.spec[1], om.spec[2]);
		this->reflect = om.reflect;
		this->refract = om.refract;
		this->trans = om.trans;
		this->shiny = om.shiny;
		this->glossy = om.glossy;
		this->refractIndex = om.refract_index;
	}
	~Material() {};

	void printInfo() {
		printf("\tMaterial Name: %s\tAmbient color: (%.2f, %.2f, %.2f)\n\tDiffuse color: (%.2f, %.2f, %.2f)\n\tSpecular color: (%.2f, %.2f, %.2f)\n",
			name.data(),
			ambient[0], ambient[1], ambient[2],
			diffuse[0], diffuse[1], diffuse[2],
			specular[0], specular[1], specular[2]);
		printf("\tReflection: %.2f\n\tRefraction: %.2f\n\tTransparency: %.2f\n\tShininess: %.2f\n\tGlossiness: %.2f\n\tIndex of Refraction: %.2f\n",
			reflect, refract, trans, shiny, glossy, refractIndex);
		printf("\n");
	}

	void printAbbrInfo() {
		printf("Material Name: %s", name.data());
	}
};

#endif