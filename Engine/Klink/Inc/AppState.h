#ifndef INCLUDED_KLINK_APPSTATE_H
#define INCLUDED_KLINK_APPSTATE_H
namespace Klink
{
	class AppState
	{
	public:

		virtual void Initialize() = 0;
		virtual void Terminate() = 0;

		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;
		virtual void DebugUI() = 0;
	};
};
#endif // INCLUDED_KLINK_APPSTATE_H