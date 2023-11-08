#include "utility.h"
#include "camera.h"
#include "color.h"
#include "hittableList.h"
#include "sphere.h"
#include "material.h"
#include "light.h"
#include "polygon.h"

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
color ray_color(const ray& r, const color& background, const hittable& world, int depth, const light& light, int max_depth) {
   hit_record rec;

   // If we've exceeded the ray bounce limit, no more light is gathered.
   if (depth <= 0)
      return color(0, 0, 0);

   // If the ray hits nothing, return the background color.
   if (!world.hit(r, 0.001, infinity, rec)) {
      /*if (depth == max_depth)
         return background;
      return color(1,1,1);*/ //This more acurately created the reference images but is less accurate (also turn off gamma correction)
      return background;
   }

   ray scattered;
   color attenuation;
   color emitted = rec.mat_ptr->emitted();
   

   if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
      return emitted;

   return emitted + (attenuation * (ray_color(scattered, background, world, depth - 1, light, max_depth)));
}

void tracer(int start, int end) {
   int scene = 3;

   // Image
   const auto aspect_ratio = 1.0 / 1.0;
   const int image_width = 512;
   const int image_height = static_cast<int>(image_width / aspect_ratio);
   const int samples_per_pixel = 256;
   const int max_depth = 5;

   // World

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

   switch (scene) {
   case 0:
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

      break;

   case 1:
      
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

      break;
   case 2:
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

      break;
   case 3:
      background = color(0.2, 0.2, 0.2);
      light1 = light(color(2, 2, 2), vec3(3, 10, 4));
      ambientColor = color(0.1, 0.1, 0.1);

      // World

      material_center = make_shared<phong>(color(1.0, 0.0, 1.0), color(1.0, 1.0, 1.0), .3, 1, .2, 4, light1, ambientColor); //pink
      auto material_pink = make_shared<lambertian>(color(1.0, 0.0, 1.0)); //pink lambertian
      material_light = make_shared<diffuse_light>(light1.lightColor); //light
      auto material_one = make_shared<metal>(color(1, 1, 1), .3, ambientColor); //dull metal
      auto material_two = make_shared<phong>(color(0.0, 0.0, 1.0), color(1.0, 1.0, 1.0), .6, .8, .1, 30, light1, ambientColor); //blue
      auto material_three = make_shared<metal>(color(1,0,0), 0.0, ambientColor); //shiny metal
      auto material_ground = make_shared<phong>(color(1, 1, 0.0), color(1.0, 1.0, 1.0), .3, 1, .2, 8, light1, ambientColor); //ground
      auto material_glass = make_shared<dielectric>(1.5); //glass

      world.add(make_shared<sphere>(point3(0.0, -100.7, -1.0), 100.0, material_ground)); //ground
      world.add(make_shared<sphere>(point3(0.0, 0.8, -7.0), 2, material_center)); //pink
      world.add(make_shared<sphere>(point3(-0.8, 0.15, -0.8), 0.6, material_one)); //dull metal
      world.add(make_shared<sphere>(point3(0.8, 0.2, -1.0), 0.2, material_two)); //blue
      world.add(make_shared<sphere>(point3(0.75, 0.3, -3.0), 0.6, material_three)); //shiny metal
      world.add(make_shared<sphere>(point3(0.0, 0.0, -2), .5, material_glass)); //glass

      world.add(make_shared<sphere>(light1.direction, 4, material_light));

      break;

   }

   // Camera
   camera cam(90.0, aspect_ratio, point3(0, 0, 1));

   // Render

   std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

   /*ray r = ray(vec3(0, 0, 0), vec3(20, -40, 40));
   ray_color(r, background, world, max_depth, light1, max_depth);
   return 0;*/
/*
   for (int j = image_height - 1; j >= 0; --j) {
      std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
      for (int i = 0; i < image_width; ++i) {
         color pixel_color(0, 0, 0);
         for (int s = 0; s < samples_per_pixel; ++s) {
            auto u = (i + random_double()) / (image_width - 1);
            auto v = (j + random_double()) / (image_height - 1);
            ray r = cam.get_ray(u, v);
            pixel_color += ray_color(r, background, world, max_depth, light1, max_depth);
         }
         write_color(std::cout, pixel_color, samples_per_pixel);
      }
   }*/

   int maxFrames = 96; //96
   for (int k = start; k < end; k++) {
      // Camera
      camera cam(90.0, aspect_ratio, point3(0, 0, 1.8 - k / (maxFrames/3.0)));

      //light1 = light(color(2, 2, 2), vec3(-2 + k / (maxFrames/4.0), 10, 4));
      light1 = light(color(2, 2, 2), vec3(3, 10, 4));
      material_light = make_shared<diffuse_light>(light1.lightColor); //light
      shared_ptr<hittable> newLight = make_shared<sphere>(light1.direction, 4, material_light);
      world.objects.at(world.objects.size() - 1).swap(newLight);

      std::ofstream myfile;
      std::string fileName = "image_" + std::to_string(k) + ".ppm";

      myfile.open(fileName);
      myfile << "P3\n" << image_width << " " << image_height << "\n255\n";

      for (int j = image_height - 1; j >= 0; --j) {
         std::cerr << "\rScanlines remaining : " << j << ' ' + std::to_string(k) << std::flush;
         for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
               auto u = (i + random_double()) / (image_width - 1);
               auto v = (j + random_double()) / (image_height - 1);
               ray r = cam.get_ray(u, v);
               pixel_color += ray_color(r, background, world, max_depth, light1, max_depth);
            }
            write_color(myfile, pixel_color, samples_per_pixel);
         }
      }

      myfile.close();

   }



   std::cerr << "\nDone.\n";
}


int main() {
   std::thread th1(tracer, 0, 8);
   std::thread th2(tracer, 8, 16);
   std::thread th3(tracer, 16, 24);
   std::thread th4(tracer, 24, 32);
   std::thread th5(tracer, 32, 40);
   std::thread th6(tracer, 40, 48);
   std::thread th7(tracer, 48, 56);
   std::thread th8(tracer, 56, 64);
   std::thread th9(tracer, 64, 72);
   std::thread th10(tracer, 72, 80);
   std::thread th11(tracer, 80, 88);
   std::thread th12(tracer, 88, 96);
   th1.join();
   th2.join();
   th3.join();
   th4.join();
   th5.join();
   th6.join();
   th7.join();
   th8.join();
   th9.join();
   th10.join();
   th11.join();
   th12.join();
   return 0;
}