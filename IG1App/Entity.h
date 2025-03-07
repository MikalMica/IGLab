#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Abs_Entity // abstract class
{
public:
	Abs_Entity()
	  : mModelMat(1.0)  // 4x4 identity matrix
	  , mShader(nullptr) {};
	virtual ~Abs_Entity();

	Abs_Entity(const Abs_Entity& e) = delete;            // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete; // no copy assignment

	virtual void render(const glm::mat4& modelViewMat) const = 0; // abstract method
	virtual void update() {}

	// modeling matrix
	glm::mat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::mat4 const& aMat) { mModelMat = aMat; };

	// load or unload entity data into the GPU
	void load();
	void unload();

protected:
	Mesh* mMesh = nullptr; // the mesh
	glm::mat4 mModelMat;  // modeling matrix
	Shader* mShader; // shader

	// transfers modelViewMat to the GPU
	virtual void upload(const glm::mat4& mModelViewMat) const;
};

class EntityWithColors : public Abs_Entity
{
public:
	explicit EntityWithColors();
	virtual void render(const glm::mat4& modelViewMat) const override;
};

class SingleColorEntity : public Abs_Entity {
protected:
	glm::vec4 mColor;
public:
	SingleColorEntity(glm::vec4 color);
	glm::vec4 Color() { return mColor; };
	void setColor(glm::vec4 color);
	void render(const glm::mat4& modelViewMat) const override;
};

class EntityWithTexture : public Abs_Entity {
protected:
	Texture* mTexture; //Texture to be rendered
	bool mModulate; //true if image is going to modulate with vertex colors
public:
	explicit EntityWithTexture();
	virtual void render(const glm::mat4& modelViewMat) const override;
	inline void setTexture(Texture* newText) { mTexture = newText; }
};

class RGBAxes : public EntityWithColors
{
public:
	explicit RGBAxes(GLdouble l);
};

class Polygon : public EntityWithColors 
{
public:
	explicit Polygon(GLuint num, GLdouble r);
};

class RegularPolygon : public SingleColorEntity
{
public:
	explicit RegularPolygon(GLuint num, GLdouble r);
};

class BiFacedRGBTriangle : public EntityWithColors
{
public:
	explicit BiFacedRGBTriangle(GLdouble r);
	void render(const glm::mat4& modelViewMat) const override;
};

class RGBTriangle : public EntityWithColors
{
public:
	explicit RGBTriangle(GLdouble r, glm::vec3 pos = glm::vec3(0, 0, 0));
	void update() override;
};

class RGBRectangle : public EntityWithColors
{
public:
	explicit RGBRectangle(GLdouble w, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
};

class Cube : public SingleColorEntity 
{
public:
	explicit Cube(GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;
};

class RGBCube : public EntityWithColors 
{
public:
	explicit RGBCube(GLdouble length);
};

class Ground : public EntityWithTexture
{
public:
	explicit Ground(GLdouble x, GLdouble z, GLuint rw, GLuint rh);
};

class BoxOutline : public EntityWithTexture
{
	Texture* mExtraTexture;
public:
	explicit BoxOutline(GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;
	inline void setExtraTexture(Texture* newText) { mExtraTexture = newText; }
};

class Star3D : public EntityWithTexture
{
	Mesh* mExtraMesh;
public:
	explicit Star3D(GLdouble re, GLuint np, GLdouble h);
	inline ~Star3D() { mExtraMesh->unload(); delete mExtraMesh; mExtraMesh = nullptr; }
	void render(const glm::mat4& modelViewMat) const override;
	void update() override;
};

class GlassParapet : public EntityWithTexture
{
public:
	explicit GlassParapet(float length);
};

class Photo : public EntityWithTexture
{
public:
	explicit Photo(float length);
	void update() override;
};
#endif //_H_Entities_H_
