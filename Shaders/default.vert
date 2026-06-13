#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 LightPos;

out vec2 texCoord;
out vec3 normal;
out vec3 vertPos;
out vec3 lightPos;
void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	// transform into view space the vertex, normal, and light position
	vec4 vertPos4 = view * model * vec4(aPos, 1.0);
	vertPos = vec3(vertPos4).xyz;
	normal = mat3(transpose(inverse(view * model))) * aNorm;
	lightPos = vec3(view * vec4(LightPos, 1.0));
	texCoord = aTex;
}