#pragma once

#include <common/Math_Base.h>
#include <common/DLL.h>

class Camera {
public:
    enum class CameraProjection {
        Perspective,
        Orthographic
    };

    EXPORT Camera(int screenWidth, int screenHeight, float fov = 45.0f, float nearClip = 0.1f, float farClip = 1000.0f,
        CameraProjection projection = CameraProjection::Perspective);

    // Setters
    EXPORT void SetPosition(const glm::vec3& __position);
    EXPORT void SetFOV(float fov);
    EXPORT void SetNearClip(float nearClip);
    EXPORT void SetFarClip(float farClip);

    // Getters
    EXPORT glm::mat4 GetViewMatrix() const;
    EXPORT glm::mat4 GetProjectionMatrix() const;
    EXPORT glm::vec3 GetPosition() const;
    EXPORT float GetYaw() const;
    EXPORT float GetPitch() const;

    // Move the camera
    EXPORT void Translate(const glm::vec3& translation);
    EXPORT void RotateYaw(float angle);
    EXPORT void RotatePitch(float angle);
    EXPORT void LookAt(const glm::vec3& target);

private:
    int __screenWidth, __screenHeight;
    float __fov, __nearClip, __farClip;
    glm::vec3 __position;
    glm::mat4 __viewMatrix;
    glm::mat4 __projectionMatrix;
    float __yaw, __pitch;
    glm::vec3 __forward, __right, __up;
    CameraProjection __projection;

    // Update the view matrix based on position and rotation
    void updateViewMatrix();

    // Update the projection matrix based on screen dimensions and projection type
    void updateProjectionMatrix();
};
