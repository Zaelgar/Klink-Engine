#include "imgui/Inc/imgui.h"
#include "Klink/Inc/Klink.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::JMath;
using namespace Klink::Input;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	int screenW = 1280;
	int screenH = 720;

	float aspect = (float)screenW / (float)screenH;

	// Setup window, initialise the graphics system
	Window myWindow;
	myWindow.Initialize(hInstance, "Hello SimpleDraw", screenW, screenH);

	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);
	SimpleDraw::StaticInitialize();
	InputSystem::StaticInitialize(myWindow.GetWindowHandle());
	DebugUI::StaticInitialize(myWindow.GetWindowHandle());

	Camera camera{};


	// Render Cube Initialization
	////////////////////////////////////////////
	
	MeshBuffer meshBuffer{};
	Texture texture{};
	ConstantBuffer constantBuffer{};
	VertexShader vertexShader{};
	PixelShader pixelShader{};

	Mesh<VertexPN> cubeMesh = MeshBuilder::CreatePNSphere(2.0f, 25, 25);
	meshBuffer.Initialize((int)cubeMesh.mVertices.size(), sizeof(VertexPN), cubeMesh.mVertices.data(), (int)cubeMesh.mIndices.size(), cubeMesh.mIndices.data());

	Matrix4 mTransform[3]{};
	mTransform[0] = Identity();

	texture.Initialize(L"../Assets/earth.jpg");
	constantBuffer.Initialize(sizeof(Matrix4));
	vertexShader.Initialize("../Assets/Shaders/DoLighting.fx", VertexPX::Format);
	pixelShader.Initialize("../Assets/Shaders/DoLighting.fx");
	
	////////////////////////////////////////////


	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();
		GraphicsSystem::Get()->BeginRender();


		// Render Cube
		////////////////////////////////////////////

		mTransform[1] = Transpose(camera.GetViewMatrix());
		mTransform[2] = Transpose(camera.GetPerspectiveMatrix());		

		texture.Bind();
		constantBuffer.Bind(mTransform);
		vertexShader.Bind();
		pixelShader.Bind();

		meshBuffer.Bind();
		meshBuffer.Render();

		////////////////////////////////////////////


		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			done = true;
		}

		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000;
		lastTime = currentTime;

		float moveSpeed = 0.05f;
		const float turnSpeed = 0.006f;
		if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
		{
			moveSpeed *= 3;
		}
		if (inputSystem->IsKeyDown(KeyCode::Q))
		{
			camera.Rise(moveSpeed);
		}
		if (inputSystem->IsKeyDown(KeyCode::E))
		{
			camera.Rise(-moveSpeed);
		}
		if (inputSystem->IsKeyDown(KeyCode::W))
		{
			camera.Walk(moveSpeed);
		}
		if (inputSystem->IsKeyDown(KeyCode::S))
		{
			camera.Walk(-moveSpeed);
		}
		if (inputSystem->IsKeyDown(KeyCode::A))
		{
			camera.Strafe(moveSpeed);
		}
		if (inputSystem->IsKeyDown(KeyCode::D))
		{
			camera.Strafe(-moveSpeed);
		}
		if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
		{
			camera.Yaw(inputSystem->GetMouseMoveX() * -turnSpeed);
			camera.Pitch(inputSystem->GetMouseMoveY() * -turnSpeed);
		}
		moveSpeed = 0.01f;

		//SimpleDraw::DrawCube({10.0f, 0.0f, 15.0f}, 10.0f, Colours::Aquamarine);
		//SimpleDraw::DrawSquare({}, 3.0f, Colours::Crimson);
		//SimpleDraw::DrawCircle({}, 5.0f, 16);
		//SimpleDraw::DrawSphere({-10.0f, 0.0f, 0.0f}, 3.0f);

		SimpleDraw::DrawDebugGrid();
		SimpleDraw::Render(camera);
		
		DebugUI::BeginRender();
		Matrix4 test = camera.GetViewMatrix();
		ImGui::Text
		(
			"_11: %9f \t_12: %f \t_13 %f \t_14 %f\n_21: %f \t_22: %f \t_23 %f \t_24 %f\n_31: %f \t_32: %f \t_33 %f \t_34 %f\n_41: %f \t_42: %f \t_43 %f \t_44 %f",
			test._11, test._12, test._13, test._14,
			test._21, test._22, test._23, test._24,
			test._31, test._32, test._33, test._34,
			test._41, test._42, test._43, test._44
		);
		DebugUI::EndRender();
		GraphicsSystem::Get()->EndRender();
	}

	//Mesh Terminate
	/////////

	meshBuffer.Terminate();
	texture.Terminate();
	constantBuffer.Terminate();
	vertexShader.Terminate();
	pixelShader.Terminate();

	/////////

	InputSystem::StaticTerminate();
	SimpleDraw::StaticTerminate();
	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}