#ifndef GRID_HPP
#define GRID_HPP

#include <string>

class Grid {
public:
	Grid(int width, int height, float default_value = 0);
	Grid(const Grid& other);
	Grid(const float* data, int width, int height);
	~Grid();
	void clear(float value = 0);
	void set(int x, int y, float value);
	float get(int x, int y) const;
	float* getData();
	const float* getData() const;
	int getWidth() const;
	int getHeight() const;
	void resize(int width, int height, float default_value = 0);
	Grid& operator*=(float scalar);
	Grid& operator/=(float scalar);
	float min();
	float max();

private:
	float* data;
	int width;
	int height;

	static void assert(bool condition, std::string message);
	static void assertAllocation(bool condition, int width, int height);
	void assertBounds(int x, int y) const;
};

#endif