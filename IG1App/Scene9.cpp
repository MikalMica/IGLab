#include "Scene9.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene9::init()
{
	Scene::init();
	PosLight* Poslight = new PosLight();
	Poslight->setAmb({ .25, .25, .25 });
	Poslight->setDiff({ 1, 1, 0});
	Poslight->setSpec({ 0, 0.2, 0 });
	Poslight->setEnabled(true);
	Poslight->setPosition({255, 255, 0});
	_light1 = Poslight;

	SpotLight* Spotlight = new SpotLight();
	Spotlight->setAmb({ .25, .25, .25 });
	Spotlight->setDiff({ .6, .6, .6 });
	Spotlight->setSpec({ 0, 0.2, 0 });
	Spotlight->setEnabled(true);
	Spotlight->setPosition({ 0, 150, 270 });
	Spotlight->setDirection({ -1, -1, -1 });
	_light2 = Spotlight;

	gLights.push_back(Poslight);
	gLights.push_back(Spotlight);
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



	SpotLight* Tielight = new SpotLight();
	Tielight->setAmb({ .25, .25, .25 });
	Tielight->setDiff({ .6, .6, .6 });
	Tielight->setSpec({ 0, 0.2, 0 });
	Tielight->setEnabled(true);
	Tielight->setCutoff(30., 50.);
	Tielight->setAttenuation(1., 0., 0.);
	Tielight->setPosition({0., 300., 0.});
	Tielight->setDirection({ 0, -1, 0 });

	Node->AddLight(Tielight);

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
	case 't':
		_light1->setEnabled(!_light1->enabled());
		break;
	case 'y':
		_light2->setEnabled(!_light2->enabled());
		break;
	}

	return true;
}