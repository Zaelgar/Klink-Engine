#include "imgui/Inc/imgui.h"
#include <Klink/Inc/Klink.h>

#include "GameState.h"
#include "HullGenerationState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	KlinkApp::AddState<GameState>("GameState");
	KlinkApp::AddState<HullGenerationState>("HullGenerationState");
	KlinkApp::Run({ "Hello_Collision", 1600, 900 });
	return 0;
}