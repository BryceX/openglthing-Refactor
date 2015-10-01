#pragma once

#include "Application.h"

class DemoApplication : public Application
{
	virtual int  OnInit() override;
	virtual bool OnTick() override;
	virtual void OnDraw() override;
	virtual void OnExit() override;
};
