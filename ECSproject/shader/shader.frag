/* #version 330 core

in vec2 TexCoord;    // 从顶点着色器传递来的纹理坐标
out vec4 FragColor;  // 输出颜色

uniform sampler2D texture1;  // 绑定的纹理

void main()
{
    FragColor = texture(texture1, TexCoord); // 采样纹理并设置颜色
} */

#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}