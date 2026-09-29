#include "utility.h"
#include "camera.h"
#include "color.h"
#include "hittableList.h"
#include "sphere.h"
#include "material.h"
#include "light.h"
#include "polygon.h"
#include "scene_list.h"

#include <iostream>
#include <fstream>
#include <string>
#include <thread>

static color ray_color(const ray& r, const color& background, const hittable& world, int depth, const light& light, int max_depth) {
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

static void tracer(int start, int end) {
   int sceneIndex = 3;

   // Image
   const auto aspect_ratio = 1.0 / 1.0;
   const int image_width = 128; // 512;
   const int image_height = static_cast<int>(image_width / aspect_ratio);
   const int samples_per_pixel = 32; // 256;
   const int max_depth = 5; // 10;

   // World

   SceneList sceneList;
   Scene& scene = sceneList(sceneIndex);

   // Camera
   //camera cam(90.0, aspect_ratio, point3(0, 0, 1));

   // Render

   //std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

   int maxFrames = 96; //96
	double startDistance = 1.0; // 1.8
	double travelDistance = 0; // 3.0
	double travelDistancePerFrame = travelDistance / (maxFrames - 1);

   for (int k = start; k < end; k++) {
      // Camera
		camera cam(90.0, aspect_ratio, point3(0, 0, startDistance - k * travelDistancePerFrame)); // this should not be making a new camera every frame, but it is for now. I should just update the camera's position instead of creating a new one each frame.

      //light1 = light(color(2, 2, 2), vec3(-2 + k / (maxFrames/4.0), 10, 4)); // animate light?
      scene.light1 = light(color(2, 2, 2), vec3(3, 10, 4));
      scene.material_light = make_shared<diffuse_light>(scene.light1.lightColor); //light
      shared_ptr<hittable> newLight = make_shared<sphere>(scene.light1.direction, 4, scene.material_light);
      scene.world.objects.at(scene.world.objects.size() - 1).swap(newLight);

      std::ofstream myfile;
      std::string fileName = "image_" + std::to_string(k) + ".ppm";

      myfile.open(fileName);
      myfile << "P3\n" << image_width << " " << image_height << "\n255\n";

      for (int j = image_height - 1; j >= 0; --j) {
         std::cerr << "\rScanlines remaining : " << j << ' ' + std::to_string(k) << std::flush;
         for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
               double u = (i + random_double()) / (image_width - 1);
               double v = (j + random_double()) / (image_height - 1);
               ray r = cam.get_ray(u, v);
               pixel_color += ray_color(r, scene.background, scene.world, max_depth, scene.light1, max_depth);
            }
            write_color(myfile, pixel_color, samples_per_pixel);
         }
      }

      myfile.close();

   }

   std::cerr << "\nDone.\n";
}


int main() {
	int numThreads = std::thread::hardware_concurrency(); // ability to set max or reserve 1 or 2 threads for other processes
	std::cout << "Number of threads: " << numThreads << std::endl;
   for (int i = 0; i< numThreads; i++) {
      std::thread th(tracer, i * (96 / numThreads), (i + 1) * (96 / numThreads));
      th.join();
	}
   return 0;
}