#include "..\Graphics\Inc\Camera.h"
#include "Precompiled.h"
#include "Camera.h"

#include "GraphicsSystem.h"

using namespace Siege;
using namespace Siege::Graphics;

Math::Matrix4 Graphics::ComputeViewMatrix(const Math::Vector3& r, const Math::Vector3& u, const Math::Vector3& l, const Math::Vector3& position)
{
	const float dx = -Math::Dot(r, position);
	const float dy = -Math::Dot(u, position);
	const float dz = -Math::Dot(l, position);
	return {
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		dx,  dy,  dz,  1.0f
	};
}

Math::Matrix4 Graphics::ComputePerspectiveMatrix(float n, float f, float fov, float aspect)
{
	const float h = 1 / tan(fov * 0.5f);
	const float w = h / aspect;
	const float zf = f;
	const float zn = n;
	const float d = zf / (zf - zn);

	return {
		w,    0.0f, 0.0f,    0.0f,
		0.0f, h,    0.0f,    0.0f,
		0.0f, 0.0f, d,       1.0f,
		0.0f, 0.0f, -zn * d, 0.0f
	};
}

void Camera::SetPosition(const Math::Vector3& position)
{
	mPosition = position;
}

void Camera::SetDirection(const Math::Vector3& direction)
{
	mDirection = Math::Normalize(direction);
}

void Camera::SetLookAt(const Math::Vector3& target)
{
	mDirection = Math::Normalize(target - mPosition);
}

void Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Camera::Strafe(float distance)
{
	const Math::Vector3 right = Math::Normalize(Math::Cross(Math::Vector3::YAxis, mDirection));
	mPosition += right * distance;
}

void Camera::Rise(float distance)
{
	mPosition += Math::Vector3::YAxis * distance;
}

void Camera::Yaw(float degree)
{
	Math::Matrix4 matRotate = Math::Matrix4::RotationY(degree * Math::Constants::DegToRad);
	mDirection = Math::TransformNormal(mDirection, matRotate);
}

void Camera::Pitch(float degree)
{
	const Math::Vector3 right = Math::Normalize(Math::Cross(Math::Vector3::YAxis, mDirection));
	const Math::Matrix4 matRotate = Math::Matrix4::RotationAxis(right, degree * Math::Constants::DegToRad);
	const Math::Vector3 newLook = Math::TransformNormal(mDirection, matRotate);
	const float dot = Math::Dot(newLook, Math::Vector3::YAxis);

	// Avoid looking straight up or down
	if (Math::Abs(dot) < 0.995f)
	{
		mDirection = newLook;
	}
}

void Camera::SetFOV(float fov)
{
	const float kMinFOV = 10.0f * Math::Constants::DegToRad;
	const float kMaxFOV = 150.0f * Math::Constants::DegToRad;
	mFov = Math::Clamp(fov, kMinFOV, kMaxFOV);
}

void Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

Math::Matrix4 Camera::GetViewMatrix() const
{
	const Math::Vector3 l = mDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross({ 0.0f, 1.0f, 0.0f }, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	return ComputeViewMatrix(r, u, l, mPosition);
}

Math::Matrix4 Camera::GetPerspectiveMatrix(float aspectRatio) const
{
	if (aspectRatio == 0.0f)
	{
		auto width = GraphicsSystem::Get()->GetBackBufferWidth();
		auto height = GraphicsSystem::Get()->GetBackBufferHeight();
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}
	return ComputePerspectiveMatrix(mNearPlane, mFarPlane, mFov, aspectRatio);
}