#include <iostream>
#include "gl_core_4_4.h"
#include <glfw3.h>
#include "Gizmos.h"
#include <math.h>
#include <tiny_obj_loader.h>
#define GLM_SWIZZLE
//#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include <stb_image.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Vertex.h"
#include "Camera.h"



using glm::vec3;
using glm::vec4;
using glm::mat4;
float deltaTime = 0;
float timePassed = 0;
float fovFloat = 0.25f;
float * fovPointer = &fovFloat;
double xPos, yPos;
int windowWidth = 1280;
int windowHeight = 720;

Vertex testVert;

unsigned int programID;

std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;





//std::string err = tinyobj::LoadObj(shapes, materials, "./Objs/dragon.obj");

static void CursorPos(GLFWwindow*windowName, double xpos, double ypos)
{
	xPos = xpos;
	yPos = ypos;
	int click = glfwGetMouseButton(windowName, GLFW_MOUSE_BUTTON_LEFT);
	if (click == GLFW_PRESS)
	{
		std::cout << xPos << "     " << yPos << std::endl;
	}
	
}

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_index_count;
};

std::vector<OpenGLInfo> m_gl_info;

void createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes)
{

	m_gl_info.resize(shapes.size());

	for (unsigned int mesh_index = 0; mesh_index < shapes.size(); ++mesh_index)
	{
		unsigned int fCount = shapes[mesh_index].mesh.positions.size();

		glGenVertexArrays(1, &m_gl_info[mesh_index].m_VAO);
		glGenBuffers(1, &m_gl_info[mesh_index].m_VBO);
		glGenBuffers(1, &m_gl_info[mesh_index].m_IBO);
		glBindVertexArray(m_gl_info[mesh_index].m_VAO);
		unsigned int float_count = shapes[mesh_index].mesh.positions.size();
		float_count += shapes[mesh_index].mesh.normals.size();
		float_count += shapes[mesh_index].mesh.texcoords.size();
		std::vector<float> vertex_data;
		vertex_data.reserve(float_count);
		vertex_data.insert(vertex_data.end(),
			shapes[mesh_index].mesh.positions.begin(),
			shapes[mesh_index].mesh.positions.end());
		vertex_data.insert(vertex_data.end(),
			shapes[mesh_index].mesh.normals.begin(),
			shapes[mesh_index].mesh.normals.end());
		m_gl_info[mesh_index].m_index_count =
			shapes[mesh_index].mesh.indices.size();
		glBindBuffer(GL_ARRAY_BUFFER, m_gl_info[mesh_index].m_VBO);
		glBufferData(GL_ARRAY_BUFFER,
			vertex_data.size() * sizeof(float),
			vertex_data.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_info[mesh_index].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			shapes[mesh_index].mesh.indices.size() * sizeof(unsigned int),
			shapes[mesh_index].mesh.indices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normal data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0,
			(void*)(sizeof(float)*shapes[mesh_index].mesh.positions.size()));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	}
}


void MakeView(GLFWwindow* a, double deltaTime)
{
	glm::vec3 position = vec3(10, 10, 10);
	mat4 viewTransform = glm::lookAt(position, vec3(0), vec3(0, 1, 0));
	mat4 projectionTransform = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	mat4 worldTransform = glm::inverse(viewTransform);
	float speed = 1;
}


void GetDeltaTime()
{
	deltaTime = glfwGetTime();
	timePassed = glfwGetTime();
	glfwSetTime(0);

}
void ChangeFOV(GLFWwindow* windowName, float &FOV)
{
	if (glfwGetKey(windowName, 'Q') == GLFW_PRESS)
	{
		fovFloat -= 0.01f;
		FOV = glm::pi<float>() * (fovFloat);
	}
	if (glfwGetKey(windowName, 'E') == GLFW_PRESS)
	{
		fovFloat += 0.01f;
		FOV = glm::pi<float>() * (fovFloat);
	}
}


