#version 330 core
out vec4 FragColor;
//uniform vec4 ourColor; // we set this variable in the OpenGL code.

//uniform sampler2D tex0;

uniform vec3 defaultAmbient;
uniform vec3 defaultSpecular;

uniform vec3 lightColor;

// texture stuff
uniform sampler2D diffuse1;
in vec2 texCoord;
in vec3 normal;
in vec3 FragPos;
in vec3 lightPos;


void main()
{
	vec4 texColor = texture(diffuse1, texCoord);
	if (texColor.a < 0.05)
		discard;


	vec3 normal = normalize(normal);
	vec3 lightDir = lightPos - FragPos;
	lightDir = normalize(lightDir);

	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 viewDir = normalize(FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 result = defaultAmbient * lightColor +
		diff * lightColor +
		defaultSpecular * spec * lightColor;

	result = result * vec3(texColor).xyz;
	FragColor = vec4(result, 1.0);
}