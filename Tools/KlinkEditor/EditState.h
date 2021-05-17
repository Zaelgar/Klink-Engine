#ifndef INCLUDED_GAMESTATE_H
#define INCLUDED_GAMESTATE_H

#include <Klink/Inc/Klink.h>

class EditState : public Klink::AppState
{
public:

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:

	void ShowMenuBar();
	void ShowExampleMenuFile();
	void ShowMainWindowWithDockSpace();

	// TODO
	// EDIT STATE should render to a texture (like in post processing project)
	// and pass to ImGUI so you can have a window to display it


private:

	Klink::Graphics::Camera mCamera;

	Klink::World mWorld;
};
#endif // INCLUDED_EDITSTATE_H