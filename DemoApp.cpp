#include "gl_core_4_4.h"
#include "DemoApp.h"
//libraries

#include <glfw3.h>
#include "gizmos.h"






int DemoApplication::OnInit()
{
	std::cout << "DEMO APP INITIALIZED\n";

	
	// framebuffer generation
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fboTexture, 0);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glGenRenderbuffers(1, &fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, fboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboDepth);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// mesh data generation

	float vertexData[] = 
	{
		// Position			UV Coords
		//	X,  Y,  Z,  W, | S, T
			-5, 0,  -5,  1,  0, 0,
			5,  0,  -5,  1,  1, 0,
			5,  10, -5,  1,  1, 1,
			-5, 10, -5,  1,  0, 1,
	};

	unsigned int indexData[] =
	{
		0,1,2,
		0,2,3,
	};

	// VAO-begin - will store handles to the VBO and IBO and make them available when bound
	glGenVertexArrays(1, &temp.m_VAO);
	glBindVertexArray(temp.m_VAO);

	// VBO - describes vertex data
	glGenBuffers(1, &temp.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, temp.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 4));

	// IBO - order to draw verticies
	glGenBuffers(1, &temp.m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

	// VAO-end
	glBindVertexArray(0);						// VAO comes first! important!
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// shader program generation
	// TODO: load these shaders from file

	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position;\
							layout(location=1) in vec2 TexCoord;\
							out vec2 vTexCoord;\
							uniform mat4 ProjectionView;\
							void main()\
							{\
								vTexCoord = TexCoord;\
								gl_Position = ProjectionView * Position;\
							}";

	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord;\
							out vec4 FragColor;\
							uniform sampler2D diffuse;\
							void main()\
							{\
							FragColor = texture(diffuse, vTexCoord);\
							}";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// check if the shader compiled correctly
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
	{
		// oh shit son
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(program, 1024, &log_length, message);
		printf(message);
	}
	else
	{
		// " fuck witches get bitches " - abrham meguca
		printf("SHADER OK!\n");
	}


	// post OpenGL-init logic
	Gizmos::create();

	myCam.Init();
	myCam.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	myCam.setPosition(vec3(10, 10, 10));
	myCam.setPerspective(glm::pi<float>()*0.25f, 1280 / 720.f, 0.1f, 1000.f);
	
	return 0;
}

bool DemoApplication::OnTick()
{
	std::cout << "dTime:" << GetDeltaTime() << std::endl;
	std::cout << "pTime:" << GetTimePassed() << std::endl;

	std::cout << "DEMO APP TICKING\n";

	Gizmos::clear();

	Gizmos::addSphere(vec3(0, 0, 0), 0.5f, 8, 8, glm::vec4(1, 1, 0, 1));

	return true;
}

void DemoApplication::OnDraw()
{
	//std::cout << "itstarted" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);				// FBO is bound
	glViewport(0, 0, 512, 512);	// defines the resolution of the camera, essentially
	glClearColor(0.75f, 0.75f, 0.75f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clears the FBO

		Gizmos::draw(myCam.getProjectionView());			// draw gizmos

	glBindFramebuffer(GL_FRAMEBUFFER, 0);				// FBO is unbound, frontbuffer is now bound
	glViewport(0, 0, 1280, 720); // return it to our window's resolution
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the front buffer
	
	// draw the quad with the framebuffer's texture on it
	glUseProgram(program);
	int loc = glGetUniformLocation(program, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(myCam.getProjectionView()[0][0]));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	loc = glGetUniformLocation(program, "diffuse");
	glUniform1i(loc, 0);

	glBindVertexArray(temp.m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	//Gizmos::draw(myCam.getProjectionView());			// draw gizmos

	std::cout << "DEMO APP DRAWING\n";
}

void DemoApplication::OnExit()
{
	std::cout << "DEMO APP EXIT\n";
}