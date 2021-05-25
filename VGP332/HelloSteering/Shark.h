#ifndef INCLUDED_HELLOSTEERING_SHARK_H
#define INCLUDED_HELLOSTEERING_SHARK_H

#include <AI\Inc\Agent.h>

class Shark : public Agent2D
{
public:
	Shark(AIWorld& world);

	void Initialize();
	void Terminate();

	void Update(float deltaTime);
	void Render(Klink::Graphics::Colours::Colour color = Klink::Graphics::Colours::Magenta);

	std::unique_ptr<Klink::AI::SteeringModule> steeringModule;
};

#endif // INCLUDED_HELLOSTEERING_SHARK_H