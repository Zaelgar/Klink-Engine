#include "imgui/Inc/imgui.h"
#include <Klink/Inc/Klink.h>

#include "EditState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Klink::AppConfig appConfig;
	appConfig.appName = "Klink Editor";
	appConfig.assetsDirectory = "./";
	appConfig.mMaximized = true;
	appConfig.mWindowHeight = 900;
	appConfig.mWindowWidth = 1600;
	appConfig.mImGuiDocking = true;

	KlinkApp::AddState<EditState>("EditState");
	KlinkApp::Run(appConfig);
	return 0;
}