#include "Mesh.h"

using namespace std;
using namespace glm;

// Placeholder for the pending index of a GPU object
constexpr GLuint NONE = numeric_limits<GLuint>::max();

Mesh::Mesh()
 : mVAO(NONE)
 , mVBO(NONE)
 , mCBO(NONE)
{
}

Mesh::~Mesh()
{
	unload();
}

void
Mesh::draw() const
{
	glDrawArrays(
	  mPrimitive,
	  0,
	  size()); // primitive graphic, first index and number of elements to be rendered
}

Mesh* Mesh::generateRegularPolygon(GLuint num, GLdouble r) {
	
	Mesh* a_mesh = new Mesh();

	a_mesh->mPrimitive = GL_LINE_LOOP;

	a_mesh->mNumVertices = num;
	a_mesh->vVertices.reserve(a_mesh->mNumVertices);

	GLdouble a_alpha = glm::radians(90.0f);

	GLdouble x = r * cos(a_alpha);
	GLdouble y = r * sin(a_alpha);
	a_mesh->vVertices.push_back(glm::vec3(x, y, 0));

	GLdouble a_spinOffset = glm::radians(360.0f / num);

	for (GLuint i = 1; i < num; ++i) {
		a_alpha += a_spinOffset;
		GLdouble x = r * cos(a_alpha);
		GLdouble y = r * sin(a_alpha);
		a_mesh->vVertices.push_back(glm::vec3(x, y, 0));
	}

	return a_mesh;
}

Mesh* Mesh::generateRGBTriangle(GLdouble r) {

	Mesh* a_mesh = new Mesh();

	a_mesh->mPrimitive = GL_TRIANGLES;

	a_mesh->mNumVertices = 3;
	a_mesh->vVertices.reserve(3);

	GLdouble a_alpha = glm::radians(90.0f);

	// Creación de la malla

	GLdouble x = r * cos(glm::radians(90.0f));
	GLdouble y = r * sin(glm::radians(90.0f));
	a_mesh->vVertices.push_back(glm::vec3(x, y, 0));

	x = r * cos(glm::radians(210.0f));
	y = r * sin(glm::radians(210.0f));
	a_mesh->vVertices.push_back(glm::vec3(x, y, 0));

	x = r * cos(glm::radians(330.0f));
	y = r * sin(glm::radians(330.0f));
	a_mesh->vVertices.push_back(glm::vec3(x, y, 0));

	// Asignación de colores

	a_mesh->vColors.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
	a_mesh->vColors.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
	a_mesh->vColors.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);

	return a_mesh;
}

void
Mesh::load()
{
	assert(mVBO == NONE); // not already loaded

	if (vVertices.size() > 0) { // transfer data
		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);

		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, vVertices.size() * sizeof(vec3), vVertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
		glEnableVertexAttribArray(0);

		if (vColors.size() > 0) {             // upload colors
			glGenBuffers(1, &mCBO);

			glBindBuffer(GL_ARRAY_BUFFER, mCBO);
			glBufferData(GL_ARRAY_BUFFER, vColors.size() * sizeof(vec4), vColors.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), nullptr);
			glEnableVertexAttribArray(1);
		}
	}
}

void
Mesh::unload()
{
	if (mVAO != NONE) {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		mVAO = NONE;
		mVBO = NONE;

		if (mCBO != NONE) {
			glDeleteBuffers(1, &mCBO);
			mCBO = NONE;
		}
	}
}

void
Mesh::render() const
{
	assert(mVAO != NONE);

	glBindVertexArray(mVAO);
	draw();
}

Mesh*
Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh*
Mesh::generateRectangle(GLdouble w, GLdouble h) {

	Mesh* a_mesh = new Mesh;

	a_mesh->mPrimitive = GL_TRIANGLE_STRIP;

	a_mesh->mNumVertices = 4;
	a_mesh->vVertices.reserve(a_mesh->mNumVertices);

	a_mesh->vVertices.push_back(vec3(-w / 2, h / 2, 0));
	a_mesh->vVertices.push_back(vec3(-w / 2, -h / 2, 0));
	a_mesh->vVertices.push_back(vec3(w / 2, h / 2, 0));
	a_mesh->vVertices.push_back(vec3(w / 2, -h / 2, 0));

	return a_mesh;
}

Mesh*
Mesh::generateRGBRectangle(GLdouble w, GLdouble h) {
	Mesh* a_mesh = generateRectangle(w, h);

	a_mesh->vColors.reserve(a_mesh->mNumVertices);

	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));






	return a_mesh;
}