#include "GameApp.h"

#include <External/ImGui/Inc/imgui.h>

using namespace Engine;

namespace
{
	float RandomFloat(float low, float high)
	{
		return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high - low)));
	}

	const float maxLifeTime = 2.0f;

	struct Particle
	{
		Math::Vector3 position;
		Math::Vector3 velocity;
		Math::Vector4 color;
		float lifeTime{ 0.0f };
	};

	std::vector<Particle> particles(1000);
	int nextParticle = 0;

	void SpawnParticle()
	{
		auto& p = particles[nextParticle];
		p.position = Math::Vector3{ 0.0f };
		p.velocity = Math::Vector3{
			RandomFloat(-1.0f, 1.0f),
			RandomFloat(-1.0f, 1.0f),
			RandomFloat(-1.0f, 1.0f)
		};
		p.lifeTime = maxLifeTime;
		nextParticle = (nextParticle + 1) % particles.size();
	}

	void UpdateParticles(float deltaTime)
	{
		for (auto& p : particles)
		{
			if (p.lifeTime > 0.0f)
			{
				p.lifeTime -= deltaTime;
				p.velocity.y += deltaTime;
				p.position += p.velocity * deltaTime;
			}
		}
	}

	Math::Vector4 startColor{ 0.8f, 0.8f, 0.0f, 1.0f };
	Math::Vector4 endColor{ 1.0f, 0.0f, 0.0f, 1.0f }; 
}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), GetAppName(), 1280, 720);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Graphics::SimpleDraw::Initialize(999999);
	Graphics::Gui::Initialize(mWindow.GetWindowHandle());
	
	mCamera.SetPosition({ 0.0f, 5.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f, 2.0f, 0.0f });

	mConstantBuffer.Initialize();

	mVertexShader.Initialize(L"../Assets/Shaders/Particle.fx", Graphics::VertexPNC::Format);
	mGeometryShader.Initialize(L"../Assets/Shaders/Particle.fx");
	mPixelShader.Initialize(L"../Assets/Shaders/Particle.fx");

	mBlendStates[0].Initialize(Graphics::BlendState::Mode::Opaque);
	mBlendStates[1].Initialize(Graphics::BlendState::Mode::AlphaBlend);
	mBlendStates[2].Initialize(Graphics::BlendState::Mode::AlphaPremultiplied);
	mBlendStates[3].Initialize(Graphics::BlendState::Mode::Additive);

	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mTexture.Initialize("../Assets/Images/flame.png");

	mMeshBuffer.Initialize(nullptr, sizeof(Graphics::VertexPNC), particles.size(), true);
}

void GameApp::OnTerminate()
{
	mMeshBuffer.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();
	for (auto& b : mBlendStates)
		b.Terminate();
	mPixelShader.Terminate();
	mGeometryShader.Terminate();
	mVertexShader.Terminate();
	mConstantBuffer.Terminate();

	Graphics::Gui::Terminate();
	Graphics::SimpleDraw::Terminate();
	Graphics::GraphicsSystem::StaticTerminate();
	Input::InputSystem::StaticTerminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	if (mWindow.ProcessMessage())
	{
		Kill();
		return;
	}

	auto inputSystem = Input::InputSystem::Get();
	inputSystem->Update();

	if (inputSystem->IsKeyPressed(Input::KeyCode::ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}

	static auto prevTime = std::chrono::high_resolution_clock::now();
	auto currTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime).count() / 1000.0f;
	prevTime = currTime;

	auto moveSpeed = inputSystem->IsKeyDown(Input::KeyCode::LSHIFT) ? 10.0f : 1.0f;
	auto turnSpeed = 90.0f;
	if (inputSystem->IsKeyDown(Input::KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	else if (inputSystem->IsKeyDown(Input::KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(Input::KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	else if (inputSystem->IsKeyDown(Input::KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}

	if (inputSystem->IsMouseDown(Input::MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	if (inputSystem->IsKeyDown(Input::KeyCode::SPACE) || true)
	{
		SpawnParticle();
	}

	UpdateParticles(deltaTime);

	Graphics::GraphicsSystem::Get()->BeginRender(Math::Vector4::Black());

	auto view = mCamera.GetViewMatrix();
	auto projection = mCamera.GetPerspectiveMatrix();

	ConstantData data;
	data.wvp = Math::Transpose(view * projection);
	data.viewPosition = { mCamera.GetPosition(), 1.0f };
	data.viewUp = Math::Vector3{ view._12, view._22, view._32 };

	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindGS();
	mConstantBuffer.BindPS();
	mVertexShader.Bind();
	mGeometryShader.Bind();
	mPixelShader.Bind();

	mBlendStates[mBlendStateSelected].Set();

	mSampler.BindPS(0);
	mTexture.BindPS(0);

	std::vector<Graphics::VertexPNC> vertices;
	vertices.reserve(particles.size());
	for (auto& p : particles)
	{
		if (p.lifeTime > 0.0f)
		{
			float size = 1.0f - p.lifeTime / maxLifeTime;

			Graphics::VertexPNC vertex;
			vertex.position = p.position;
			vertex.normal.x = size;
			vertex.normal.y = size;
			vertex.color = Math::Vector4{
				Math::Lerp(endColor.x, startColor.x, size),
				Math::Lerp(endColor.y, startColor.y, size),
				Math::Lerp(endColor.z, startColor.z, size),
				1.0f
			};
			vertices.push_back(vertex);
		}
	}
	std::sort(vertices.begin(), vertices.end(), [&](auto& a, auto& b)
	{
		auto s = Math::Dot(a.position, mCamera.GetDirection());
		auto t = Math::Dot(b.position, mCamera.GetDirection());
		return s > t;
	});
	mMeshBuffer.UpdateVertices(vertices.data(), vertices.size());
	mMeshBuffer.SetTopology(Graphics::MeshBuffer::Topology::PointList);
	mMeshBuffer.Render();

	mGeometryShader.UnBind();

	// Draw ground plane
	const float planeSize = 30.0f;
	for (float i = -planeSize; i <= planeSize; i += 1.0f)
	{
		Graphics::SimpleDraw::AddLine(-planeSize, 0.0f, i, planeSize, 0.0f, i, Math::Vector4::Gray());
		Graphics::SimpleDraw::AddLine(i, 0.0f, -planeSize, i, 0.0f, planeSize, Math::Vector4::Gray());
	}
	Graphics::SimpleDraw::AddTransform(Math::Matrix::Identity());

	Graphics::SimpleDraw::Render(mCamera);

	// Show FPS
	Graphics::Gui::BeginRender();
	ImGui::SetNextWindowPos({ 30.0f, 30.0f });
	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %f", 1.0f / deltaTime);
	ImGui::Text("Particles: %d", vertices.size());
	const char* blendItems[] = { "Opaque", "AlphaBlend", "AlphaPremultiplied", "Additive" };
	ImGui::Combo("Blend Mode", &mBlendStateSelected, blendItems, IM_ARRAYSIZE(blendItems));
	ImGui::ColorEdit4("Start Color", (float*)&startColor);
	ImGui::ColorEdit4("End Color", (float*)&endColor);
	ImGui::End();
	Graphics::Gui::EndRender();

	Graphics::GraphicsSystem::Get()->EndRender();
}
