#include "Precompiled.h"
#include "Editor.h"

#include "Component.h"
#include "GameObject.h"
#include "World.h"

using namespace Klink;

Editor::Editor(World& world)
	: mWorld(world)
{}

void Editor::ShowWorldView()
{
	ImGui::Begin("World");
	if (ImGui::TreeNode("Services"))
	{
		ImGui::Selectable("Targeting", false);
		ImGui::Selectable("Navigation", false);
		ImGui::Selectable("Physics", false);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Cameras"))
	{
		ImGui::Selectable("Main", false);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("GameObjects"))
	{
		const size_t objectCount = mWorld.mUpdateList.size();
		for (auto gameObject : mWorld.mUpdateList)
		{
			/*
			if (ImGui::Selectable(gameObject->GetName().c_str(), gameObject == mSelectedGameObject))
			{
				mSelectedGameObject = gameObject;
			}
			*/
		}
	}
}

void Editor::ShowInspectorView()
{

}