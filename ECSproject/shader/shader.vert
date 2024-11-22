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
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
  
out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}