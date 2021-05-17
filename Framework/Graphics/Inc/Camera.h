#ifndef INCLUDED_KLINK_GRAPHICS_CAMERA_H
#define INCLUDED_KLINK_GRAPHICS_CAMERA_H

namespace Klink::Graphics
{
	using namespace JMath;

	Matrix4 ComputeViewMatrix(
		const Vector3 right,
		const Vector3 up,
		const Vector3 look,
		const Vector3 position);

	Matrix4 ComputePerspectiveMatrix(
		float nearPlane,
		float farPlane,
		float fov,
		float aspectRatio);

class Camera
{
public:

	Camera() = default;
	Camera(Vector3 _position, Vector3 _rotation, Vector3 _lookAt, float nearPl, float farPl, float FoV, int screenHeight, int screenWidth);

	void ResizeScreen(uint32_t width, uint32_t height);

	void SetPosition(const Vector3& position);
	void SetDirection(const Vector3& direction);
	void SetTarget(const Vector3& target);
	
	void SetFov(float angle, bool rad = false);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);

	void Walk(float distance);
	void Strafe(float distance);
	void Rise(float distance)	{ mPosition.y += distance; }

	void Yaw(float rotation);
	void Pitch(float rotation);

	const Vector3& GetPosition()  const { return mPosition; }
	const Vector3& GetDirection() const { return mDirection; }
	
	float GetFOV()		 const	{ return mFOV; }
	float GetNearPlane() const	{ return mNearPlane; }
	float GetFarPlane()  const	{ return mFarPlane; }

	Matrix4 GetViewMatrix() const;
	Matrix4 GetPerspectiveMatrix(float aspectRatio = 0.0f) const;

	void SetMoveSpeed(float speed) { mCameraMoveSpeed = speed; }
	void SetRotationSpeed(float speed) { mCameraRotationSpeed = speed; }

	float GetMoveSpeed() { return mCameraMoveSpeed; }
	float GetRotationSpeed() { return mCameraRotationSpeed; }

private:

	Vector3 mPosition{ 0.0f, 5.0f, -10.0f };
	Vector3 mDirection{ 0.0f, 0.0f, 1.0f };

	float mFOV{60.0f * DegreesToRad};
	float mNearPlane{ 0.1f };
	float mFarPlane{ 1000.0f };
	float mAspectRatio{ 0.0f };

	float mCameraMoveSpeed = 1.0f;
	float mCameraRotationSpeed = 1.0f;
};

}; // namespace Graphics

#endif // INCLUDED_KLINK_GRAPHICS_CAMERA_H