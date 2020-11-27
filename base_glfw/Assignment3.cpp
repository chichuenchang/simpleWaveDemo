#include "Assignment3.h"
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

#include "util.hpp"



Assignment3::Assignment3()
{

}

void Assignment3::initVAO() {
	glEnable(GL_DEPTH_TEST);
	
	//struct to hold vertices data
	struct gridVert {
		glm::vec3 pos;
		glm::vec2 texCrd;
	};
	std::vector<gridVert> grid;
	//std::vector<gridVert> resetGrid;

	for (int i = -numGrid / 2; i < numGrid / 2; i++) {
		for (int j = -numGrid / 2; j < numGrid / 2; j++) {
			grid.push_back({ glm::vec3(-0.1f*j, 0.0f, -0.1f*i), glm::vec2((j + numGrid / 2.0f) / numGrid, (i + numGrid / 2.0f) / numGrid) });
		}
	}
	//resetGrid = grid;

	glGenVertexArrays(1, &_assignVAO);
	VAOID = _assignVAO;
	glBindVertexArray(_assignVAO);

	GLuint assignVBO;
	glGenBuffers(1, &assignVBO);
	glBindBuffer(GL_ARRAY_BUFFER, assignVBO);
	glBufferData(GL_ARRAY_BUFFER, grid.size() * sizeof(grid[0]), grid.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gridVert), 0);
	glEnableVertexAttribArray(2);//put the index 2 here as texCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(gridVert), (GLvoid*)offsetof(gridVert, texCrd));

	std::vector<unsigned int> indices;

	for (int i = 0; i < numGrid - 1; i++)
	{
		for (int j = 0; j < numGrid; j++)
		{
			indices.push_back(i*numGrid + j);
			indices.push_back(i*numGrid + j + numGrid);
		}
		indices.push_back(99999);
	}

	unsigned int AssignIBO;
	glGenBuffers(1, &AssignIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AssignIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	//unbind in the last
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Assignment3::drawVAO(GLuint VAO_ID) {
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(99999);
	glBindVertexArray(VAO_ID); 

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLE_STRIP, (numGrid - 1) * 2 * numGrid + (numGrid - 1), GL_UNSIGNED_INT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void Assignment3::initTexture(GLuint * textureID, unsigned char * texData) {
	int tex_width = 0;
	int tex_height=0;

	//standard texture init calls
	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	assert(glGetError() == GL_NO_ERROR);
}