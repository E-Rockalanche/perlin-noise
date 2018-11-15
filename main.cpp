#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#include "perlin.hpp"
#include "grid.hpp"
#include "model.hpp"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

int size = 256;
int start_octave = 2;
int max_octave = 5;
float amplitude = size/8.0;
float water_level = 0.0;

#define clamp(value, low, high) (((value) < (low)) ? (low) : (((value) > (high)) ? (high) : (value)))

Model terrain;

//state
GLenum shade_mode = GL_SMOOTH;
GLenum fill_mode = GL_FILL;
bool show_normals = false;

// Light colours
GLfloat gsLightSpecularColour[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gsLightDiffuseColour[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gsLightAmbientColour[] = {0.1, 0.1, 0.1, 1.0};
GLfloat gsLightPosition[] = {0.0, 1000.0, 0.0, 1.0};

void createModel() {
	terrain.clear();
	terrain.vertices.resize(size * size, Vec3(0, 0, 0));
	terrain.normals.resize(size * size, Vec3(0, 0, 0));
	terrain.colours.resize(size * size, Vec3(0, 0, 0));

	Grid noise = generatePerlinNoise(size, amplitude, start_octave, max_octave);
	for(int y = 0; y < size; y++) {
		for(int x = 0; x < size; x++) {
			float height = noise.get(x, y);

			float xx = (float)x - size/2.0;
			// float yy = (height > water_level) ? height : water_level;
			float zz = (float)y - size/2.0;
			terrain.vertices[x + y*size] = Vec3(xx, height, zz);

			float red = 0;
			float green = 0;
			float blue = 0;
			
			if (height < water_level) {
				float weight = (height + amplitude) / (water_level + amplitude);
				blue = weight;
			} else {
				float weight = (height - water_level) / (amplitude - water_level);
				green = 0.25 + 0.75 * weight;
			}

			terrain.colours[x + y*size] = Vec3(red, green, blue);
		}
	}


	for(int y = 0; y < size-1; y++) {
		for(int x = 0; x < size-1; x++) {
			int tl, tr, bl, br;
			tl = x + y*size;
			bl = x + (y + 1)*size;
			br = (x + 1) + (y + 1)*size;
			tr = (x + 1) + y*size;

			const Vec3& vtl = terrain.vertices[tl];
			const Vec3& vtr = terrain.vertices[tr];
			const Vec3& vbl = terrain.vertices[bl];
			const Vec3& vbr = terrain.vertices[br];

			// face 1
			terrain.indices.push_back(tl);
			terrain.indices.push_back(bl);
			terrain.indices.push_back(br);

			Vec3 normal1 = Vec3::crossProduct(vbl - vtl, vbr - vtl);
			terrain.normals[tl] += normal1;
			terrain.normals[bl] += normal1;
			terrain.normals[br] += normal1;

			// face 2
			terrain.indices.push_back(tl);
			terrain.indices.push_back(br);
			terrain.indices.push_back(tr);

			Vec3 normal2 = Vec3::crossProduct(vbr - vtl, vtr - vtl);
			terrain.normals[tl] += normal2;
			terrain.normals[br] += normal2;
			terrain.normals[tr] += normal2;
		}
	}

	for(int i = 0; i < (int)terrain.normals.size(); i++) {
		terrain.normals[i].normalize();
	}
}

void initializeScene()
{
	createModel();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, gsLightSpecularColour);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gsLightDiffuseColour);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gsLightAmbientColour);
	glLightfv(GL_LIGHT0, GL_POSITION, gsLightPosition);

	glShadeModel( shade_mode );

  	glEnable(GL_DEPTH_TEST);
  	glEnable(GL_NORMALIZE);
}

void keyboard(unsigned char key, int x, int y)  {
	switch(key) {
		case 'r':
			createModel();
			break;

		case 's':
			shade_mode = (shade_mode == GL_SMOOTH) ? GL_FLAT : GL_SMOOTH;
			break;

		case 'f':
			fill_mode = (fill_mode == GL_FILL) ? GL_LINE : GL_FILL;
			break;

		case 'n':
			show_normals = !show_normals;
			break;
	}
}

void renderScene(void)  {
	static float sAccum = 0.0f;
	sAccum += 0.01f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,   // FOV y
					1.0f, 	// Aspect ratio (width/height)
					0.1f, 	// Near distance
					1000.0f);// Far distance

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt( 0.0f, size, -size,    // Eye position
				0.0f, 0.0f, 0.0f, 	  // Lookat position
  				0.0f, 1.0f, 0.0f );	  // Up vector

	glPolygonMode( GL_FRONT_AND_BACK, fill_mode );

	// glEnable(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	{
		glRotatef( sAccum, 0.0f, 1.0f, 0.0f );
		
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, terrain.normals.data());

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, terrain.vertices.data());

		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(	3, GL_FLOAT, 0, terrain.colours.data());

		glDrawElements(GL_TRIANGLES, terrain.indices.size(), GL_UNSIGNED_INT, terrain.indices.data());

		if (show_normals) {
			glDisable(GL_LIGHTING);
			for(int i = 0; i < (int)terrain.vertices.size(); i++) {
				const Vec3& v1 = terrain.vertices[i];
				const Vec3 v2 = terrain.normals[i] + v1;
				glLineWidth(1);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(v1.x, v1.y, v1.z);
				glVertex3f(v2.x, v2.y, v2.z);
				glEnd();
			}
		}
	}
	glPopMatrix();
	glutSwapBuffers();

	GLenum err;
	while((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "openGL error: " << err << '\n';
	}
}

int main(int argc, char* argv[]) {
	std::srand(std::time(NULL));

	for(int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "-size" && ++i < argc) {
			size = std::stoi(argv[i]);
			amplitude = size/8.0;
		} else if (arg == "-amplitude" && ++i < argc) {
			amplitude = std::stof(argv[i]);
		} else if (arg == "-water_level" && ++i < argc) {
			water_level = std::stof(argv[i]);
		} else if (arg == "-start_octave" && ++i < argc) {
			start_octave = std::stoi(argv[i]);
		} else if (arg == "-max_octave" && ++i < argc) {
			max_octave = std::stoi(argv[i]);
		} else {
			std::cout << "unknown flag " << arg << '\n';
			return 1;
		}
	}

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(100,100);
	glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	glutCreateWindow("Perlin Noise");

	glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutKeyboardFunc(keyboard);

	initializeScene();

	//  GLUT main loop
	glutMainLoop();

	return(0);
}