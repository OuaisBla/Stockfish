// EvalViewer.cpp : main project file.

#include "stdafx.h"
#include "MainForm.h"
#include "EngineInit.h"

using namespace EvalViewer;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

  EngineInit();

	// Create the main window and run it
	Application::Run(gcnew EvalGraph());

  EngineReset();

	return 0;
}
