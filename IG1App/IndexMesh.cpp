#include "IndexMesh.h"

void 
IndexMesh::load() {
	Mesh::load(); glBindVertexArray(mVAO);
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		vIndexes.size() * sizeof(GLuint),
		vIndexes.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

// IMPORTANTE este metodo no hace una mierda
void 
IndexMesh::unload() {
	Mesh::unload(); 
}

void 
IndexMesh::draw() const{
	glDrawElements(
		mPrimitive, 
		vIndexes.size(), 
		GL_UNSIGNED_INT, 
		nullptr);
}

IndexMesh* 
IndexMesh::generateByRevolution(
	const std::vector<glm::vec2>& profile, GLuint nSamples, 
	GLfloat angleMax) {

	IndexMesh* mesh = new IndexMesh;
	mesh->mPrimitive = GL_TRIANGLES;
	int tamPerfil = profile.size();
	mesh->vVertices.reserve(nSamples * tamPerfil);

	// Genera los vertices de las muestras
	GLdouble theta1 = angleMax / nSamples;

	for (int i = 0; i <= nSamples; ++i) { // muestra i-esima
		GLdouble c = cos(i * theta1), s = sin(i * theta1);
		for (auto p : profile) {
			mesh->vVertices.emplace_back(p.x * c, p.y, -p.x * s);
		}
	}

	return mesh;
}