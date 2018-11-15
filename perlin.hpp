#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <climits>
#include "grid.hpp"

float samplePerlinNoise(int x, int y, int octave, int size);
Grid generatePerlinNoise(int size, float amplitude, int start_octave = 1, int max_octave = INT_MAX);

#endif