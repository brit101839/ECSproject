#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D ourTexture;
uniform vec3 overlayColor;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0) * vec4(overlayColor, 1.0);
}