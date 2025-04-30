#include "Scene2.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene2::init()
{
	Scene::init();
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	gObjects.push_back(new RGBAxes(400.0));

	gObjects.push_back(new RGBRectangle(200, 300));
}
