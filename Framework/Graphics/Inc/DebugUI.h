//====================================================================================================
// Filename:	Gui.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_GRAPHICS_DEBUGGUI_H
#define INCLUDED_GRAPHICS_DEBUGGUI_H

namespace Klink::Graphics::DebugUI
{

enum class Theme
{
	Dark,
	Classic,
	Light
};

void StaticInitialize(HWND window, bool docking, bool multiViewport);
void SetTheme(Theme theme);
void StaticTerminate();
void BeginRender();
void EndRender();

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_DEBUGGUI_H