#include <random>
#include "myrandom.h"

int randomInt(int min, int max) {
	std::random_device rd;
	static std::default_random_engine e{ rd() };
	std::uniform_int_distribution<int> d(min, max);
	return d(e);
}

float randomFloat(float min, float max) {
	std::random_device rd;
	static std::default_random_engine e{ rd() };
	std::uniform_real_distribution<float> d(min, max);
	return d(e);
}


float randomFloat() {
	std::random_device rd;
	static std::default_random_engine e{ rd() };
	std::uniform_real_distribution<float> d;
	return d(e);
}