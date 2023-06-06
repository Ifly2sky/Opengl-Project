#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;
uniform sampler2D texture0;

void main()
{
    FragColor = texture(texture0, texCoord);//vec4(1.0,1.0,0.0,1.0);
}