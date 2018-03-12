#version 330

in vec3 texCoords;
uniform samplerCube cubeTexture;
out vec4 fragColour;

void main(){
	fragColour = texture(cubeTexture, texCoords);
}