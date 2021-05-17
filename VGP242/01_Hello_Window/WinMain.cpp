#include <Core/Inc/Core.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Klink::Core::Window myWindow;
	myWindow.Initialize(hInstance, "Hello Window", 1280, 720);

	bool done = false;
	while(!done)
	{
		done = myWindow.ProcessMessage();

		// Run Game Loop HERE
	}

	myWindow.Terminate();
	return 0;
}