#include "imgui/Inc/imgui.h"
#include <Klink/Inc/Klink.h>

#include "GameState.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	KlinkApp::AddState<GameState>("GameState");
	KlinkApp::Run({ "Hello Linear Regression" });
	return 0;
}