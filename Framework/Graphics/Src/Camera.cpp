#include "Precompiled.h"
#include "Camera.h"

#include "GraphicsSystem.h"

using namespace Klink::Graphics;
using namespace Klink::JMath;

Matrix4 Klink::Graphics::ComputeViewMatrix(const Vector3 right, const Vector3 up, const Vector3 look, const Vector3 position)
{
	const float dx = -Dot(position, right);
	const float dy = -Dot(position, up);
	const float dz = -Dot(position, look);

	return {
		right.x, up.x, look.x, 0.0f,
		right.y, up.y, look.y, 0.0f,
		right.z, up.z, look.z, 0.0f,
		dx     , dy  , dz    , 1.0f
	};
}

Matrix4 Klink::Graphics::ComputePerspectiveMatrix(float n, float f, float fov, float aspectRatio)
{
	const float h = 1 / tan(fov * 0.5f);
	const float w = h / aspectRatio;
	const float zf = f;
	const float zn = n;
	const float d = zf / (zf - zn);

	return {
		w,		0.0f,	0.0f,		0.0f,
		0.0f,	h,		0.0f,		0.0f,
		0.0f,	0.0f,	d,			1.0f,
		0.0f,	0.0f,	-zn * d,	0.0f
	};
}

Camera::Camera(Vector3 position, Vector3 rotation, Vector3 lookAt, float nearPlane, float farPlane, float FoV, int screenHeight, int screenWidth)
	: mPosition(position),
	mDirection(rotation),
	mNearPlane(nearPlane),
	mFarPlane(farPlane),
	mFOV(FoV*DegreesToRad)
{
	float aspect = (float)screenWidth / (float)screenHeight;
	ComputePerspectiveMatrix(mNearPlane, mFarPlane, mFOV, aspect);
}

void Camera::ResizeScreen(uint32_t width, uint32_t height)
{
	float aspect = (float)width / (float)height;
	ComputePerspectiveMatrix(mNearPlane, mFarPlane, mFOV, aspect);
}

void Camera::SetPosition(const Vector3& position)
{
	mPosition = position;
}

void Camera::SetDirection(const Vector3& direction)
{
	ASSERT(direction.Magnitude() != 0, "Direction must have a direction");
	mDirection = direction;
}

void Camera::SetTarget(const Vector3& target)
{
	mDirection = (target - mPosition).Normalized();
}

void Camera:: SetFov(float angle, bool rad)
{
	mFOV = !rad ? angle * Constants::DegreesToRad : angle;
}

Matrix4 Camera::GetViewMatrix() const
{
	const Vector3 look = mDirection.Normalized();
	const Vector3 right = Normalize(Cross({ 0.0f, 1.0f, 0.0f }, look));
	const Vector3 up = Normalize(Cross(look, right));
	return ComputeViewMatrix(right, up, look, mPosition);
}

Matrix4 Camera::GetPerspectiveMatrix(float aspectRatio) const
{
	if (aspectRatio == 0.0f)
	{
		auto w = GraphicsSystem::Get()->GetBackBufferWidth();
		auto h = GraphicsSystem::Get()->GetBackBufferHeight();
		aspectRatio = static_cast<float>(w) / static_cast<float>(h);
	}
	return ComputePerspectiveMatrix(mNearPlane, mFarPlane, mFOV, aspectRatio);
}

void Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Camera::Strafe(float distance)
{
	const JMath::Vector3 right = JMath::Normalize(JMath::Cross(JMath::Vector3::YAxis(), mDirection));
	mPosition += right * distance;
}

void Camera::Yaw(float degree)
{
	JMath::Matrix4 matRotate = JMath::Matrix4::RotationY(degree * JMath::Constants::DegreesToRad);
	mDirection = JMath::Matrix4::TransformNormal(mDirection, matRotate);
}

void Camera::Pitch(float degree)
{
	const JMath::Vector3 right = JMath::Normalize(JMath::Cross(JMath::Vector3::YAxis(), mDirection));
	const JMath::Matrix4 matRotate = JMath::Matrix4::MatrixRotationAxis(right, degree * JMath::Constants::DegreesToRad);
	const JMath::Vector3 newLook = JMath::Matrix4::TransformNormal(mDirection, matRotate);
	const float dot = JMath::Dot(newLook, JMath::Vector3::YAxis());

	// Avoid looking straight up or down
	if (JMath::Abs(dot) < 0.995f)
	{
		mDirection = newLook;
	}
}