//
//int main()
//{
//	const int rows = 12;
//	const int cols = 12;
//
//	// Window Creation Code
//	if (glfwInit() == false)
//	{
//		return-1;
//	}
//
//	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "test", nullptr, nullptr);
//	if (window == nullptr) {
//		glfwTerminate();
//		return -2;
//	}
//	glfwMakeContextCurrent(window);
//	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
//		glfwDestroyWindow(window);
//		glfwTerminate();
//		return -3;
//	}
//	auto major = ogl_GetMajorVersion();
//	auto minor = ogl_GetMinorVersion();
//	printf("GL: %i.%i\n", major, minor);
//
//
//
//	//const char* vsSource = "#version 410\n  in vec4 Position;  in vec4 Colour; out vec4 vColour; uniform mat4 ProjectionView; uniform float time; uniform float heightScale; void main(){vColour = Colour; vec4 P = Position; P.y += cos(time + Position.x)*heightScale; gl_Position = ProjectionView*P; }";
//	//char* fsSource = "#version 410\n \in vec4 vColour; \ out vec4 FragColor; \  void main() {FragColor = vColour; FragColor *= vec4(0,.75,.5,1);}";
//
//
//// Loading a plane
//
//// Position     TexCoords
//// X, Y, Z, W,  S,T
//	
//			
//		/*float vertexData[] =  { -5, 0, 5, 1, 0,1,
//							   5, 0, 5, 1,  1,1,
//							   5, 0,-5, 1,  1,0,
//							  -5, 0,-5, 1,  0,0, };
//
//		*/
//
//	Vertex vertexData[] = 
//		{{{-5,0,5,1},  {0,1,0,0}, {1,0,0,0}, {0,1}},/* Vertex 1 */
//		 {{ 5,0,5,1},  {0,1,0,0}, {1,0,0,0}, {1,1}},
//		 {{ 5,0,-5,1}, {0,1,0,0}, {1,0,0,0}, {1,0}},
//		 {{-5,0,-5,1}, {0,1,0,0}, {1,0,0,0}, {0,0}}}; 
//
//		unsigned int indexData[] = { 0,1,2,
//									 0,2,3, };
//
//		unsigned int m_vao;
//		unsigned int m_ibo;
//		unsigned int m_vbo;
//
//		glGenVertexArrays(1, &m_vao);
//		glBindVertexArray(m_vao);
//		glGenBuffers(1, &m_vbo);
//
//		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4,
//			vertexData, GL_STATIC_DRAW);
//
//		glGenBuffers(1, &m_ibo);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,
//			indexData, GL_STATIC_DRAW);
//
//		// vec4 position
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
//			sizeof(Vertex), 0);
//
//		// vec2 TexCoord
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
//			sizeof(Vertex), (void*)(sizeof( glm::vec4 ) * 3));
//		
//		// vec4 normal
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)) );
//
//		// vec4 tangent
//		glEnableVertexAttribArray(3);
//		glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2) );
//
//		glBindVertexArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//
//
//
//		
//
//		//programID = loadShader("vertexpath", "fragmentpath");
//
//		unsigned int m_texture	= LoadTexture("./textures/rock_diffuse.tga");
//		unsigned int m_normal	= LoadTexture("./textures/rock_normal.tga");
//		unsigned int m_specular = LoadTexture("./textures/rock_specular.tga");
//		
//
//
//		RenderObject myFBX;
//		unsigned int fbxShaderProg = loadShader("testvert.txt", "testfrag.txt");
//		//myFBX = LoadFBX("./FBX/stanford/bunny.fbx");
//		//CreateOpenGLBuffers(myFBX.m_FBX);
//
//		Gizmos::create();
//
//
//
//
//		float time = 0;
//		glClearColor(0.25f, 0.25f, 0.25f, 1);
//		glEnable(GL_DEPTH_TEST);
//
//		
//		float initFOV = glm::pi<float>()*fovFloat;
//		
//
//		FlyCam planetView;
//		planetView.Init();
//		
//		planetView.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
//		planetView.setPosition(vec3(10, 10, 10));
//		
//
//		//setup and bind frame buffer
//		unsigned int fbo;
//		unsigned int fboTexture;
//
//		glGenFramebuffers(1, &fbo);
//		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//
//		//unbind FBO
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		glGenTextures(1, &fboTexture);
//		glBindTexture(GL_TEXTURE_2D, fboTexture);
//		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fboTexture, 0);
//
//
//		unsigned int fboDepth;
//		glGenRenderbuffers(1, &fboDepth);
//		glBindRenderbuffer(GL_RENDERBUFFER, fboDepth);
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
//		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboDepth);
//
//		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
//		glDrawBuffers(1, drawBuffers);
//
//		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//		if (status != GL_FRAMEBUFFER_COMPLETE)
//		{
//			printf("Framebuffer Error!\n");
//		}
//
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
//		{
//			GetDeltaTime();
//			glfwSetCursorPosCallback(window, CursorPos);
//
//			planetView.setPerspective(initFOV, 1280 / 720.f, 0.1f, 1000.f);
//			planetView.Move(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, deltaTime * 3);
//			planetView.Zoom();
//			ChangeFOV(window, initFOV);
//			
//		
//			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		
//			//time = glfwGetTime();
//
//			glUseProgram(fbxShaderProg);
//			
//			
//			//float timehaspassed = glGetUniformLocation(programID, "time");
//			//float height = glGetUniformLocation(programID, "heightScale");
//
//			//glUniform1f(timehaspassed, time);
//			//glUniform1f(height, 1);
//
//
//			//find a way to get these two lines  into init
//			
//			
//
//
//
//			//glBindVertexArray(grid.VAO);
//			//unsigned int indexCount = (rows - 1) * (cols - 1) * 6;
//			//glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
//			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//
//			//binding camera here
//			int loc = glGetUniformLocation(fbxShaderProg, "ProjectionView");
//			glUniformMatrix4fv(loc, 1, GL_FALSE, &(planetView.getProjectionView()[0][0]));
//
//
//			unsigned int bunnyCam = glGetUniformLocation(fbxShaderProg, "cameraPosition");
//			glUniform3fv(bunnyCam, 1, glm::value_ptr(planetView.getPosition()));
//
//			//telling shader where it is
//			loc = glGetUniformLocation(fbxShaderProg, "diffuseMap");
//			glUniform1i(loc, 0);
//			
//			loc = glGetUniformLocation(fbxShaderProg, "normalMap");
//			glUniform1i(loc, 1);
//
//			loc = glGetUniformLocation(fbxShaderProg, "specularMap");
//			glUniform1i(loc, 2);
//
//
//			loc = glGetUniformLocation(fbxShaderProg, "TOGGLE");
//			glUniform1i(loc, glfwGetKey(window, 'G'));
//
//			if (loc == GL_INVALID_VALUE) std::cout << "INVALID VALUE";
//			if (loc == GL_INVALID_OPERATION) std::cout << "INVALID OPERATION";
//
//			vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
//			loc = glGetUniformLocation(fbxShaderProg, "lightDirection");
//			glUniform3f(loc, light.x, light.y, light.z);
//
//			//setting texture slots
//			glActiveTexture(GL_TEXTURE0);
//			glBindTexture(GL_TEXTURE_2D, m_texture);
//
//			glActiveTexture(GL_TEXTURE1);
//			glBindTexture(GL_TEXTURE_2D, m_normal);
//
//			glActiveTexture(GL_TEXTURE2);
//			glBindTexture(GL_TEXTURE_2D, m_specular);
//
//
//			glBindVertexArray(m_vao);
//			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//
//
//			
//
//			//for (unsigned int i = 0; i < myFBX.m_FBX->getMeshCount(); ++i) {
//			//	FBXMeshNode* mesh = myFBX.m_FBX->getMeshByIndex(i);
//			//	unsigned int* glData = (unsigned int*)mesh->m_userData;
//			//	glBindVertexArray(glData[0]);
//			//	glDrawElements(GL_TRIANGLES,
//			//		(unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
//			//}
//
//			//myFBX.Draw();
//			
//
//
//			//myFBX2.Draw();
//
//
//
//			/*for (unsigned int i = 0; i < m_gl_info.size(); ++i)
//			{
//				glBindVertexArray(m_gl_info[i].m_VAO);
//				glDrawElements(GL_TRIANGLES, m_gl_info[i].m_index_count, GL_UNSIGNED_INT, 0);
//			}*/
//
//			/*Gizmos::clear();
//			Gizmos::addTransform(glm::mat4(1));
//			vec4 white(1);
//			vec4 black(0, 0, 0, 1);
//			for (int i = 0; i < 21; i++)
//			{
//				Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
//				Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
//			}
//
//			mat4 spin = glm::rotate(time /30,vec3(0,1,0));
//
//			Gizmos::addSphere(vec3(),1,24,24,vec4(255,205,0,1),&spin);
//
//			mat4 planet = glm::rotate(time/50, vec3(0, .8f, 0))
//						* glm::translate(vec3(5,0,0))
//						* glm::rotate(time/40, vec3(0, 1, 0));
//
//			Gizmos::addSphere(vec3(), .5, 24, 24, vec4(0, 56, 30, 1), &planet);
//
//
//			mat4 moon = glm::translate(vec3(planet[3].xyz))
//					  * glm::rotate(time/2, vec3(0, .8f, 0))
//					 * glm::translate(vec3(3, 0, 0))
//					  * glm::rotate(time, vec3(0, 1, 0));
//
//			Gizmos::addSphere(vec3(), .2, 24, 24, vec4(0, 0, 0, 1), &moon);
//
//			mat4 satellite = glm::translate(vec3(moon[3].xyz))
//				* glm::rotate(time/90, vec3(.25f, 0, 0))
//				*glm::translate(vec3(2, 1, 0));
//
//			Gizmos::addSphere(vec3(), .01, 10, 10, vec4(0, 200, 50, 1), &satellite);
//
//
//			Gizmos::draw(planetView.projectionTransform*planetView.getView());*/
//
//
//
//
//
//			glfwSwapBuffers(window);
//			glfwPollEvents();
//		}
//		Gizmos::destroy();
//		glfwTerminate();
//		glfwDestroyWindow(window);
//
//		return 0;
//	
//
//
//}

#include "DemoApp.h"

int main()
{
	Application *app = new DemoApplication();

	// if the application did not fail to initialize
	if (app->Init() == 0)
	{
		// WE GOOD, IT INITIALIZED FINE!
		std::cout << "we good" << std::endl;

		while (app->Tick())	// while tick returns true, keep the game running
		{
			app->Draw();
		}
	}
	else
	{
		// qq
		std::cout << "something fukt up" << std::endl;
	}

	app->Exit();

	delete app;
}