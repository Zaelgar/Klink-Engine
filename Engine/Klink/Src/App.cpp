#include "Precompiled.h"
#include "AppState.h"

#include "App.h"
#include "MetaRegistration.h"

using namespace Klink;
using namespace Klink::Input;
using namespace Klink::Graphics;

void App::ChangeState(std::string name)
{
	if (auto iter = mAppStates.find(name); iter != mAppStates.end())
		mNextState = iter->second.get();
}

void App::Run(AppConfig appConfig)
{
	LOG("Static Meta Registers...");
	Core::StaticMetaRegister();
	JMath::StaticMetaRegister();
	Klink::StaticMetaRegister();

	LOG("Seeding random function...");
	srand(static_cast<unsigned int>(time(NULL)));

	mAppConfig = appConfig;

	LOG("Creating Window...");
	mWindow.Initialize(
		GetModuleHandle(NULL),
		appConfig.appName.c_str(),
		appConfig.mWindowWidth,
		appConfig.mWindowHeight,
		appConfig.mMaximized);

	/// Static systems initialization
	LOG("Static systems initialization...");
	Timer::StaticInitialize();
	InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	TextureManager::StaticInitialize(appConfig.assetsDirectory);
	SamplerManager::StaticInitialize();
	SpriteRenderer::StaticInitialize();
	BlendStateManager::StaticInitialize();
	RasterizerStateManager::StaticInitialize();
	ShaderManager::StaticInitialize();

	SimpleDraw::StaticInitialize(50000);
	DebugUI::StaticInitialize(mWindow.GetWindowHandle(), appConfig.mImGuiDocking, true);

	Timer timer{};
	timer.Initialize();

	LOG("App states initializing...");
	mCurrentState = mAppStates.begin()->second.get();
	mCurrentState->Initialize();

	/// LOOP
	bool done = false;
	while (!done)
	{
		done = mWindow.ProcessMessage();
		if (done)
		{
			break;
		}

		if (mNextState)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		timer.Update();

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			done = true;
		}

		mCurrentState->Update(timer.GetDeltaTime());

		GraphicsSystem::Get()->BeginRender();

		mCurrentState->Render();

		DebugUI::BeginRender();
		mCurrentState->DebugUI();
		DebugUI::EndRender();

		GraphicsSystem::Get()->EndRender();
	}

	mCurrentState->Terminate();

	DebugUI::StaticTerminate();
	SimpleDraw::StaticTerminate();
	ShaderManager::StaticTerminate();
	RasterizerStateManager::StaticTerminate();
	BlendStateManager::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	SamplerManager::StaticTerminate();
	TextureManager::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();
	Timer::StaticTerminate();

	mWindow.Terminate();
}