/*
	A small library for reusing code across cs475 projects
*/
#include <stdlib.h>
#include <time.h>
#include <random>
#include <ctime>
#include <omp.h>
#include "lib_cs475.h"
 
float randFloat(){
	static std::mt19937 float_engine((unsigned)time(nullptr));
	static std::uniform_real_distribution<float> float_dist;
	return float_dist(float_engine);
}

bool openmp_supported(){
	#ifndef _OPENMP
		return false;
	#else
		return true;
	#endif
}