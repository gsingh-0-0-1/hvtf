#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP
#include <math.h>
#include <iostream>
using namespace std;

const int NPOINTS = 1000;

const int SN_PEAK_TIME = NPOINTS * 0.2;
const int SN_PEAK_VAL = 100;

int SUPERNOVA_LIGHT_CURVE[NPOINTS];

int supernova_light_curve(int start, int end, int val, int max){
	double factor = NPOINTS / (end - start);
	int index = val * factor;
	return SUPERNOVA_LIGHT_CURVE[index] * max / SN_PEAK_VAL;
}

void define_light_curves(){
	for (int i = 0; i < NPOINTS; i++){
		SUPERNOVA_LIGHT_CURVE[i] = (SN_PEAK_VAL * exp(1) / SN_PEAK_TIME) * i * exp(-i * 1.0 / SN_PEAK_TIME);
	}
}

#endif