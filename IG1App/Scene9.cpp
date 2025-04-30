#include "Scene9.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene9::init()
{
	Scene::init();
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	Sphere* tatooine = new Sphere(glm::vec4(1.0f, 233.0f / 255.0f, 0.0f, 1.0f), 250, 100, 100);

	gObjects.push_back(tatooine);

	CompoundEntity* Node = new CompoundEntity();

	AdvancedTIE* tie = new AdvancedTIE();
	tie->setModelMat(translate(tie->modelMat(), glm::vec3(0.0f, 300.0f, 0.0f)));

	Node->AddEntity(tie);

	mNode = Node;
	gObjects.push_back(Node);
	Node = nullptr;
}

bool
Scene9::handleKey(glm::uint key) {

	switch (key) {
	case 'f':
		rotate();
		break;
	case 'g':
		orbit();
		break;
	}

	return true;
}