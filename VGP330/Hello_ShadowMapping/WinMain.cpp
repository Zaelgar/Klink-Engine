#include "imgui/Inc/imgui.h"
#include "Klink/Inc/Klink.h"

#include "GameState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	KlinkApp::AddState<GameState>("GameState");
	KlinkApp::Run({ "Hello ShadowMapping" });
	return 0;
}