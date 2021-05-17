#include "imgui/Inc/imgui.h"
#include <Klink/Inc/Klink.h>

#include "GameState.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
	KlinkApp::AddState<GameState>("GameState");
	KlinkApp::Run({ "Hello Travelling Salesman" });
	return 0;
}