#include <iostream>
#include <cassert>
#include <glad/glad.h>
//GLFW
#include <GLFW/glfw3.h>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//customized header
#include "util.hpp"
#include "mesh.hpp"
//gui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//assignment3
#include "assignment3.h"
#include "CompileShader.h"

// Window
GLFWwindow* window = nullptr;
GLfloat clear_color[4] = { 0.05f, 0.2f, 0.2f, 1.00f };
int width = 1600, height = 1600;
//initialize
void initGLFW();
void initOpenGL();
void initGui();
void drawGui();
// GLFW call backs
void scrollCallback(GLFWwindow* w, double x, double y);
void keyCallback(GLFWwindow* w, int key, int sc, int action, int mode);
void mouseButtonCallback(GLFWwindow* w, int b, int action, int mode);
void cursorPosCallback(GLFWwindow* w, double xp, double yp);
void framebufferSizeCallback(GLFWwindow* w, int width, int height);
//GUI

//Camera
bool camRot = false;
glm::vec3 camCoords = glm::vec3(0.0, 0.0, 1.0);
glm::vec2 camOrigin;
glm::vec2 mouseOrigin;
//mouse
glm::vec3 mousPck = glm::vec3(0.001f); //mouse pick coord

//ASSIGNMENT3-----------------------
Assignment3 ASSGN3;
CompileShader ASSGN3SHADER;
bool debugView = 0;

int main() {

	std::cout << "Hello, OpenGL!" << std::endl;
	initGLFW();
	initOpenGL();
	initGui();

	//ASSGN3.reloadShader();
	ASSGN3SHADER.reloadShader();
	ASSGN3.initVAO();

	//[uniform location]
	//mouse pick coordinate location passed into the shader
	GLuint mousPck_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "mousPck");
	//transformaiton
	GLuint xform_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "xform");
	GLuint P_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "P");
	GLuint V_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "V");
	GLuint M_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "M");
	//time
	GLuint tick_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "uTime");
	//debug switch view mode
	GLuint debugView_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "debugSwtch");
	//wave variable
	ASSGN3.Wave1.WvDir_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "WvDir");
	ASSGN3.Wave1.Amp_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "Amp");
	ASSGN3.Wave1.WvLen_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "WvLen");
	ASSGN3.Wave1.Spd_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "Spd");
	ASSGN3.Wave1.StpQ_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "StpQ");
	ASSGN3.Wave1.WvMode_uloc = glGetUniformLocation(ASSGN3SHADER.shaderID, "WvMode");
	

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(clear_color[0],clear_color[1], clear_color[2],clear_color[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		glUseProgram(ASSGN3SHADER.shaderID);

		//transformation
		glm::mat4 transform;
		glm::mat4 M;
		float aspect = (float)width / (float)height;
		glm::mat4 proj = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, -camCoords.z });
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(camCoords.y), { 1.0f, 0.0f, 0.0f });
		rot = glm::rotate(rot, glm::radians(camCoords.x), { 0.0f, 1.0f, 0.0f });
		glm::mat4 scaler = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		transform = proj * view * trans * rot * scaler;
		M = trans * rot * scaler;

		//[Pass uniform here] 
		glUniform3fv(mousPck_uloc, 1, glm::value_ptr(mousPck));
		glUniform1f(tick_uloc, glfwGetTime());
		glUniformMatrix4fv(xform_uloc, 1, GL_FALSE, glm::value_ptr(transform));
		glUniformMatrix4fv(P_uloc, 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(V_uloc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(M_uloc, 1, GL_FALSE, glm::value_ptr(M));
		glUniform1i(debugView_uloc, debugView);
		//wave
		glUniform2fv(ASSGN3.Wave1.WvDir_uloc, 1, glm::value_ptr(ASSGN3.Wave1.WvDir));
		glUniform1f(ASSGN3.Wave1.Amp_uloc, ASSGN3.Wave1.Amp);
		glUniform1f(ASSGN3.Wave1.WvLen_uloc, ASSGN3.Wave1.WvLen);
		glUniform1f(ASSGN3.Wave1.Spd_uloc, ASSGN3.Wave1.Spd);
		glUniform1f(ASSGN3.Wave1.StpQ_uloc, ASSGN3.Wave1.StpQ);
		glUniform1i(ASSGN3.Wave1.WvMode_uloc, ASSGN3.Wave1.WvMode);

		//draw call	
		if (!debugView) { 
			ASSGN3.drawVAO(ASSGN3.VAOID); }
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			ASSGN3.drawVAO(ASSGN3.VAOID); 
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		//draw imGui
		drawGui();
		//unbind VAO & shader
		glBindVertexArray(0);
		glUseProgram(0);
		//assert(glGetError() == GL_NO_ERROR);

		glfwSwapBuffers(window);
	}
	//gui clean up
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, "OpenGL Demo", nullptr, nullptr);
	if (!window) {
		std::cerr << "Cannot create window";
		std::exit(1);
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetScrollCallback(window, scrollCallback);
}


