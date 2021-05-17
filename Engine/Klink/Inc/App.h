#ifndef INCLUDED_KLINK_APP_H
#define INCLUDED_KLINK_APP_H

namespace Klink
{
	class AppState;

	struct AppConfig
	{
		AppConfig() = default;
		AppConfig(std::string name)
			:appName(std::move(name))
		{}
		AppConfig(std::string name, uint32_t windowWidth, uint32_t windowHeight)
			:appName(std::move(name))
		{
			if (windowWidth < 800 || windowWidth > 1920)
				mWindowWidth = 1280;
			else
				mWindowWidth = windowWidth;
			if (windowHeight < 600 || windowHeight > 1080)
				mWindowHeight = 720;
			else
				mWindowHeight = windowHeight;
		}


		bool mMaximized = false;
		bool mImGuiDocking = false;

		std::string appName = "Klink NO_NAME";
		std::filesystem::path assetsDirectory = L"../../Assets";
		uint32_t mWindowWidth = 1280;
		uint32_t mWindowHeight = 720;
	};

class App
{
public:

	void Run(AppConfig appConfig);

	template <class StateType>
	void AddState(std::string name);

	void ChangeState(std::string name);

private:

	AppConfig mAppConfig;
	Core::Window mWindow;
	std::map<std::string, std::unique_ptr<AppState>> mAppStates;
	AppState* mCurrentState = nullptr;
	AppState* mNextState = nullptr;
};

template <class StateType>
void App::AddState(std::string name)
{
	mAppStates.emplace(std::move(name), std::make_unique<StateType>());
}

};

#endif // INCLUDED_KLINK_APP_H