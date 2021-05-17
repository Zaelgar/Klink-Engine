#pragma once

#include "imgui.h"

struct AppLog
{
	ImGuiTextBuffer textBuffer;
	bool scrollToBottom;

	void Clear()
	{
		textBuffer.clear();
	}

	void AddLog(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		textBuffer.appendfv(fmt, args);
		va_end(args);
		scrollToBottom = true;
	}

	void Draw(const char* title, bool* p_opened = nullptr)
	{
		ImGui::Begin(title, p_opened);
		ImGui::TextUnformatted(textBuffer.begin());
		if (scrollToBottom)
			ImGui::SetScrollHere(1.0f);
		scrollToBottom = false;
		ImGui::End();
	}
};