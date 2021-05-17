#ifndef INCLUDED_HELLOSTEERING_FISH_H
#define INCLUDED_HELLOSTEERING_FISH_H

class Fish : public Agent2D
{
public:
	Fish(AIWorld& world);

	void Initialize();
	void Terminate();

	void Update(float deltaTime, bool b);
	void Render(Klink::Graphics::Colours::Colour color = Klink::Graphics::Colours::Magenta);

	std::unique_ptr<Klink::AI::SteeringModule> steeringModule;

	Klink::JMath::Triangle mTriangle;

	Vector2 RandomPositionOnScreen();
};

#endif // INCLUDED_HELLOSTEERING_FISH_H