void initOpenGL() {
	assert(window);
	if (gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)) == 0) {
		std::cerr << "Failed to intialize OpenGL loader" << std::endl;
		std::exit(1);
	}
	assert(glGetError() == GL_NO_ERROR);
}

//ImGui ------------------------------------------------------------------------
void initGui() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char*GLSL_VERSION = "#version 330";
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	}

void drawGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//show demo window
	static bool show_demo_window = 1;
	if(show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

	static int counter = 0;
	{
		ImGui::Begin("JC Zheng, Gerstner Wave");                          
		ImGui::Text("Assignmeng3 ");
		ImGui::Text("Shamefully there's a obvious bug in the vertices of the island where they shouldn't have moved with the wave. ");
		ImGui::Text("I think it has something to do with the transformation of space in the vertex shader where all the position  ");
		ImGui::Text("should transfere in the world space.");


		ImGui::Text("Wave Parameters");
		ImGui::SliderFloat2("Wave Direction", (float *)&ASSGN3.Wave1.WvDir, -1.0f, 1.0f);
		ImGui::SliderFloat("Wave Amplitude", &ASSGN3.Wave1.Amp, 0.001f, 1.2f);
		ImGui::SliderFloat("Wave Length", &ASSGN3.Wave1.WvLen, 1.0f, 10.0f);
		ImGui::SliderFloat("Speed", &ASSGN3.Wave1.Spd, 0.1f, 30.0f);
		ImGui::SliderFloat("Steepness", &ASSGN3.Wave1.StpQ, 0.001f, 2.0f);
		ImGui::RadioButton("Directional Wave", &ASSGN3.Wave1.WvMode, 0); ImGui::SameLine();
		ImGui::RadioButton("Circular Wave", &ASSGN3.Wave1.WvMode, 1); 

		ImGui::Checkbox("DebugView", &debugView);

		
		//regular
		ImGui::ColorEdit3("clear color", (float*)&clear_color); 
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


// GLFW window callbacks--------------------------------------------------------------------

void scrollCallback(GLFWwindow* w, double x, double y) {
	float offset = (y > 0) ? 0.1f : -0.1f;
	camCoords.z = glm::clamp(camCoords.z + offset, 0.1f, 10.0f);
}

void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		glfwSetWindowShouldClose(w, true);
	}

	if (key == GLFW_KEY_R && action == GLFW_RELEASE) {
			ASSGN3SHADER.reloadShader();
	}
}

void mouseButtonCallback(GLFWwindow* w, int button, int action, int mode) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		// Activate rotation mode
		camRot = true;
		camOrigin = glm::vec2(camCoords);
		static double xpos, ypos; //has to be double, as is the argument type of glfwGetCursorPos();
		glfwGetCursorPos(w, &xpos, &ypos);
		mouseOrigin = glm::vec2(xpos, ypos);
		//pick function
		double x021 = ASSGN3.numGrid* (xpos - width / 2.0f) / width;
		double y021 = -ASSGN3.numGrid* (ypos - height / 2.0f) / height;
		//debug mousPck
		mousPck = glm::vec3(x021, y021, 0.0f);
	
	
	} if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		camRot = false;

	}
}

void cursorPosCallback(GLFWwindow* w, double xp, double yp) {
	if (camRot) {
		float rotScale = std::fmin(width / 450.f, height / 270.f);
		glm::vec2 mouseDelta = glm::vec2(xp, yp) - mouseOrigin;
		glm::vec2 newAngle = camOrigin + mouseDelta / rotScale;
		newAngle.y = glm::clamp(newAngle.y, -90.0f, 90.0f);
		while (newAngle.x > 180.0f) newAngle.x -= 360.0f;
		while (newAngle.y < -180.0f) newAngle.y += 360.0f;
		if (glm::length(newAngle - glm::vec2(camCoords)) > std::numeric_limits<float>::epsilon()) {
			camCoords.x = newAngle.x;
			camCoords.y = newAngle.y;
		}
	}
}

void framebufferSizeCallback(GLFWwindow* w, int width, int height) {
	::width = width;
	::height = height;
	glViewport(0, 0, width, height);
}

