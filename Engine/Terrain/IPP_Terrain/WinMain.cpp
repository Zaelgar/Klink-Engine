#include "imgui/Inc/imgui.h"
#include "Klink/Inc/Klink.h"

#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::JMath;
using namespace Klink::Input;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	KlinkApp::AddState<GameState>("GameState");
	KlinkApp::Run({ "Hello Lighting" });
	return 0;
}