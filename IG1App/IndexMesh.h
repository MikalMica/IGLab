#pragma once
#include "Mesh.h"
#include <numbers>

class IndexMesh : public Mesh
{
	std::vector<GLuint> vIndexes;
	GLuint mIBO;

protected:
	void draw() const override;

public:
	static IndexMesh* generateByRevolution(
		const std::vector<glm::vec2>& profile, GLuint nSamples,
		GLfloat angleMax = 2 * std::numbers::pi);

	void load() override;
	void unload() override;
};

