#include "CompileShader.h"



CompileShader::CompileShader()
{
}


void CompileShader::reloadShader() {
	std::vector<GLuint> shaders;
	shaders.push_back(compileShader(GL_VERTEX_SHADER, "sh_v.glsl"));
	shaders.push_back(compileShader(GL_FRAGMENT_SHADER, "sh_f.glsl"));
	programId = linkProgram(shaders);

	shaderID = programId;
}

