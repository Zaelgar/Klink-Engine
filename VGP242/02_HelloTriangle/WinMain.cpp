#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <JMath/Inc/JMath.h>

#include <Graphics/Inc/VertexTypes.h>


#include <Graphics/Inc/ColourShader.h>
#include <Graphics/Inc/PixelShader.h>

#include <Graphics/Inc/Mesh.h>

using namespace Core;
using namespace Graphics;
using namespace JMath;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Setup window, initialise the graphics system
	Klink::Core::Window myWindow;
	myWindow.Initialize(hInstance, "Hello Triangle", 1280, 720);

	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	//////////////////

	// Define 3 vertices for rendering triangle
	Vertex vertices[] =
	{
		{{0.0f, 0.5f, 0.0f}, {0.5f, 0.1f, 0.5f, 1.0f}},
		{{0.5f, 0.0f, 0.0f}, {0.5f, 0.1f, 0.5f, 1.0f}},
		{{-0.5f, 0.0f, 0.0f}, {0.5f, 0.1f, 0.5f, 1.0f}}
	};

	Mesh* triangleMeshBuffer = new Mesh();
	triangleMeshBuffer->Initialise(vertices, sizeof(Vertex), 3);

	/////////////////

	// Define 18 Vertices for rendering a "cube"
	Vertex cubeVertices[] =
	{
		{{0.5f, 0.6f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}},
		{{0.1f, 0.4f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}},
		{{0.2f, 0.6f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}},

		{{0.5f, 0.6f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}},
		{{0.4f, 0.4f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}},
		{{0.1f, 0.4f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}},

		{{0.5f, 0.6f, 0.0f}, {0.3f, 0.3f, 0.3f, 1.0f}},
		{{0.4f, 0.1f, 0.0f}, {0.3f, 0.3f, 0.3f, 1.0f}},
		{{0.4f, 0.4f, 0.0f}, {0.3f, 0.3f, 0.3f, 1.0f}},

		{{0.5f, 0.6f, 0.0f}, {0.3f, 0.3f, 0.3f, 1.0f}},
		{{0.5f, 0.3f, 0.0f}, {0.3f, 0.3f, 0.3f, 1.0f}},
		{{0.4f, 0.1f, 0.0f}, {0.3f, 0.3f, 0.3f, 1.0f}},

		{{0.1f, 0.4f, 0.0f}, {0.8f, 0.8f, 0.8f, 1.0f}},
		{{0.4f, 0.4f, 0.0f}, {0.8f, 0.8f, 0.8f, 1.0f}},
		{{0.1f, 0.1f, 0.0f}, {0.8f, 0.8f, 0.8f, 1.0f}},

		{{0.4f, 0.4f, 0.0f}, {0.8f, 0.8f, 0.8f, 1.0f}},
		{{0.4f, 0.1f, 0.0f}, {0.8f, 0.8f, 0.8f, 1.0f}},
		{{0.1f, 0.1f, 0.0f}, {0.8f, 0.8f, 0.8f, 1.0f}}
	};

	Mesh* cubeMeshBuffer = new Mesh();
	cubeMeshBuffer->Initialise(cubeVertices, sizeof(Vertex), 18);

	/////////////////

	// Define 18 Vertices for rendering a "cube" but with seperated triangle colours for each surface
	Vertex cube2Vertices[] =
	{
		{{0.5f, 0.6f, 0.0f}, {0.3f, 0.5f, 0.5f, 1.0f}},
		{{0.1f, 0.4f, 0.0f}, {0.3f, 0.5f, 0.5f, 1.0f}},
		{{0.2f, 0.6f, 0.0f}, {0.3f, 0.5f, 0.5f, 1.0f}},

		{{0.5f, 0.6f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}},
		{{0.4f, 0.4f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}},
		{{0.1f, 0.4f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}},

		{{0.5f, 0.6f, 0.0f}, {0.1f, 0.3f, 0.3f, 1.0f}},
		{{0.4f, 0.1f, 0.0f}, {0.1f, 0.3f, 0.3f, 1.0f}},
		{{0.4f, 0.4f, 0.0f}, {0.1f, 0.3f, 0.3f, 1.0f}},

		{{0.5f, 0.6f, 0.0f}, {0.3f, 0.3f, 0.3f, 1.0f}},
		{{0.5f, 0.3f, 0.0f}, {0.3f, 0.3f, 0.3f, 1.0f}},
		{{0.4f, 0.1f, 0.0f}, {0.3f, 0.3f, 0.3f, 1.0f}},

		{{0.1f, 0.4f, 0.0f}, {0.7f, 0.8f, 0.8f, 1.0f}},
		{{0.4f, 0.4f, 0.0f}, {0.7f, 0.8f, 0.8f, 1.0f}},
		{{0.1f, 0.1f, 0.0f}, {0.7f, 0.8f, 0.8f, 1.0f}},

		{{0.4f, 0.4f, 0.0f}, {0.8f, 0.8f, 0.8f, 1.0f}},
		{{0.4f, 0.1f, 0.0f}, {0.8f, 0.8f, 0.8f, 1.0f}},
		{{0.1f, 0.1f, 0.0f}, {0.8f, 0.8f, 0.8f, 1.0f}}
	};

	Mesh* cube2MeshBuffer = new Mesh();
	cube2MeshBuffer->Initialise(cube2Vertices, sizeof(Vertex), 18);

	/////////////////

	Vertex indexedVerts[] =
	{
		{{0.2f, 0.6f, 0.0f}, {0.3f, 0.5f, 0.5f, 1.0f}},	// 0
		{{0.5f, 0.6f, 0.0f}, {0.3f, 0.5f, 0.5f, 1.0f}}, // 1
		{{0.1f, 0.4f, 0.0f}, {0.3f, 0.5f, 0.5f, 1.0f}}, // 2
		{{0.4f, 0.4f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}}, // 3
		{{0.5f, 0.3f, 0.0f}, {0.3f, 0.3f, 0.3f, 1.0f}}, // 4
		{{0.1f, 0.1f, 0.0f}, {0.7f, 0.8f, 0.8f, 1.0f}}, // 5
		{{0.4f, 0.1f, 0.0f}, {0.8f, 0.8f, 0.8f, 1.0f}}  // 6
	};

	int index[] =
	{// Tri 1  Tri 2   Tri 3  Tri 4   Tri 5  Tri 6
		0,1,2, 1,3,2,  1,6,3, 1,4,6,  2,3,5, 3,6,5
	};

	Mesh* indexedCube = new Mesh();
	indexedCube->Initialise(indexedVerts, sizeof(Vertex), 7, index, 18);

	/////////////////

	ColourShader vertexShader;
	vertexShader.Initialize();

	PixelShader pixelShader;
	pixelShader.Initialise();

	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();

		const FLOAT clearColour[4]{ 0.2f,0.2f,0.2f,1.0f };

		GraphicsSystem::Get()->BeginRender(clearColour);

		vertexShader.Set();
		pixelShader.Set();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}
		else if (GetAsyncKeyState(VK_NUMPAD1))
		{
			triangleMeshBuffer->Render();
		}
		else if (GetAsyncKeyState(VK_NUMPAD2))
		{
			cubeMeshBuffer->Render();
		}
		else if (GetAsyncKeyState(VK_NUMPAD3))
		{
			cube2MeshBuffer->Render();
		}
		else if (GetAsyncKeyState(VK_NUMPAD4))
		{
			indexedCube->Render();
		}

		GraphicsSystem::Get()->EndRender();
	}

	
	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}