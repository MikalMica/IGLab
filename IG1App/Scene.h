#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"
#include "Light.h"

#include <vector>

class Scene
{
public:
	Scene() = default;
	~Scene();

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	virtual void init();

	void render(Camera const& cam) const;
	void uploadLights(Camera const& cam) const;
	void switchLights();
	virtual void update();
	void switchElementsLights();

	// load or unload scene data into the GPU
	void load();
	void unload();

	void rotate();
	void orbit();

	virtual bool handleKey(glm::uint key) { return false; }

protected:
	void destroy();
	void setGL();
	void resetGL();

	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
	std::vector<Light*> gLights;
	Abs_Entity* mNode = nullptr; // Ghost node of the scene
	bool lightsOn = true;
};

#endif //_H_Scene_H_
