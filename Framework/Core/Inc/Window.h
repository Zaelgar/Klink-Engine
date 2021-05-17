#ifndef INCLUDED_KLINK_CORE_WINDOW_H
#define INCLUDED_KLINK_CORE_WINDOW_H

namespace Klink::Core
{

class Window
{
public:
	Window() = default;
	~Window() = default;

	void Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height, bool isMaximized = false);
	void Terminate();

	bool ProcessMessage();

	HWND GetWindowHandle() const { return mWindow; }

private:
	// Default member initialisation
	HINSTANCE mInstance{ nullptr };
	HWND mWindow{ nullptr };
	std::string mAppName;
}; // Class Window

}	// namespace Klink

#endif	// ifndef INCLUDED_CORE_WINDOW_H