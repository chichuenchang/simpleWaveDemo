#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>



class Assignment3
{
public:
	Assignment3();
	
	GLuint VAOID;
	const int numGrid = 300;

	struct WaveVar {
		glm::vec2 WvDir = glm::vec2(0.5f, 0.1f);
		float Amp = 0.5f;
		float WvLen = 7.0f;
		float Spd = 20.0f;
		float StpQ = 1.5f;
		int WvMode = 0;
		//uniform location
		GLuint WvDir_uloc, Amp_uloc, WvLen_uloc, Spd_uloc, StpQ_uloc, WvMode_uloc;
	};
	WaveVar Wave1;
	//WaveVar Wave2;

	void initVAO();
	void drawVAO(GLuint VAO_ID);
	void initTexture(GLuint * textureID, unsigned char * texData);

private:
	//[uniform location]
	GLuint _texUnitLoc;
	//textureID
	GLuint _textureId = -1;		
	//VaoID
	GLuint _assignVAO; //= 	GLuint VAOID public

};

