#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <JMath/Inc/JMath.h>

#include <Graphics/Inc/VertexTypes.h>

#include <Graphics/Inc/ColourShader.h>
#include <Graphics/Inc/PixelShader.h>
#include <Graphics/Inc/Mesh.h>
#include <Graphics/Inc/MeshBuilder.h>

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
	myWindow.Initialize(hInstance, "Hello Sphere", screenW, screenH);

	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	MeshBuilder myMeshBuilder = {};

	/////////////////
	// Cube set up
	
	Mesh indexedCube = myMeshBuilder.CreateCube(1.0f, Vector4(0.5f, 0.5f, 0.5f, 1.0f));

	Mesh cylinder = myMeshBuilder.CreateCylinder(1.0f, 2.0f, 20, 10, Vector4(1.0f, 0.5f, 0.6f, 1.0f));

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
		
		//indexedCube.Render();
		cylinder.Render();
		colourShader.RenderV(cylinder.GetVertexCount(), matrices);
		//colourShader.Render(indexedCube.GetIndexCount(), matrices);


		GraphicsSystem::Get()->EndRender();
	}


	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}