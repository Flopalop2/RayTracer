#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.h"
#include "light.h"
//#include "hittable.h" 
struct hit_record;

class material {
public:
   virtual color emitted() const {
      return color(0, 0, 0);
   }

   virtual bool scatter(
      const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
   ) const = 0;
};

class lambertian : public material {
public:
   lambertian(const color& a) : albedo(a) {}

   virtual bool scatter(
      const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
   ) const override {
      auto scatter_direction = rec.normal + random_unit_vector();

      // Catch degenerate scatter direction
      if (scatter_direction.near_zero())
         scatter_direction = rec.normal;

      scattered = ray(rec.p, scatter_direction);
      attenuation = albedo;
      return true;
   }

public:
   color albedo;
};

class metal : public material {
public:
   metal(const color& a, double f, color ambient) : albedo(a), fuzz(f < 1 ? f : 1), ambient(ambient) {}

   virtual bool scatter(
      const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
   ) const override {
      vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
      scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
      attenuation = albedo + ambient;
      return (dot(scattered.direction(), rec.normal) > 0);
   }

public:
   color albedo;
   double fuzz;
   color ambient;
};

class dielectric : public material {
public:
   dielectric(double index_of_refraction) : ir(index_of_refraction) {}

   virtual bool scatter(
      const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
   ) const override {
      attenuation = color(1.0, 1.0, 1.0);
      double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

      vec3 unit_direction = unit_vector(r_in.direction());
      double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
      double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

      bool cannot_refract = refraction_ratio * sin_theta > 1.0;
      vec3 direction;

      if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
         direction = reflect(unit_direction, rec.normal);
      else
         direction = refract(unit_direction, rec.normal, refraction_ratio);

      scattered = ray(rec.p, direction);
      return true;
   }

public:
   double ir; // Index of Refraction

private:
   static double reflectance(double cosine, double ref_idx) {
      // Use Schlick's approximation for reflectance.
      auto r0 = (1 - ref_idx) / (1 + ref_idx);
      r0 = r0 * r0;
      return r0 + (1 - r0) * pow((1 - cosine), 5);
   }
};

class diffuse_light : public material {
public:
   diffuse_light(color c) : emit(c) {}

   virtual bool scatter(
      const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
   ) const override {
      return false;
   }

   virtual color emitted() const override {
      return emit;
   }

public:
   color emit;
};

class phong : public material {
public:
   phong(const color& od, const color& os, const double ks, const double kd, const double ka, const double kgls, light light1, color Ia) : od(od), os(os), ks(ks), kd(kd), ka(ka), kgls(kgls), light1(light1), Ia(Ia){}

   virtual bool scatter(
      const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
   ) const override {
      //auto scatter_direction = rec.normal + random_unit_vector(); //more accurate
      auto scatter_direction = rec.normal + random_in_unit_sphere(); //looks better

      // Catch degenerate scatter direction
      if (scatter_direction.near_zero())
         scatter_direction = rec.normal;

      scattered = ray(rec.p, scatter_direction);

      point3 light = light1.direction - rec.p;
      light = unit_vector(light);
      point3 view = -r_in.direction();
      view = unit_vector(view);
      point3 normal = rec.normal;
      normal = unit_vector(normal);
      vec3 R = (2 * normal * dot(light, normal)) - light;
      R = unit_vector(R);

      auto diffuse = kd * light1.lightColor * od * (dot(normal, light));
      diffuse = clip(diffuse, 0.0, 1.0);
      auto specular = ks * light1.lightColor * os * pow(dot(view, R), kgls);
      specular = clip(specular, 0.0, 1.0);
      auto ambient = Ia * light1.lightColor * ka;

      /*double visibleToLight = dot(normal, light);
      if (visibleToLight <= 0)
         attenuation = ambient;
      else*/
         attenuation = diffuse + specular + ambient;

      return true;

   }

   

public:
   color od;
   color os;
   double ks;
   double kd;
   double ka;
   double kgls;
   light light1;
   color Ia;

};

#endif