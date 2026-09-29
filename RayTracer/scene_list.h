#pragma once
#ifndef SCENE_LIST
#define SCENE_LIST

#include "scene.h"
#include <stdexcept>
#include "utility.h"

class Scene0 : public Scene {
public:
	Scene0() {
		background = color(0.2, 0.2, 0.2);
		light1 = light(color(1, 1, 1), vec3(0, 100, 0));
		ambientColor = color(0.0, 0.0, 0.0);

		// World

		material_center = make_shared<phong>(color(1.0, 0.0, 1.0), color(1.0, 1.0, 1.0), .2, .7, .1, 16, light1, ambientColor);
		material_light = make_shared<diffuse_light>(light1.lightColor);

		//world.add(make_shared<sphere>(point3(0.0, -100.4, -1.0), 100.0, material_ground));
		world.add(make_shared<sphere>(point3(0.0, 0.0, 0.0), 0.4, material_center));
		//world.add(make_shared<sphere>(point3(-0.8, 0.0, -1.0), 0.4, material_left));
		//world.add(make_shared<sphere>(point3(0.8, 0.0, -1.0), 0.4, material_right));
		world.add(make_shared<sphere>(light1.direction, 3, material_light));
	}
};

class Scene1 : public Scene {
public:
	Scene1() {
		background = color(0.2, 0.2, 0.2);
		light1 = light(color(1, 1, 1), vec3(100, 100, 100));
		ambientColor = color(0.1, 0.1, 0.1);

		material_red = make_shared<phong>(color(1.0, 0.0, 0.0), color(1.0, 1.0, 1.0), .3, .6, .1, 32, light1, ambientColor);
		material_white = make_shared<phong>(color(1.0, 1.0, 1.0), color(1.0, 1.0, 1.0), .1, .8, .3, 4, light1, ambientColor);
		material_green = make_shared<phong>(color(0.0, 1.0, 0.0), color(0.5, 1.0, 0.5), .2, .7, .1, 64, light1, ambientColor);
		material_blue = make_shared<phong>(color(0.0, 0.0, 1.0), color(1.0, 1.0, 1.0), .0, .9, .1, 16, light1, ambientColor);

		material_light = make_shared<diffuse_light>(light1.lightColor);

		// World

		world.add(make_shared<sphere>(point3(0.45, 0.0, -0.15), 0.15, material_white));
		world.add(make_shared<sphere>(point3(0.0, 0.0, -0.1), 0.2, material_red));
		world.add(make_shared<sphere>(point3(-0.6, 0.0, 0.0), 0.3, material_green));
		world.add(make_shared<sphere>(point3(0.0, -10000.5, 0.0), 10000, material_blue));
		world.add(make_shared<sphere>(light1.direction, 3, material_light));
	}
};

class Scene2 : public Scene {
public:
	Scene2() {
		background = color(0.2, 0.2, 0.2);
		light1 = light(color(1, 1, 1), vec3(0.0, 2, 0.0));
		ambientColor = color(0.1, 0.1, 0.1);

		//World
		material_light = make_shared<diffuse_light>(light1.lightColor); //light
		material_blueTriangle = make_shared<phong>(color(0, 0, 1), color(1, 1, 1), 1.0, 0.9, 0.1, 4.0, light1, ambientColor); //blue triangle
		material_yellowTriangle = make_shared<phong>(color(1, 1, 0), color(1, 1, 1), 1.0, 0.9, 0.1, 4.0, light1, ambientColor); //blue triangle
		material_mirror = make_shared<metal>(color(1, 1, 1), 0.1, ambientColor); //shiny metal
		material_x = make_shared<metal>(color(1, 0, 0), 0.5, ambientColor);

		//triangle1
		polygon1.push_back(vec3(0.0, -0.7, -0.5));
		polygon1.push_back(vec3(1.0, 0.4, -1.0));
		polygon1.push_back(vec3(0.0, -0.7, -1.5));

		/*polygon1.push_back(vec3(-3, -3, 7));
		polygon1.push_back(vec3(3, -4, 3));
		polygon1.push_back(vec3(4, -5, 4));*/

		//triangle2
		polygon2.push_back(vec3(0.0, -0.7, -0.5));
		polygon2.push_back(vec3(0.0, -0.7, -1.5));
		polygon2.push_back(vec3(-1.0, 0.4, -1.0));

		world.add(make_shared<polygon>(polygon1, material_x)); //blue triangle
		world.add(make_shared<polygon>(polygon2, material_yellowTriangle)); //yellow triangle

		world.add(make_shared<sphere>(point3(0.0, 0.3, -1.0), .25, material_mirror)); //mirror sphere

		world.add(make_shared<sphere>(light1.direction, .1, material_light)); //light
	}
};

class Scene3 : public Scene {
public:
	Scene3() {
		background = color(0.2, 0.2, 0.2);
		light1 = light(color(2, 2, 2), vec3(3, 10, 4));
		ambientColor = color(0.1, 0.1, 0.1);

		// World

		material_center = make_shared<phong>(color(1.0, 0.0, 1.0), color(1.0, 1.0, 1.0), .3, 1, .2, 4, light1, ambientColor); //pink
		//auto material_pink = make_shared<lambertian>(color(1.0, 0.0, 1.0)); //pink lambertian
		material_light = make_shared<diffuse_light>(light1.lightColor); //light
		auto material_one = make_shared<metal>(color(1, 1, 1), .3, ambientColor); //dull metal
		auto material_two = make_shared<phong>(color(0.0, 0.0, 1.0), color(1.0, 1.0, 1.0), .6, .8, .1, 30, light1, ambientColor); //blue
		auto material_three = make_shared<metal>(color(1, 0, 0), 0.0, ambientColor); //shiny metal
		auto material_ground = make_shared<phong>(color(1, 1, 0.0), color(1.0, 1.0, 1.0), .3, 1, .2, 8, light1, ambientColor); //ground
		auto material_glass = make_shared<dielectric>(1.5); //glass

		world.add(make_shared<sphere>(point3(0.0, -100.7, -1.0), 100.0, material_ground)); //ground
		world.add(make_shared<sphere>(point3(0.0, 0.8, -7.0), 2, material_center)); //pink
		world.add(make_shared<sphere>(point3(-0.8, 0.15, -0.8), 0.6, material_one)); //dull metal
		world.add(make_shared<sphere>(point3(0.8, 0.2, -1.0), 0.2, material_two)); //blue
		world.add(make_shared<sphere>(point3(0.75, 0.3, -3.0), 0.6, material_three)); //shiny metal
		world.add(make_shared<sphere>(point3(0.0, 0.0, -2), .5, material_glass)); //glass

		world.add(make_shared<sphere>(light1.direction, 4, material_light));
	}
};

class SceneList {
private:
	vector<std::unique_ptr<Scene>> scenes;

public:
	SceneList() {
		scenes.push_back(make_unique<Scene0>());
		scenes.push_back(make_unique<Scene1>());
		scenes.push_back(make_unique<Scene2>());
		scenes.push_back(make_unique<Scene3>());
	}

	Scene& operator()(size_t index) {
		if (index >= scenes.size()) {
			throw std::out_of_range("Scene index out of bounds!");
		}
		return *scenes[index];
	}

	size_t size() const { return scenes.size(); }
};
#endif