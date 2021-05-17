#ifndef INCLUDED_KLINK_AI_STEERINGMODULE_H
#define INCLUDED_KLINK_AI_STEERINGMODULE_H

#include "SteeringBehaviour.h"

namespace Klink::AI
{
enum class CalculationType {None, Prioritized, WeightedSum};

/*
struct SteeringBehaviourMapCompare
{
	bool operator() (SteeringBehaviour& lhs, SteeringBehaviour& rhs)
	{
		if (lhs.GetPriority() < rhs.GetPriority())
		{
			return true;
		}
		return false;
	}
};
*/

class SteeringModule
{
public:

	SteeringModule(Agent2D& agent);

	template<class BehaviourType>
	BehaviourType* AddBehaviour(std::string name, int priority);
	
	template<class BehaviourType>
	BehaviourType* GetBehaviour(std::string name);


	Klink::JMath::Vector2 Calculate();
	void SetCalculationType(Klink::AI::CalculationType calculationType) { mCalculationType = calculationType; }

private:

	using BehaviourMap = std::map<std::string, std::unique_ptr<SteeringBehaviour>>;	// used to sort map in order of priority

	Agent2D& mAgent;
	BehaviourMap mBehaviours;
	CalculationType mCalculationType = CalculationType::Prioritized;
};

template<class BehaviourType>
BehaviourType* SteeringModule::AddBehaviour(std::string name, int priority)
{
	auto[iter, success] = mBehaviours.try_emplace(std::move(name), std::make_unique<BehaviourType>());
	return success ? static_cast<BehaviourType*>(iter->second.get()) : nullptr;
}

template<class BehaviourType>
BehaviourType* SteeringModule::GetBehaviour(std::string name)
{
	auto iter = mBehaviours.find(name);
	return iter == mBehaviours.end() ? nullptr : iter->second.get();
}

}; // namespace AI

#endif // INCLUDED_KLINK_AI_STEERINGMODULE_H