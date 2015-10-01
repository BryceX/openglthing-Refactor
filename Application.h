//#pragma once
//#include <iostream>
//#include "gl_core_4_4.h"
//#include <glfw3.h>
//#include <stb_image.h>
//#include "glm\glm.hpp"
//#include "glm\ext.hpp"
//
//struct Model
//{
//	unsigned char* temp;
//	unsigned int temp2;
//
//	unsigned int m_vao;
//	unsigned int m_ibo;
//	unsigned int m_vbo;
//};
//
//Model LoadObj(const char *file);
//
//class Application 
//{
//public:
//	GLFWwindow* window;
//	float deltaTime;
//	Model object;
//	unsigned int programID;
//	bool Init();
//	void Shut();
//	void GetDeltaTime();
//	bool Update();
//	void Draw();
//};
//
//
//

#pragma once

// forward declarations
class GLFWwindow;

// Base application class
class Application
{
private:
	float timePassed = 0;
	float deltaTime = 0;

public:
	// Initialize window and OpenGL context
	int Init();
	virtual int OnInit() = 0;	// = 0 means that it's an abstract function
								// abstract functions must be filled in by child classes

	// Called every frame to perform calculations
	bool Tick();
	virtual bool OnTick() = 0;

	// Draws meshes to the front buffer
	void Draw();
	virtual void OnDraw() = 0;

	// Performs clean up and other deinitialization routines
	void Exit();
	virtual void OnExit() = 0;

	float windowWidth = 1280;
	float windowHeight = 720;
	
	float GetDeltaTime();
	float GetTimePassed();
	
	GLFWwindow *window;
};