#pragma once
#include "Scene.h"

class Scene9 : public Scene
{
	PosLight* _light1;
	SpotLight* _light2;
public:
	void init() override;
	bool handleKey(glm::uint key) override;
};