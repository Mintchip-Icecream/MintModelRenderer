#include "Model3D.h"

void Model::update()
{
	currentRotation += rotationPerFrame * ((float)1 / 60) * speed;
}

void Model::draw(unsigned int shaderProgram)
{
	glm::mat4 model = glm::mat4(1.0f);

	//model = glm::translate(model, glm::vec3, posx, posy, posz);
	model = glm::rotate(model, glm::radians(currentRotation),
		//glm::vec3(0.05f, 1.0f, 0.0f));
		glm::vec3(0.05f, 1.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shaderProgram);
	}
}

void Model::loadModel(const std::string& modelFile) {
	Assimp::Importer importer;
	// read the file to get the scene graph
	const aiScene* scene = importer.ReadFile(modelFile,
		// put the file loading flags here
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs
	);
	if (scene == nullptr) { // handle error
		std::cout << "Failed to read scene: " << importer.GetErrorString() << std::endl;
		return;
	}

	modelDirectory = modelFile.substr(0, modelFile.find_last_of('\\'));

	copyNodeWithMesh(scene->mRootNode, scene);
}

void Model::copyNodeWithMesh(aiNode *node, const aiScene* scene) {
	// process the meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(copyMesh(mesh, scene));
	}
	// repeat process for n amount of children of this node
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		copyNodeWithMesh(node->mChildren[i], scene);
	}
}

unsigned int Model::createTexture(const string path)
{
	unsigned int textureID;

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

	glGenTextures(1, &textureID);
	if (data) {
		std::cout << "loaded texture: " << path << std::endl;
		GLenum format; 
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		std::cout << "Num of components: " << std::to_string(nrComponents) << std::endl;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		stbi_image_free(data);
	}
	else {
		std::cout << "failed to load texture: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}
