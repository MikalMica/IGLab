#include "Scene7.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene7::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	gObjects.push_back(new IndexedBox(50));
}
