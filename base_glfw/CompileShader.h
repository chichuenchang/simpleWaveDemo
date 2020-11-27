#pragma once
#include <glad/glad.h>
#include <vector>
#include "util.hpp"

class CompileShader
{
public:
	CompileShader();

	GLuint shaderID;
	void reloadShader();

private:
	GLuint programId;	

};

