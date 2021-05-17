#include "imgui/Inc/imgui.h"
#include "Klink/Inc/Klink.h"

#include "GameState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Klink::AppConfig SheepVsWolfApp;
	SheepVsWolfApp.appName = "Sheep!";
	SheepVsWolfApp.assetsDirectory = L"../../Assets/SheepVsWolf";
	SheepVsWolfApp.windowWidth = 1280;
	SheepVsWolfApp.windowHeight = 720;
	

	KlinkApp::AddState<GameState>("GameState");
	KlinkApp::Run(SheepVsWolfApp);
	return 0;
}