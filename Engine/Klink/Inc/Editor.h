#ifndef INCLUDED_KLINK_EDITOR_H
#define INCLUDED_KLINK_EDITOR_H

namespace Klink
{
	class World;

	class Editor
	{
	public:
		Editor(World& world);

		void ShowWorldView();
		void ShowInspectorView();

	private:
		World& mWorld;
		//GameObject* mSelectedGameObject = nullptr;
	};
}

#endif // INCLUDED_KLINK_EDITOR_H