#include "Scene1.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene1::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	gObjects.push_back(new RGBAxes(400.0));

	RegularPolygon* triangle = new RegularPolygon(3, 300);
	triangle->setColor(vec4(0, 1, 1, 1));
	gObjects.push_back(triangle);

	RegularPolygon* circle = new RegularPolygon(100, 300);
	circle->setColor(vec4(1, 0, 1, 1));
	gObjects.push_back(circle);
}