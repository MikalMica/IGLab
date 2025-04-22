#include "Scene9.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene9::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	CompoundEntity* Node = new CompoundEntity();

	AdvancedTIE* tie = new AdvancedTIE();
	tie->setModelMat(translate(tie->modelMat(), glm::vec3(0.0f, 300.0f, 0.0f)));

	Node->AddEntity(tie);
	gObjects.push_back(Node);

	Sphere* tatooine = new Sphere(250, 100, 100);
	tatooine->setColor(glm::vec4(1.0f, 233.0f / 255.0f, 0.0f, 1.0f));

	gObjects.push_back(tatooine);
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