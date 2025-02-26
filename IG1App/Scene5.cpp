#include "Scene5.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene5::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	
	Texture* a_text = new Texture();
	a_text->load("../assets/images/baldosaC.png");
	Ground* a_ground = new Ground(50, 50, 1, 1);
	a_ground->setTexture(a_text);


	//gObjects.push_back(new BoxOutline(100));
	gObjects.push_back(new RGBAxes(100));
	gObjects.push_back(a_ground);
}