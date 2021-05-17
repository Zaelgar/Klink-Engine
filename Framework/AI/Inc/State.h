#ifndef INCLUDED_KLINK_AI_STATE_H
#define INCLUDED_KLINK_AI_STATE_H

namespace Klink::AI
{

template <class AgentType>
class State
{
public:

	virtual ~State() = default;

	virtual void Enter(AgentType& agent) = 0;
	virtual void Update(AgentType& agent, float deltaTime) = 0;
	virtual void Exit(AgentType& agent) = 0;
};

} // namespace Klink::AI

#endif // INCLUDED_KLINK_AI_STATE_H