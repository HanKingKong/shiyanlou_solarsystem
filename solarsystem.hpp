#include <GL/glut.h>
#include "stars.hpp"

#define STARS_NUM 10


class SolarSystem {
public:
	SolarSystem();
	~SolarSystem();

	void onDisplay();
	void onUpdate();
	void onKeyboard(unsigned char key, int x, int y);

private:
	Star *stars[STARS_NUM];

	// 
	GLfloat viewX, viewY, viewZ;
	GLfloat centerX, centerY, centerZ;
	GLdouble upX, upY, upZ;
};

