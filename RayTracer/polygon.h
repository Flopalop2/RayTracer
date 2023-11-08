#pragma once
#ifndef POLYGON_H
#define POLYGON_H

#include "hittable.h"
#include "utility.h"

class polygon : public hittable {
public:
   polygon() {}
   polygon(vector<point3> points, shared_ptr<material> m)
      : points(points), mat_ptr(m) {};

   virtual bool hit(
      const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
   vector<point3> points;
   shared_ptr<material> mat_ptr;
};

inline int findDomCoord(vec3 plane) {
   if (plane.e[0] < 0) plane.e[0] *= -1;
   if (plane.e[1] < 0) plane.e[1] *= -1;
   if (plane.e[2] < 0) plane.e[2] *= -1;

   int max = 0;
   double maxValue = -1;
   for (int i = 0; i < 2; i++) {
      if (plane.e[i] > maxValue)
         maxValue = plane.e[i];
         max = i;
   }

   return max;
}

bool polygon::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
   //get plane normal
   auto v1 = points.at(0) - points.at(1);
   auto v2 = points.at(2) - points.at(1);
   auto pn = cross(v1, v2); //normalize this?
   //pn = unit_vector(pn);

   int max = findDomCoord(pn);
   
   //planar intersection test
   auto d = ((pn.e[0] * points.at(0).e[0]) + (pn.e[1] * points.at(0).e[1]) + (pn.e[2] * points.at(0).e[2])); // is this right?
   //std::cerr << d << std::endl;
   //d = 0.005;
   auto vd = dot(pn,r.direction());
   if (vd >= 0) return false; //also if plane is one sided?
   if (vd == 0) return false;
   auto v0 = -(dot(pn, r.origin()) + d);
   auto t = v0 / vd;
   if (t < t_min || t > t_max) return false;
   if (vd > 0) pn = pn * -1; //reverse planes normal
   
   auto ri = vec3((r.origin().e[0] + r.direction().e[0] * t), (r.origin().e[1] + r.origin().e[1] * t), (r.origin().e[2] + r.direction().e[2] * t));

   //project points
   vector<vector<double>> projected;

   for (int i = 0; i < points.size(); i++) {
      vector<double> tempVec;
      for (int j = 0; j < 3; j++) 
         if (j != max) 
            tempVec.push_back(points.at(i).e[j]);
      
      projected.push_back(tempVec);
      //std::cout << tempVec.at(0) << "," << tempVec.at(1) << //std::endl;
   }
   //project r
   vector<double> rProj;
   for (int j = 0; j < 3; j++)
      if (j != max)
         rProj.push_back(ri.e[j]);
   //std::cout << rProj.at(0) << "," << rProj.at(1) << //std::endl;

   //translate by r
   vector<vector<double>> translated;
   for (int i = 0; i < projected.size(); i++) {
      vector<double> toTranslate;
      toTranslate.push_back(projected.at(i).at(0) - rProj.at(0));
      toTranslate.push_back(projected.at(i).at(1) - rProj.at(1));
      translated.push_back(toTranslate);
      //std::cout << toTranslate.at(0) << "," << toTranslate.at(1) << //std::endl;
   }

   //translate r
   vector<double> rTrans;
   rTrans.push_back(0);
   rTrans.push_back(0);
   //std::cout << rTrans.at(0) << "," << rTrans.at(1) << //std::endl;

   int numCrossings = 0;
   auto v0_prime = translated.at(0).at(1);
   //std::cout << v0_prime << //std::endl;
   int signHolder = 0;
   if (v0_prime < 0)
      signHolder = -1;
   else
      signHolder = 1;

   //std::cout << signHolder << //std::endl;

   int nextSignHolder = 0;

   for (int i = 0; i < translated.size(); i++) {
      int iPlusOne = i + 1;
      if (i == translated.size() - 1)
         iPlusOne = 0;

      if (translated.at(iPlusOne).at(1) < 0)
         nextSignHolder = -1;
      else
         nextSignHolder = 1;

      double uCross = 0;
      if (signHolder != nextSignHolder) {
         if ((translated.at(i).at(0) > 0) && (translated.at(iPlusOne).at(0) > 0))
            numCrossings++;
         else if ((translated.at(i).at(0) > 0) || (translated.at(iPlusOne).at(0) > 0)) {
            uCross = translated.at(i).at(0) - translated.at(i).at(1) * (translated.at(iPlusOne).at(0) - translated.at(i).at(0)) / (translated.at(iPlusOne).at(1) - translated.at(i).at(1));
            if (uCross > 0)
               numCrossings++;
         }
      }
      //std::cout << i << " , " << signHolder << " , " << nextSignHolder << " , " << numCrossings << " , " << uCross << //std::endl;
      signHolder = nextSignHolder;

   }

   if (numCrossings % 2 == 0)
      return false;
   //std::cout << "true" << std::endl;
   rec.t = t;
   rec.p = r.at(rec.t);
   vec3 outward_normal = pn; //normalize?
   //pn = unit_vector(pn);
   rec.set_face_normal(r, outward_normal);
   rec.mat_ptr = mat_ptr;

   return true;
}



#endif