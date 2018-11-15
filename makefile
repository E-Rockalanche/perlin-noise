CXX = g++
CFLAGS = -Wall -Wextra -c -std=c++17
LFLAGS = -static -lm -lglut32cu -lglu32 -lopengl32
MAKE_OBJ = $(CXX) $< -o $@ $(CFLAGS)
MAKE_EXE = $(CXX) $^ -o $@ $(LFLAGS)
CLEAN := del .\obj\*.o .\obj\models\*.o $(TARGET)

noise.exe: main.o grid.o perlin.o vec2.o vec3.o
	$(MAKE_EXE)

main.o: main.cpp grid.hpp perlin.hpp
	$(MAKE_OBJ)

grid.o: grid.cpp grid.hpp
	$(MAKE_OBJ)

perlin.o: perlin.cpp perlin.hpp vec2.hpp grid.hpp
	$(MAKE_OBJ)

vec2.o: vec2.cpp vec2.hpp
	$(MAKE_OBJ)

vec3.o: vec3.cpp vec3.hpp
	$(MAKE_OBJ)

clean:
	$(CLEAN)