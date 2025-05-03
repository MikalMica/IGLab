#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	DirLight* dirLight = new DirLight(0);
	dirLight->setAmb({ .25, .25, .25 });
	dirLight->setDiff({ .6, .6, .6 });
	dirLight->setSpec({ 0, 0.2, 0 });
	dirLight->setEnabled(true);

	gLights.push_back(dirLight);
}

Scene::~Scene()
{
	destroy();
	resetGL();
}

void
Scene::destroy()
{ // release memory and resources

	for (Abs_Entity* el : gObjects)
		delete el;

	gObjects.clear();

	for (Light* lg : gLights)
		delete lg;

	gLights.clear();
}

void
Scene::load()
{

	for (Abs_Entity* obj : gObjects)
		obj->load();
}

void
Scene::unload()
{
	for (Abs_Entity* obj : gObjects)
		obj->unload();

	Shader* s = Shader::get("light");
	s->use();

	for (auto light : gLights) {
		light->unload(*s);
	}
}

void
Scene::setGL()
{
	// OpenGL basic setting
	glClearColor(0.6, 0.7, 0.8, 1.0); // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);    // enable Depth test
}
void
Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);     // disable Depth test
}

void
Scene::uploadLights(Camera const& cam) const {
	Shader* s = Shader::get("light");
	s->use();

	for (auto light : gLights) {
		light->upload(*s, cam.viewMat());
	}

	for (auto el : gObjects) {
		el->uploadLights(cam.viewMat());
	}
}

void
Scene::render(Camera const& cam) const
{
	cam.upload();
	uploadLights(cam);

	for (Abs_Entity* el : gObjects)
		el->render(cam.viewMat());
}

void
Scene::update()
{
	for (Abs_Entity* el : gObjects)
		el->update();
}

void
Scene::orbit()
{
	mNode->setModelMat(glm::rotate(mNode->modelMat(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
}

void
Scene::rotate() {
	mNode->setModelMat(glm::rotate(mNode->modelMat(), glm::radians(-5.0f), glm::vec3(0, 1, 0)));
}

void 
Scene::switchLights() {
	lightsOn = !lightsOn;

	for (auto light : gLights) {
		light->setEnabled(lightsOn);
	}
}

void
Scene::switchElementsLights() {
	for (auto el : gObjects) {
		el->switchLights();
	}
}