#pragma once

#include "Application.h"
#include "Camera.h"

#include "OpenGLInfo.h"

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_index_count;
};

class DemoApplication : public Application
{
	FlyCam myCam;
	OpenGLInfo temp;

	unsigned int fbo = 0;
	unsigned int fboTexture = 0;
	unsigned int fboDepth = 0;
	unsigned program;
	virtual int  OnInit() override;
	virtual bool OnTick() override;
	virtual void OnDraw() override;
	virtual void OnExit() override;

};
