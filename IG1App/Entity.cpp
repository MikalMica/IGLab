#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Abs_Entity::upload(const mat4& modelViewMat) const
{
	mShader->setUniform("modelView", modelViewMat);
}

Abs_Entity::~Abs_Entity()
{
	delete mMesh;
	mMesh = nullptr;
}

void
Abs_Entity::load()
{
	mMesh->load();
}

void
Abs_Entity::unload()
{
	mMesh->unload();
}

EntityWithColors::EntityWithColors()
{
	mShader = Shader::get("vcolors");
}

void
EntityWithColors::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mMesh->render();
	}
}

SingleColorEntity::SingleColorEntity(vec4 color = vec4(1, 1, 1, 1)) 
{
	mColor = color;
	mShader = Shader::get("simple");
}

void
SingleColorEntity::setColor(vec4 color) 
{
	mColor = color;
}

void
SingleColorEntity::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("color", mColor);
		mMesh->render();
	}
}

EntityWithTexture::EntityWithTexture() : mModulate(false), mTexture(nullptr)
{
	mShader = Shader::get("texture");
}

void
EntityWithTexture::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();

		mShader->setUniform("modulate", mModulate); // sets the modulate
		upload(aMat);

		if (mTexture != nullptr) mTexture->bind(); // binds the texture if it isn't nullptr

		mMesh->render();

		if (mTexture != nullptr) mTexture->unbind(); // unbinds the texture
	}
}

RGBAxes::RGBAxes(GLdouble l)
{
	mShader = Shader::get("vcolors");
	mMesh = Mesh::createRGBAxes(l);
	load();
}

Polygon::Polygon(GLuint num, GLdouble r) {

	mShader = Shader::get("vcolors");
	mMesh = Mesh::generateRegularPolygon(num, r);
	load();
}

RegularPolygon::RegularPolygon(GLuint num, GLdouble r) {

	mShader = Shader::get("simple");
	mMesh = Mesh::generateRegularPolygon(num, r);
	load();
}

BiFacedRGBTriangle::BiFacedRGBTriangle(GLdouble r) {

	mShader = Shader::get("vcolors");
	mMesh = Mesh::generateRGBTriangle(r);
	load();
}

void
BiFacedRGBTriangle::render(mat4 const& modelViewMat) const 
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE);

		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		mMesh->render();

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		glDisable(GL_CULL_FACE);
	}
}

RGBTriangle::RGBTriangle(GLdouble r, vec3 pos) {

	mShader = Shader::get("vcolors");
	mMesh = Mesh::generateRGBTriangle(r, pos);
	load();
}

void
RGBTriangle::update() {
	std::vector<glm::vec3> a_vertices = mMesh->vertices();

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	for (int i = 0; i < a_vertices.size(); ++i) {
		x += a_vertices[i].x;
		y += a_vertices[i].y;
		z += a_vertices[i].z;
	}

	x /= a_vertices.size();
	y /= a_vertices.size();
	z /= a_vertices.size();

	glm::mat4 mT = glm::translate(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), glm::vec3(x, y, 0));
	glm::mat4 mR = glm::rotate(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), glm::radians(-2.0f), glm::vec3(0, 0, 1));
	glm::mat4 mT2 = glm::translate(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), glm::vec3(-x, -y, 0));

	glm::mat4 mR2 = glm::rotate(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), glm::radians(1.0f), glm::vec3(0, 0, 1));
	
	mModelMat *= mT * mR * mT2;

	mModelMat = mR2 * mModelMat;


}

RGBRectangle::RGBRectangle(GLdouble w, GLdouble h) {

	mShader = Shader::get("vcolors");
	mMesh = Mesh::generateRGBRectangle(w, h);
	load();
}

void
RGBRectangle::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
	}
}

Cube::Cube(GLdouble length) {
	mShader = Shader::get("simple");
	mMesh = Mesh::generateCube(length);
	load();
}

void
Cube::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		mMesh->render();

		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		glDisable(GL_CULL_FACE);
	}
}

RGBCube::RGBCube(GLdouble length){
	mShader = Shader::get("vcolors");
	mMesh = Mesh::generateRGBCubeTriangles(length);
	load();
}

Ground::Ground(GLdouble x, GLdouble z, GLuint rw, GLuint rh) {
	mShader = Shader::get("texture");
	mMesh = Mesh::generateRectangleTexCor(x, z,rw, rh);
	// Rotates the rectangle to make it horizontal
	mModelMat = glm::rotate(mModelMat, glm::radians(90.0f), glm::vec3(1, 0, 0));
	load();
}

BoxOutline::BoxOutline(GLdouble length) {
	mShader = Shader::get("texture");
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	load();
}

void
BoxOutline::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();

		mShader->setUniform("modulate", mModulate); // sets the modulate
		upload(aMat);

		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (mTexture != nullptr) mTexture->bind(); // binds the texture if it isn't nullptr

		mMesh->render();

		if (mTexture != nullptr) mTexture->unbind(); // unbinds the texture

		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (mExtraTexture != nullptr) mExtraTexture->bind();

		mMesh->render();

		if (mExtraTexture != nullptr) mExtraTexture->unbind();

		glDisable(GL_CULL_FACE);
	}
}

Star3D::Star3D(GLdouble re, GLuint np, GLdouble h) {
	mShader = Shader::get("texture");
	mMesh = Mesh::generateStar3DTexCor(re, np, h);
	mExtraMesh = Mesh::generateStar3DTexCor(re, np, -h);
	load();
	mExtraMesh->load();
}

void
Star3D::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();

		mShader->setUniform("modulate", mModulate); // sets the modulate
		upload(aMat);

		if (mTexture != nullptr) mTexture->bind(); // binds the texture if it isn't nullptr

		mMesh->render();
		mExtraMesh->render();

		if (mTexture != nullptr) mTexture->unbind(); // unbinds the texture
	}
}

void
Star3D::update() 
{
	std::vector<glm::vec3> a_vertices = mMesh->vertices();

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	for (int i = 0; i < a_vertices.size(); ++i) {
		x += a_vertices[i].x;
		y += a_vertices[i].y;
		z += a_vertices[i].z;
	}

	x /= a_vertices.size();
	y /= a_vertices.size();
	z /= a_vertices.size();

	mModelMat = rotate(mModelMat, radians(1.0f), { 0, 1, 1 });
}

GlassParapet::GlassParapet(float length) {
	mShader = Shader::get("texture");
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	load();
}