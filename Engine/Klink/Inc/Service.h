#ifndef INCLUDED_KLINK_SERVICE_H
#define INCLUDED_KLINK_SERVICE_H

namespace Klink
{
	class World;

	class Service
	{
	public:
		META_CLASS_DECLARE;

		Service() = default;
		virtual ~Service() = default;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}

		World& GetWorld() { return *mWorld; }
		const World& GetWorld() const { return *mWorld; }

	private:
		friend class World;

		World* mWorld = nullptr;
		std::string mTypeId;
	};
}

#endif // INCLUDED_KLINK_SERVICE_H