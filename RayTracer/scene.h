#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "hittableList.h"
#include "color.h"
#include "material.h"
#include "sphere.h"
#include "light.h"
#include "polygon.h"
#include "utility.h"

class Scene {
public:
	Scene() {}
	hittable_list world;

	//auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	shared_ptr<phong> material_center;
	//auto material_left = make_shared<diffuse_light>(color(0, 0, 0));
	//auto material_left = make_shared<dielectric>(1.5);
	//auto material_left = make_shared<metal>(color(1.0, 0.0, 1.0), 0.0);
	//auto material_right = make_shared<metal>(color(0.0, 1.0, 0.0), 0.5);
	shared_ptr<diffuse_light> material_light;

	shared_ptr<phong> material_red;
	shared_ptr<phong> material_white;
	shared_ptr<phong> material_green;
	shared_ptr<phong> material_blue;
	shared_ptr<phong> material_blueTriangle;
	shared_ptr<phong> material_yellowTriangle;
	shared_ptr<metal> material_mirror;
	shared_ptr<metal> material_x;

	color background;
	light light1;
	color ambientColor;

	vector<vec3> polygon1;
	vector<vec3> polygon2;
};
#endif