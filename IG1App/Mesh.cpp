#include "Mesh.h"

using namespace std;
using namespace glm;

// Placeholder for the pending index of a GPU object
constexpr GLuint NONE = numeric_limits<GLuint>::max();

Mesh::Mesh()
 : mVAO(NONE)
 , mVBO(NONE)
 , mCBO(NONE)
 , mTCO(NONE)
 , mNBO(NONE)
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

Mesh* Mesh::generateRegularPolygon(GLuint num, GLdouble r, GLdouble _x, GLdouble _y) {
	
	Mesh* a_mesh = new Mesh();

	a_mesh->mPrimitive = GL_LINE_LOOP;

	a_mesh->mNumVertices = num;
	a_mesh->vVertices.reserve(a_mesh->mNumVertices);

	GLdouble a_alpha = glm::radians(90.0f);

	GLdouble x = r * cos(a_alpha) + _x;
	GLdouble y = r * sin(a_alpha) + _y;
	a_mesh->vVertices.push_back(glm::vec3(x, y, 0));

	GLdouble a_spinOffset = glm::radians(360.0f / num);

	for (GLuint i = 1; i < num; ++i) {
		a_alpha += a_spinOffset;
		GLdouble x = r * cos(a_alpha) + _x;
		GLdouble y = r * sin(a_alpha) + _y;
		a_mesh->vVertices.push_back(glm::vec3(x, y, 0));
	}

	return a_mesh;
}

Mesh* Mesh::generateRGBTriangle(GLdouble r, vec3 pos) {

	Mesh* a_mesh = new Mesh();

	a_mesh->mPrimitive = GL_TRIANGLES;

	a_mesh->mNumVertices = 3;
	a_mesh->vVertices.reserve(3);

	GLdouble a_alpha = glm::radians(90.0f);

	// Creaci�n de la malla

	GLdouble x = r * cos(glm::radians(90.0f));
	GLdouble y = r * sin(glm::radians(90.0f));
	a_mesh->vVertices.push_back(glm::vec3(x + pos.x, y + pos.y, 0));

	x = r * cos(glm::radians(210.0f));
	y = r * sin(glm::radians(210.0f));
	a_mesh->vVertices.push_back(glm::vec3(x + pos.x, y + pos.y, 0));

	x = r * cos(glm::radians(330.0f));
	y = r * sin(glm::radians(330.0f));
	a_mesh->vVertices.push_back(glm::vec3(x + pos.x, y + pos.y, 0));

	// Asignaci�n de colores

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

		if (vTexCoords.size() > 0) { //upload textures
			glGenBuffers(1, &mTCO);
			glBindBuffer(GL_ARRAY_BUFFER, mTCO);
			glBufferData(GL_ARRAY_BUFFER, vTexCoords.size() * sizeof(vec2), vTexCoords.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), nullptr);
			glEnableVertexAttribArray(2);
		}

		if (vNormals.size() > 0) {
			glGenBuffers(1, &mNBO);
			glBindBuffer(GL_ARRAY_BUFFER, mNBO);
			glBufferData(GL_ARRAY_BUFFER,
				vNormals.size() * sizeof(vec3),
				vNormals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
				sizeof(vec3), nullptr);
			glEnableVertexAttribArray(3);
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

		if (mTCO != NONE) glDeleteBuffers(1, &mTCO);
		if (mNBO != NONE) glDeleteBuffers(1, &mNBO);
		mNBO = NONE;
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
Mesh::generateRectangle(GLdouble w, GLdouble h, GLfloat y) {

	Mesh* a_mesh = new Mesh;

	a_mesh->mPrimitive = GL_TRIANGLE_STRIP;

	a_mesh->mNumVertices = 4;
	a_mesh->vVertices.reserve(a_mesh->mNumVertices);

	a_mesh->vVertices.push_back(vec3(w / 2, -h / 2, y));
	a_mesh->vVertices.push_back(vec3(-w / 2, -h / 2, y));
	a_mesh->vVertices.push_back(vec3(w / 2, h / 2, y));
	a_mesh->vVertices.push_back(vec3(-w / 2, h / 2, y));

	return a_mesh;
}

Mesh*
Mesh::generateRGBRectangle(GLdouble w, GLdouble h) {
	Mesh* a_mesh = generateRectangle(w, h, 0);

	a_mesh->vColors.reserve(a_mesh->mNumVertices);

	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));

	return a_mesh;
}

Mesh*
Mesh::generateCube(GLdouble length) {

	Mesh* a_mesh = new Mesh;

	a_mesh->mPrimitive = GL_TRIANGLE_STRIP;

	a_mesh->mNumVertices = 14;
	a_mesh->vVertices.reserve(a_mesh->mNumVertices);

	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, -length/2));
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, length / 2));

	return a_mesh;
}

