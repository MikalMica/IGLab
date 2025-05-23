#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

class Mesh
{
public:
	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh
	static Mesh* generateRegularPolygon(GLuint num, GLdouble r, GLdouble x = 0, GLdouble y = 0);
	static Mesh* generateRGBTriangle(GLdouble r, glm::vec3 pos = glm::vec3(0, 0, 0));
	static Mesh* generateRectangle(GLdouble w, GLdouble h, GLfloat y);
	static Mesh* generateRGBRectangle(GLdouble w, GLdouble h);
	static Mesh* generateCube(GLdouble length);
	static Mesh* generateRGBCubeTriangles(GLdouble length);
	static Mesh* generateRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh, GLfloat y);
	static Mesh* generateBoxOutline(GLdouble length, GLint x, GLint y , GLint z);
	static Mesh* generateBoxOutlineTexCor(GLdouble length, GLint x, GLint y, GLint z);
	static Mesh* generateStar3D(GLdouble re, GLuint np, GLdouble h, GLint _x, GLint _y, GLint _z);
	static Mesh* generateStar3DTexCor(GLdouble re, GLuint np, GLdouble h, GLint _x, GLint _y, GLint _z);
	static Mesh* generateTIEWing(GLdouble width, GLdouble height, GLdouble profundity,GLdouble x, GLdouble y, GLdouble z);

	Mesh();
	virtual ~Mesh();

	Mesh(const Mesh& m) = delete;            // no copy constructor
	Mesh& operator=(const Mesh& m) = delete; // no copy assignment

	virtual void render() const;

	GLuint size() const { return mNumVertices; }; // number of elements
	std::vector<glm::vec3> const& vertices() const { return vVertices; };
	std::vector<glm::vec4> const& colors() const { return vColors; };

	virtual void load();
	virtual void unload();

protected:
	GLuint mPrimitive =
	  GL_TRIANGLES;          // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0; // number of elements ( = vVertices.size())
	std::vector<glm::vec3> vVertices; // vertex array
	std::vector<glm::vec4> vColors;   // color array
	std::vector<glm::vec2> vTexCoords;
	std::vector<glm::vec3> vNormals; // normals of the mesh
	virtual void draw() const;

	GLuint mVAO;  // vertex array object
	GLuint mNBO;

private:
	GLuint mVBO;  // vertex buffer object
	GLuint mCBO;  // color buffer object
	GLuint mTCO;  // texture coordinates object
};

#endif //_H_Scene_H_
