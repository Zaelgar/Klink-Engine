#include "imgui/Inc/imgui.h"
#include "Klink/Inc/Klink.h"

#include "GameState.h"

std::vector<Klink::JMath::Vector2> minerals;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	KlinkApp::AddState<GameState>("GameState");
	KlinkApp::Run({ "Hello Lighting" });
	return 0;
}