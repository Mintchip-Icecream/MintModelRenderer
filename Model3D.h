#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vec3.hpp>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include <Object.h>
#include <Mesh.h>
#include <iostream>
#include <stb/stb_image.h>
using namespace std;

class Model : public Object {
public:
	Model(const std::string& modelFile) {
		loadModel(modelFile);
	}
	void update() override;
	void draw(unsigned int shaderProgram) override;
private:
	float currentRotation = 0.0f;
	float rotationPerFrame = -1.0f;
	float speed = 0.6f;
	float posx, posy, posz = 0;
	string modelDirectory;
	vector<Mesh> meshes;
	vector<Texture> textures;
	void loadModel(const std::string& modelFile); // uses assimp to load model
	void copyNodeWithMesh(aiNode* node, const aiScene* scene); // recursive mesh node travel in scene
	Mesh copyMesh(aiMesh* mesh, const aiScene* scene) { // generates mesh from aimesh
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		// populate vertices with the vertices, normals, and texture coords
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex theVertex;

			// get the position vertex
			glm::vec3 vector;
			aiVector3D aiVec = mesh->mVertices[i];
			vector.x = aiVec.x;
			vector.y = aiVec.y;
			vector.z = aiVec.z;
			theVertex.Position = vector;
			if (mesh->HasNormals()) {
				// add normals
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				theVertex.Normal = vector;
			}
			if (mesh->mTextureCoords[0]) {
				// add textures if there's texture coords
				glm::vec2 vector2;
				vector2.x = mesh->mTextureCoords[0][i].x;
				vector2.y = mesh->mTextureCoords[0][i].y;
				theVertex.TexCoords = vector2;
			}
			else {
				theVertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			vertices.push_back(theVertex);
		}
		// populate indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		// populate textures
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we only use diffuse textures for now
		vector<Texture> diffuse;
		for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
			// get the path of the texture
			aiString str;
			if (material->GetTexture(aiTextureType_DIFFUSE, i, &str) == AI_SUCCESS) {
				string p(str.data);

				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}

				Texture texture;
				const string filepath = modelDirectory + '/' + p;
				texture.id = createTexture(filepath);
				texture.type = "diffuse";
				texture.path = str.C_Str();
				textures.push_back(texture);

			}

		}

		return Mesh(vertices, indices, textures);
	} // creates a mesh
	unsigned int createTexture(const string path); // creates a texture for a mesh
};