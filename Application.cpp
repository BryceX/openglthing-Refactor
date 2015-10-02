//#include "Application.h"
//
//Model LoadObj(const char *file)
//{
//	Model model;
//	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
//	model.temp = stbi_load(file, &imageWidth, &imageHeight, &imageFormat, STBI_default);
//	unsigned int m_texture;
//	glGenTextures(1, &m_texture);
//	glGenTextures(1, &m_texture);
//	glBindTexture(GL_TEXTURE_2D, m_texture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, model.temp);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	stbi_image_free(model.temp);
//	 //load it from disk
//	// load it into OpenGL
//	// return a data structure with all the relevant information (VAO, indexCount)
//
//	return model;
//	
//}
//
//bool Application::Init() 
//{
//	//if (glfwInit() == false)
//	//{
//	//	return false;
//	//}
//	//GLFWwindow* window = glfwCreateWindow(1280, 720, "test", nullptr, nullptr);
//	//glfwMakeContextCurrent(window);
//	//if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
//	//	glfwDestroyWindow(window);
//	//	glfwTerminate();
//	//	return -3;
//	//}
//	//auto major = ogl_GetMajorVersion();
//	//auto minor = ogl_GetMinorVersion();
//	//printf("GL: %i.%i\n", major, minor);
//	//programID = glCreateProgram();
//
//	////funny guy funny guy object = loadObj("./resources/models/chiruchiru.obj");
//
//	//object = LoadObj("./textures/crate.png");
//
//
//
//
//
//
//
//
//	return true;
//}
//bool Application::Update()
//{
//	GetDeltaTime();
//
//
//	return true;
//}
//void Application::Draw()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glUseProgram(programID);
//	object.temp2 = glGetUniformLocation(programID, "ProjectionView");
//
//	object.temp2 = glGetUniformLocation(programID, "diffuse");
//	glUniform1i(object.temp2, 0);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, object.m_texture);
//	glBindVertexArray(m_vao);
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//
//
//
//	// draw my obj
//	// glBindVertexArray(object.VAO);
//	// glDrawElements(stuff, stuff, object.indexCount, stuff);
//
//
//
//
//
//}
//
//void Application::Shut()
//{
//
//	glfwTerminate();
//	glfwDestroyWindow(window);
//}
//void Application::GetDeltaTime()
//{
//	deltaTime = glfwGetTime();
//	glfwSetTime(0);
//}

#include "Application.h"	// header for this file

// system headers
#include <iostream>			

// library headers
#include "gl_core_4_4.h"
#include <glfw3.h>

// custom headers



int Application::Init()
{
	// initializes GLFW, our window manager and input manager
	if (glfwInit() == false)
	{
		return-1;
	}

	window = glfwCreateWindow(windowWidth, windowHeight, "test", nullptr, nullptr);
	if (window == nullptr) {
		// glfw failed to initialize, terminate the prog
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {

		// openGL failed to initialize, terminate the prog
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	// glfw
	glfwSetTime(0.0);	// reset time

	return OnInit();	// ALL CLEAR! No problems.
}

bool Application::Tick()
{
	glfwPollEvents();

	// deltaTime calcs
	deltaTime = glfwGetTime() - timePassed;	// difference in time between this frame and the last
	timePassed = glfwGetTime();	// record the total amount of time elapsed since the beginning of this app

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}

	return OnTick();
}

void Application::Draw()
{
	OnDraw();

	glfwSwapBuffers(window);
}

void Application::Exit()
{
	OnExit();
}

float Application::GetDeltaTime()
{
	return deltaTime;
}

float Application::GetTimePassed()
{
	return timePassed;
}