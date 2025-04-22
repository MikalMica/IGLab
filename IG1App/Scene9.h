#pragma once
#include "Scene.h"

class Scene9 : public Scene
{
public:
	void init() override;
	bool handleKey(glm::uint key) override;
};