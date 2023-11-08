#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "utility.h"

class light {
public:
	light() : lightColor(color(0,0,0)), direction(vec3(0,0,0)) {}
	light(color lightColor, vec3 direction) : lightColor(lightColor), direction(direction) {}


public:
	color lightColor;
	vec3 direction;


};

#endif 
