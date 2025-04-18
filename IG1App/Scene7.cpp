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

	Texture* a_text = new Texture();
	a_text->load("../assets/images/noche.jpg", 100);

	AdvancedTIEWing* Wing = new AdvancedTIEWing(50, 50, 50, 0, 0, 0);
	Wing->setTexture(a_text);

	gObjects.push_back(Wing);
}