Mesh*
Mesh::generateRGBCubeTriangles(GLdouble length) {
	Mesh* a_mesh = new Mesh;

	a_mesh->mPrimitive = GL_TRIANGLES;

	a_mesh->mNumVertices = 36;
	a_mesh->vVertices.reserve(a_mesh->mNumVertices);

	//Triangulo 1
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, length / 2, -length / 2));
	
	//Triangulo 2
	a_mesh->vVertices.push_back(vec3(length / 2, length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, length / 2, length / 2));

	//Triangulo 3
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, length / 2, length / 2));

	//Triangulo 4
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, length / 2));

	//Triangulo 5
	a_mesh->vVertices.push_back(vec3(length / 2, length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, length / 2, -length / 2));

	//Triangulo 6
	a_mesh->vVertices.push_back(vec3(length / 2, length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, -length / 2));

	//Triangulo 7
	a_mesh->vVertices.push_back(vec3(length / 2, length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, -length / 2));

	//Triangulo 8
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, -length / 2));

	//Triangulo 9
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, length / 2));

	//Triangulo 10
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, length / 2, length / 2));

	//Triangulo 11
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, length / 2));

	//Triangulo 12
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(length / 2, -length / 2, -length / 2));
	a_mesh->vVertices.push_back(vec3(-length / 2, -length / 2, length / 2));

	a_mesh->vColors.reserve(a_mesh->mNumVertices);

	//Triangulos 1 y 2
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));

	//Triangulos 3 y 4
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));

	//Triangulos 5 y 6
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));

	//Triangulos 7 y 8
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	a_mesh->vColors.emplace_back(vec4(1, 0, 0, 1));

	//Triangulos 9 y 10
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	a_mesh->vColors.emplace_back(vec4(0, 1, 0, 1));

	//Triangulos 11 y 12
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	a_mesh->vColors.emplace_back(vec4(0, 0, 1, 1));

	return a_mesh;
}

Mesh*
Mesh::generateRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh, GLfloat y) {
	Mesh* a_mesh = generateRectangle(w, h, y);

	a_mesh->vTexCoords.reserve(a_mesh->mNumVertices);

	
	a_mesh->vTexCoords.emplace_back(vec2(rw, 0));
	a_mesh->vTexCoords.emplace_back(vec2(0, 0));
	a_mesh->vTexCoords.emplace_back(vec2(rw, rh));
	a_mesh->vTexCoords.emplace_back(vec2(0, rh));
	
	return a_mesh;
}

Mesh*
Mesh::generateBoxOutline(GLdouble length, GLint x, GLint y, GLint z) {

	Mesh* a_mesh = new Mesh;

	a_mesh->mPrimitive = GL_TRIANGLE_STRIP;

	a_mesh->mNumVertices = 10;
	a_mesh->vVertices.reserve(a_mesh->mNumVertices);

	a_mesh->vVertices.push_back(vec3(length / 2 + x, length / 2 + y, length / 2 + z));
	a_mesh->vVertices.push_back(vec3(length / 2 + x, -length / 2 + y, length / 2 + z));
	a_mesh->vVertices.push_back(vec3(length / 2 + x, length / 2 + y, -length / 2 + z));
	a_mesh->vVertices.push_back(vec3(length / 2 + x, -length / 2 + y, -length / 2 + z));
	a_mesh->vVertices.push_back(vec3(-length / 2 + x, length / 2 + y, -length / 2 + z));
	a_mesh->vVertices.push_back(vec3(-length / 2 + x, -length / 2 + y, -length / 2 + z));
	a_mesh->vVertices.push_back(vec3(-length / 2 + x, length / 2 + y, length / 2 + z));
	a_mesh->vVertices.push_back(vec3(-length / 2 + x, -length / 2 + y, length / 2 + z));
	a_mesh->vVertices.push_back(vec3(length / 2 + x, length / 2 + y, length / 2 + z));
	a_mesh->vVertices.push_back(vec3(length / 2 + x,  -length / 2 + y, length / 2 + z));

	return a_mesh;
}

