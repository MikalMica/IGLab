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
	Texture* a_extraText = new Texture();
	Texture* a_starText = new Texture();
	Texture* a_glassText = new Texture();
	//a_text->load("../assets/images/baldosaC.png");
	a_text->load("../assets/images/container.jpg");
	a_extraText->load("../assets/images/papelE.png");
	a_starText->load("../assets/images/baldosaP.png");
	a_glassText->load("../assets/images/windowV.jpg", 100U);
	//Ground* a_ground = new Ground(50, 50, 1, 1);
	//a_ground->setTexture(a_text);
	BoxOutline* a_box = new BoxOutline(100);
	a_box->setTexture(a_text);
	a_box->setExtraTexture(a_extraText);

	Star3D* a_star = new Star3D(100, 8, 100);
	a_star->setTexture(a_starText);

	GlassParapet* a_glass = new GlassParapet(100);
	a_glass->setTexture(a_glassText);


	//gObjects.push_back(a_box);
	gObjects.push_back(new RGBAxes(100));
	gObjects.push_back(a_glass);
	gObjects.push_back(new Photo(100));
	//gObjects.push_back(a_ground);
}