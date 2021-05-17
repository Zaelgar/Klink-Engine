#include "Precompiled.h"
#include "Klink.h"
#include "App.h"

Klink::App Klink::sApp;

void KlinkApp::ChangeState(std::string name)
{
	Klink::sApp.ChangeState(std::move(name));
}

void KlinkApp::Run(Klink::AppConfig appConfig)
{
	Klink::sApp.Run(std::move(appConfig));
}

void KlinkApp::Shutdown()
{
	PostQuitMessage(0);
}