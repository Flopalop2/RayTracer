#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"

class camera {
public:
   camera(
      double vfov, // vertical field-of-view in degrees
      double aspect_ratio,
      point3 origin
   ) {
      auto theta = degrees_to_radians(vfov);
      auto h = tan(theta / 2);
      auto viewport_width = 2.0 * 2;
      auto viewport_height = 1/aspect_ratio * viewport_width;
      auto focal_length = 4;

      this->origin = origin;
      horizontal = vec3(viewport_width, 0.0, 0.0);
      vertical = vec3(0.0, viewport_height, 0.0);
      lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
   }

   ray get_ray(double u, double v) const {
      return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
   }

private:
   point3 origin;
   point3 lower_left_corner;
   vec3 horizontal;
   vec3 vertical;
};
#endif