Mesh*
Mesh::generateBoxOutlineTexCor(GLdouble length, GLint x, GLint y, GLint z) {
	Mesh* a_mesh = generateBoxOutline(length, x, y, z);

	a_mesh->vTexCoords.reserve(a_mesh->mNumVertices);

	a_mesh->vTexCoords.emplace_back(0, 1);
	a_mesh->vTexCoords.emplace_back(0, 0);
	a_mesh->vTexCoords.emplace_back(1, 1);
	a_mesh->vTexCoords.emplace_back(1, 0);

	a_mesh->vTexCoords.emplace_back(0, 1);
	a_mesh->vTexCoords.emplace_back(0, 0);
	a_mesh->vTexCoords.emplace_back(1, 1);
	a_mesh->vTexCoords.emplace_back(1, 0);

	a_mesh->vTexCoords.emplace_back(0, 1);
	a_mesh->vTexCoords.emplace_back(0, 0);

	return a_mesh;
}

Mesh*
Mesh::generateStar3D(GLdouble re, GLuint np, GLdouble h, GLint _x, GLint _y, GLint _z) {
	Mesh* a_mesh = new Mesh;

	a_mesh->mPrimitive = GL_TRIANGLE_FAN;

	a_mesh->mNumVertices = np * 2 + 2;
	a_mesh->vVertices.reserve(a_mesh->mNumVertices);

	a_mesh->vVertices.push_back(vec3(_x, _y, _z));

	GLdouble a_alpha = 0;

	float x = re * cos(radians(a_alpha));
	float y = re * sin(radians(a_alpha));

	for (int i = 0; i < np; ++i) {

		a_mesh->vVertices.push_back(vec3(x + _x, y + _y, h + _z));

		x = (re/2) * cos(radians(a_alpha + (360 / (np * 2))));
		y = (re/2) * sin(radians(a_alpha + (360 / (np * 2))));

		a_mesh->vVertices.push_back(vec3(x + _x, y + _y, h + _z));

		a_alpha += 360 / np;

		x = re * cos(radians(a_alpha));
		y = re * sin(radians(a_alpha));

	}

	a_mesh->vVertices.push_back(vec3(x + _x, y + _y, h + _z));

	return a_mesh;
}

Mesh*
Mesh::generateStar3DTexCor(GLdouble re, GLuint np, GLdouble h, GLint _x, GLint _y, GLint _z) {
	Mesh* a_mesh = generateStar3D(re, np, h, _x, _y, _z);

	a_mesh->vTexCoords.reserve(a_mesh->mNumVertices);

	a_mesh->vTexCoords.emplace_back(vec2( 0.5f, 0.5f ));

	GLdouble a_alpha = 0;

	float x = cos(radians(a_alpha))/2 + 0.5f;
	float y = sin(radians(a_alpha))/2 + 0.5f;

	for (int i = 0; i < np; ++i) {

		a_mesh->vTexCoords.emplace_back(vec2(x, y));

		x = cos(radians(a_alpha + (360 / (np * 2))))/2 + 0.5f;
		y = sin(radians(a_alpha + (360 / (np * 2))))/2 + 0.5f;

		a_mesh->vTexCoords.emplace_back(vec2(x, y));

		a_alpha += 360 / np;

		x = cos(radians(a_alpha))/2 + 0.5f;
		y = sin(radians(a_alpha))/2 + 0.5f;
	}

	a_mesh->vTexCoords.emplace_back(vec2(x, y));

	return a_mesh;
}

Mesh*
Mesh::generateTIEWing(GLdouble width, GLdouble height, GLdouble profundity, GLdouble x, GLdouble y, GLdouble z) {
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(8);

	mesh->vVertices.push_back({ x - width / 2, y + height, z + profundity / 2 });
	mesh->vVertices.push_back({ x - width / 2, y + height, z - profundity / 2 });
	mesh->vVertices.push_back({ x, y + height / 2, z + profundity/2});
	mesh->vVertices.push_back({ x, y + height / 2, z - profundity / 2 });
	mesh->vVertices.push_back({ x, y - height / 2, z + profundity / 2 });
	mesh->vVertices.push_back({ x, y - height / 2, z - profundity / 2 });
	mesh->vVertices.push_back({ x - width / 2, y - height, z + profundity / 2 });
	mesh->vVertices.push_back({ x - width / 2, y - height, z - profundity / 2 });

	mesh->vTexCoords.reserve(8);

	mesh->vTexCoords.emplace_back(vec2(0.0f, 1.0f));
	mesh->vTexCoords.emplace_back(vec2(0.0f, 0.0f));
	mesh->vTexCoords.emplace_back(vec2(0.33f, 1.0f));
	mesh->vTexCoords.emplace_back(vec2(0.33f, 0.0f));
	mesh->vTexCoords.emplace_back(vec2(0.66f, 1.0f));
	mesh->vTexCoords.emplace_back(vec2(0.66f, 0.0f));
	mesh->vTexCoords.emplace_back(vec2(1.0f, 1.0f));
	mesh->vTexCoords.emplace_back(vec2(1.0f, 0.0f));

	return mesh;

}