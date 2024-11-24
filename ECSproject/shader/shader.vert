/* #version 330 core

layout(location = 0) in vec3 aPos;  // 位置
layout(location = 1) in vec2 aTexCoord;  // 纹理坐标

uniform mat4 model;  // 模型变换矩阵
uniform mat4 projection; // 投影矩阵

out vec2 TexCoord;

void main()
{
    gl_Position = model * vec4(aPos, 1.0); // 应用模型矩阵变换
    TexCoord = aTexCoord;                 // 传递纹理坐标给片段着色器
}*/

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model; // 模型矩阵
uniform mat4 view; // 观察矩阵
uniform mat4 projection; // 投影矩阵

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}