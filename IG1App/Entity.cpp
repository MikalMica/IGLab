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