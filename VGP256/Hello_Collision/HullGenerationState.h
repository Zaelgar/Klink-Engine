#ifndef INCLUDED_HULLGENERATIONSTATE_H
#define INCLUDED_HULLGENERATIONSTATE_H

#include <Klink/Inc/Klink.h>

class HullGenerationState : public Klink::AppState
{
public:

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:

	// General
	Klink::Graphics::Camera mCamera;
	std::vector<Klink::JMath::Vector2> mPoints;

	// Settings
	int pointCount = 30;
	Klink::JMath::Rect areaBoundary = Rect(200.f, 200.f, 1400.f, 700.f);

};
#endif // INCLUDED_HULLGENERATIONSTATE_H