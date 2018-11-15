#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include "vec3.hpp"

struct Model {
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<float> tex_coords;
	std::vector<Vec3> colours;
	std::vector<int> indices;

	void clear() {
		vertices.clear();
		normals.clear();
		tex_coords.clear();
		colours.clear();
		indices.clear();
	}
};

#endif