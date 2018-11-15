#include <cstdlib>
#include <iostream>
#include <cfloat>
#include <cstdlib>

#define _USE_MATH_DEFINES
#include <cmath>

#include "perlin.hpp"
#include "vec2.hpp"
#include "grid.hpp"

#define NUM_VALUES 256
Vec2 random_vectors[NUM_VALUES];

const Vec2& getVector(int x, int y, int octave) {
	const unsigned int index = ((x * 11 + y) * 31 + octave) * 53;
	return random_vectors[index % NUM_VALUES];
}

float samplePerlinNoise(int x, int y, int o, int size) {
	const int cur_size = std::pow(2, o);
	const int div = size / cur_size;
	const float cur_amplitude = 1.0 / cur_size;

	int xx = x / div;
	int yy = y / div;

	float xt = (float)x / div - xx;
	float yt = (float)y / div - yy;

	float x_weight = (3 - 2 * xt) * xt * xt;
	float y_weight = (3 - 2 * yt) * yt * yt;

	float weights[4];
	for(int i = 0; i < 4; i++) {
		const Vec2& v = getVector((xx + i % 2) % cur_size, (yy + i / 2) % cur_size, o);
		weights[i] = dotProduct(v, Vec2(xt - i%2, yt - i/2));
	}

	float top = weights[0] + x_weight * (weights[1] - weights[0]);
	float bottom = weights[2] + x_weight * (weights[3] - weights[2]);
	return (top + y_weight * (bottom - top)) * cur_amplitude;
}

Grid generatePerlinNoise(int size, float amplitude, int start_octave, int max_octave) {
	Grid noise(size, size);

	for(int i = 0; i < NUM_VALUES; i++) {
		float angle = 2.0 * M_PI * rand() / RAND_MAX;
		random_vectors[i] = Vec2(std::cos(angle), std::sin(angle));
	}

	float min = FLT_MAX;
	float max = -FLT_MAX;

	float total_amplitude = 0.0;
	int cur_size = std::pow(2, start_octave);
	for(int o = start_octave; o <= max_octave; o++) {
		if (cur_size > size) {
			break;
		}
		total_amplitude += 1.0 / cur_size;
		cur_size *= 2;
	}

	if (total_amplitude == 0) {
		total_amplitude = 1.0;
	}

	for(int x = 0; x < size; x++) {
		for(int y = 0; y < size; y++) {
			float sum = 0;
			int cur_size = std::pow(2, start_octave);
			for(int o = start_octave; o <= max_octave; o++) {
				if (cur_size > size) {
					break;
				}
				sum += samplePerlinNoise(x, y, o, size);
				cur_size *= 2;
			}

			float value = sum * amplitude / total_amplitude;
			min = (value < min) ? value : min;
			max = (value > max) ? value : max;
			noise.set(x, y, value);
		}
	}

	float max_amplitude = (max > -min) ? max : (-min);
	noise *= amplitude / max_amplitude;

	return noise;
}