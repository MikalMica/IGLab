#include "Scene3.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene3::init()
{
	Scene::init();
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	gObjects.push_back(new RegularPolygon(100, 300));

	gObjects.push_back(new RGBTriangle(50, glm::vec3(300, 0, 0)));

	gObjects.push_back(new RGBRectangle(600, 300));
}
