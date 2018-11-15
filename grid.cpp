#include <string>
#include <stdexcept>
#include "grid.hpp"

Grid::Grid(int width, int height, float default_value) {
	this->width = width;
	this->height = height;
	const int size = width * height;
	data = new float[size];
	assertAllocation(data, width, height);
	for(int i = 0; i < size; i++) {
		data[i] = default_value;
	}
}

Grid::Grid(const Grid& other) {
	width = other.width;
	height = other.height;
	const int size = width * height;
	data = new float[size];
	assertAllocation(data, width, height);
	for(int i = 0; i < size; i++) {
		data[i] = other.data[i];
	}
}

Grid::Grid(const float* data, int width, int height) {
	this->width = width;
	this->height = height;
	const int size = width * height;
	this->data = new float[size];
	assertAllocation(this->data, width, height);
	for(int i = 0; i < size; i++) {
		this->data[i] = data[i];
	}
}

Grid::~Grid() {
	delete[] data;
}

void Grid::clear(float value) {
	const int size = width * height;
	for(int i = 0; i < size; i++) {
		data[i] = value;
	}
}

void Grid::set(int x, int y, float value) {
	assertBounds(x, y);
	data[x + y*width] = value;
}

float Grid::get(int x, int y) const {
	assertBounds(x, y);
	return data[x + y*width];
}

float* Grid::getData() {
	return data;
}

const float* Grid::getData() const {
	return data;
}

int Grid::getWidth() const {
	return width;
}

int Grid::getHeight() const {
	return height;
}

void Grid::resize(int new_width, int new_height, float default_value) {
	const int new_size = new_width * new_height;
	float* new_data = new float[new_size];
	assertAllocation(new_data, new_width, new_height);
	for(int y = 0; y < new_height; y++) {
		const int new_offset = y*new_width;
		const int old_offset = y*width;
		for(int x = 0; x < new_width; x++) {
			if (x < width && y < height) {
				new_data[x + new_offset] = data[x + old_offset];
			} else {
				new_data[x + new_offset] = default_value;
			}
		}
	}
	delete[] data;
	data = new_data;
	this->width = width;
	this->height = height;
}

Grid& Grid::operator*=(float scalar) {
	const int size = width * height;
	for(int i = 0; i < size; i++) {
		data[i] *= scalar;
	}
	return *this;
}

Grid& Grid::operator/=(float scalar) {
	const int size = width * height;
	for(int i = 0; i < size; i++) {
		data[i] /= scalar;
	}
	return *this;
}

float Grid::min() {
	const int size = width * height;
	float min = data[0];
	for(int i = 1; i < size; i++) {
		if (data[i] < min) {
			min = data[i];
		}
	}
	return min;
}

float Grid::max() {
	const int size = width * height;
	float max = data[0];
	for(int i = 1; i < size; i++) {
		if (data[i] > max) {
			max = data[i];
		}
	}
	return max;
}

void Grid::assert(bool condition, std::string message) {
	if (!condition) {
		throw new std::runtime_error(message);
	}
}

void Grid::assertAllocation(bool condition, int width, int height) {
	if (!condition) {
		throw new std::runtime_error("failed to allocate grid of size "
			+ std::to_string(width) + "x" + std::to_string(height));
	}
}

void Grid::assertBounds(int x, int y) const {
	if (x < 0 || y < 0 || x >= width || y >= height) {
		throw new std::runtime_error("grid position (" + std::to_string(x)
			+ ", " + std::to_string(y) + ") out of bounds ("
			+ std::to_string(width) + ", " + std::to_string(height) + ")");
	}
}