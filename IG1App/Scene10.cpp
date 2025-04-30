#include "Scene10.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene10::init()
{
	Scene::init();
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	Sphere* tatooine = new Sphere(glm::vec4(1.0f, 233.0f / 255.0f, 0.0f, 1.0f), 250, 100, 100);
	//tatooine->setColor(glm::vec4(1.0f, 233.0f / 255.0f, 0.0f, 1.0f));
	tatooine->setModelMat(glm::translate(tatooine->modelMat(), { 300, 0, 0 }));
	gObjects.push_back(tatooine);

	Sphere* tatooine2 = new Sphere(glm::vec4(1.0f, 233.0f / 255.0f, 0.0f, 1.0f), 250, 100, 100);
	//tatooine2->setColor(glm::vec4(1.0f, 233.0f / 255.0f, 0.0f, 1.0f));
	Material m;
	m.setAmb({ .24725, .1995, .0745 });
	m.setDiff({ .75164, .60648, .22648 });
	m.setSpec({ .628281, .555802, .366065 });
	m.setExp(51.2f);
	tatooine2->setMaterial(m);

	tatooine2->setModelMat(glm::translate(tatooine2->modelMat(), { -300, 0, 0 }));
	gObjects.push_back(tatooine2);
}