#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <string>
#include <vector>
#include <iostream>
using namespace std;


// a mesh is made up of the geometry (stored in vertex buffer), 
// topology (stored in index buffer), and the textures (using the UV coords in the vertex buffer)
struct Vertex { // we upload the position, normal and texcoords for the vertex buffer
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		std::cout << "Vertex count: " << std::to_string(vertices.size()) << std::endl;
		std::cout << "IBO count: " << std::to_string(indices.size()) << std::endl;
		std::cout << "Texture count: " << std::to_string(textures.size()) << std::endl;

		initialize();
	}
	void draw(unsigned int shaderProgram);
private:
	unsigned int VBO, VAO, IBO;
	void initialize() {
		// set up the buffers

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);

		// fill in buffers
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// the VBO is gonna get the vertices, which contain a vec3 for position,
		// vec3 for normals, and vec2 for the UV map, the size of each array will be 6 floats in a struct
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		// set up the VBO format for the vertex shader to intake

		// this is our position coords
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// these are the normals, indexed at the 4th float
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		// these are the UVs, indexed at the 7th float
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));

		// fill in faces buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);

	}
};