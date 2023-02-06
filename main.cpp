#include "main.hpp"
#include "application.hpp"
#include "system.hpp"

//
// Global Variables
//

Application::Application* App = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Create the app
	App = new Application::Application();
	
	App->SetInstance(hInstance);
	App->SetCmdLine(System::ConvertStringToWstring(std::string(lpCmdLine)));
	App->SetCmdShow(nShowCmd);
	App->SetAppName(L"Katip");

	// Initialize the app
	App->Initialize();

	// Run the app
	App->Run();

	delete App;
	
	return 0;
}