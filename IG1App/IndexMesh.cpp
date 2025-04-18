#include "IndexMesh.h"
#include <iostream>

using namespace std;

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
	mesh->mNumVertices = nSamples * tamPerfil;

	// Genera los vertices de las muestras
	GLdouble theta1 = angleMax / nSamples;

	for (int i = 0; i <= nSamples; ++i) { // muestra i-esima
		GLdouble c = cos(i * theta1), s = sin(i * theta1);
		for (auto p : profile) {
			mesh->vVertices.emplace_back(p.x * c, p.y, -p.x * s);
		}
	}

	for (int i = 0; i < nSamples; ++i) // caras i a i + 1
		for (int j = 0; j < tamPerfil - 1; ++j) { // una cara
			if (profile[j].x != 0.0) // triángulo inferior
				for (auto [s, t] : { pair{i, j}, {i, j + 1}, {i + 1, j} })
					mesh->vIndexes.push_back(s * tamPerfil + t);
			if (profile[j + 1].x != 0.0) // triángulo superior
				for (auto [s, t] : { pair{i, j + 1}, {i + 1, j + 1}, {i + 1, j} })
					mesh->vIndexes.push_back(s * tamPerfil + t);
		}
	mesh->mNumVertices = mesh->vVertices.size();

	mesh->buildNormalVectors();

	return mesh;
}

IndexMesh* 
IndexMesh::generateIndexedBox(GLdouble L) {

	IndexMesh* mesh = new IndexMesh;
	mesh->mPrimitive = GL_TRIANGLES;
	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(8);
	mesh->vIndexes.reserve(36);
	mesh->vNormals.reserve(8);

	// Push back the vertices of the cube
	mesh->vVertices.push_back({ L / 2, L / 2, -L / 2 }); // 0 ++-
	mesh->vVertices.push_back({ L / 2, -L / 2, -L / 2}); // 1 +--
	mesh->vVertices.push_back({ L / 2, L / 2, L / 2 }); // 2 +++
	mesh->vVertices.push_back({ L / 2, -L / 2, L / 2 }); // 3 +-+
	mesh->vVertices.push_back({ -L / 2, L / 2, L / 2 }); // 4 -++
	mesh->vVertices.push_back({ -L / 2, -L / 2, L / 2 }); // 5 --+
	mesh->vVertices.push_back({ -L / 2, L / 2, -L / 2 }); // 6 -+-
	mesh->vVertices.push_back({ -L / 2, -L / 2, -L / 2 }); // 7 ---

	// Set the indexes of the triangles

	// Triangle 1
	mesh->vIndexes.push_back(0);
	mesh->vIndexes.push_back(1);
	mesh->vIndexes.push_back(2);

	// Triangle 2
	mesh->vIndexes.push_back(2);
	mesh->vIndexes.push_back(1);
	mesh->vIndexes.push_back(3);

	// Triangle 3
	mesh->vIndexes.push_back(2);
	mesh->vIndexes.push_back(3);
	mesh->vIndexes.push_back(4);

	// Triangle 4
	mesh->vIndexes.push_back(4);
	mesh->vIndexes.push_back(3);
	mesh->vIndexes.push_back(5);

	// Triangle 5
	mesh->vIndexes.push_back(4);
	mesh->vIndexes.push_back(5);
	mesh->vIndexes.push_back(6);

	// Triangle 6
	mesh->vIndexes.push_back(6);
	mesh->vIndexes.push_back(5);
	mesh->vIndexes.push_back(7);

	// Triangle 7
	mesh->vIndexes.push_back(6);
	mesh->vIndexes.push_back(7);
	mesh->vIndexes.push_back(0);

	// Triangle 8
	mesh->vIndexes.push_back(0);
	mesh->vIndexes.push_back(7);
	mesh->vIndexes.push_back(1);

	// Triangle 9
	mesh->vIndexes.push_back(4);
	mesh->vIndexes.push_back(6);
	mesh->vIndexes.push_back(2);

	// Triangle 10
	mesh->vIndexes.push_back(2);
	mesh->vIndexes.push_back(6);
	mesh->vIndexes.push_back(0);

	// Triangle 11
	mesh->vIndexes.push_back(1);
	mesh->vIndexes.push_back(7);
	mesh->vIndexes.push_back(3);

	// Triangle 12
	mesh->vIndexes.push_back(3);
	mesh->vIndexes.push_back(7);
	mesh->vIndexes.push_back(5);

	// Set the normals
	mesh->buildNormalVectors();

	return mesh;
}

void
IndexMesh::buildNormalVectors() {

	for (int i = 0; i < vVertices.size(); ++i) {
		vNormals.push_back({ 0, 0, 0 });
	}

	for (int i = 0; i < vIndexes.size(); i = i + 3) { // Suponemos triangulos
		glm::vec3 normal = glm::normalize(glm::cross(vVertices[vIndexes[i + 1]] - vVertices[vIndexes[i]], 
													 vVertices[vIndexes[i + 2]] - vVertices[vIndexes[i]]));

		vNormals[vIndexes[i]] += normal;
		vNormals[vIndexes[i+1]] += normal;
		vNormals[vIndexes[i+2]] += normal;
	}

	for (int i = 0; i < vNormals.size(); ++i) {
		vNormals[i] = glm::normalize(vNormals[i]);
	}
}