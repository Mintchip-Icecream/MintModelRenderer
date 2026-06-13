#include "Mesh.h"

void Mesh::draw(unsigned int shaderProgram) {
	unsigned int diffuseNr = 1;
    unsigned int ambientNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        string num;
        string name = textures[i].type;

        if (name == "diffuse")
            num = std::to_string(diffuseNr++);
        if (name == "specular")
            num = std::to_string(ambientNr++);
        if (name == "ambient")
            num = std::to_string(specularNr++);

        // set the sampler to the texture
        glUniform1i(glGetUniformLocation(shaderProgram, (name + num).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

    // clear out everything
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}