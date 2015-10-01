#include "DemoApp.h"

#include <iostream>

int DemoApplication::OnInit()
{
	std::cout << "DEMO APP INITIALIZED\n";
	return 0;
}

bool DemoApplication::OnTick()
{
	std::cout << "DEMO APP TICKING\n";
	return true;
}

void DemoApplication::OnDraw()
{
	std::cout << "DEMO APP DRAWING\n";
}

void DemoApplication::OnExit()
{
	std::cout << "DEMO APP EXIT\n";
}