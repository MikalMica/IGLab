#include "Scene5.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene5::init()
{
	Scene::init();
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene


	//Creamos el suelo con su textura
	Texture* a_text = new Texture();
	a_text->load("../assets/images/baldosaC.png");

	Ground* ground = new Ground(300, 300, 1, 1);
	ground->setTexture(a_text);

	gObjects.push_back(ground);
	ground == nullptr;

	//Creamos una caja en una esquina del suelo
	a_text = new Texture();
	a_text->load("../assets/images/container.jpg");

	BoxOutline* box = new BoxOutline(50, 100, 25, 100);
	box->setTexture(a_text);

	a_text = new Texture();
	a_text->load("../assets/images/papelE.png");

	box->setExtraTexture(a_text);
	gObjects.push_back(box);
	box = nullptr;

	//Creamos una estrella encima de la caja
	a_text = new Texture();
	a_text->load("../assets/images/baldosaP.png");

	Star3D* star = new Star3D(25, 8, 25, 100, 100, 100);
	star->setTexture(a_text);

	gObjects.push_back(star);
	star = nullptr;

	//Creamos la imagen de la pantalla
	gObjects.push_back(new Photo(50, -5));

	//Creamos la cristalera
	a_text = new Texture();
	a_text->load("../assets/images/windowV.jpg", 100);

	GlassParapet* glass = new GlassParapet(300);
	glass->setTexture(a_text);

	gObjects.push_back(glass);
	glass = nullptr;
	
}