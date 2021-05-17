#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <JMath/Inc/EngineMath.h>

#include <Graphics/Inc/VertexTypes.h>

#include <Graphics/Inc/ColourShader.h>
#include <Graphics/Inc/PixelShader.h>
#include <Graphics/Inc/Mesh.h>

#include <Graphics/Inc/Camera.h>

using namespace Core;
using namespace Graphics;
using namespace JMath;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	int screenW = 1280;
	int screenH = 720;

	float aspect = (float)screenW / (float)screenH;

	// Setup window, initialise the graphics system
	Klink::Core::Window myWindow;
	myWindow.Initialize(hInstance, "Hello Cube", screenW, screenH);

	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	/////////////////
	// Cube set up
	Vertex cubeVertices[] =
	{
		{{-0.5f, 0.5f, 0.5f},  {1.0f, 0.3f, 0.3f, 1.0f}},	// 0
		{{0.5f, 0.5f, 0.5f},   {0.8f, 0.4f, 0.6f, 1.0f}},	// 1
		{{-0.5f, 0.5f, -0.5f}, {0.7f, 0.5f, 0.5f, 1.0f}},	// 2
		{{0.5f, 0.5f, -0.5f},  {0.6f, 0.6f, 0.9f, 1.0f}},	// 3
		{{-0.5f, -0.5f, 0.5f}, {0.5f, 0.7f, 0.5f, 1.0f}},	// 4
		{{0.5f, -0.5f, 0.5f},  {0.4f, 0.8f, 0.1f, 1.0f}},	// 5
		{{-0.5f, -0.5f, -0.5f},{0.3f, 0.9f, 0.6f, 1.0f}},	// 6
		{{0.5f, -0.5f, -0.5f}, {0.2f, 1.0f, 0.7f, 1.0f}},	// 7
	};

	int index[] =
	{// top 1    top 2    rightside1  rightside2  front1    front 2
		0,1,2,   1,3,2,   3,1,7,      1,5,7,      2,3,6,    3,7,6,

		// left1    left2    back1     back2     bottom1   bottom2
		   0,2,6,   0,6,4,   1,0,4,    1,4,5,    7,5,4,    6,7,4
	};

	Mesh indexedCube{};
	indexedCube.Initialise(cubeVertices, sizeof(Vertex), 8, index, 36);

	// NOTE: constant buffer takes the matrices transposed!!
	Matrix4 matrices[3];
	matrices[0] = Identity();

	ColourShader colourShader;
	/////////////////

	colourShader.Initialize();
	Camera camera(Vector3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), 0.1f, 1000.0f, 60.0f, screenH, screenW);

	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}

		const FLOAT clearColour[4]{ 0.2f,0.2f,0.2f,1.0f };
		GraphicsSystem::Get()->BeginRender(clearColour);

		matrices[0] = matrices[0] * RotationY(0.01f) * RotationX(0.005f);
		matrices[1] = camera.GetViewMatrix();
		matrices[2] = camera.GetPerspectiveMatrix();

		colourShader.Render(matrices);
		colourShader.Bind();
		indexedCube.Render();
		
		GraphicsSystem::Get()->EndRender();
	}


	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}