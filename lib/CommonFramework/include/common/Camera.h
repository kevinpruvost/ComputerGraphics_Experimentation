#pragma once

#include <common/Math_Base.h>
#include <common/DLL.h>

class COMMONFRAMEWORK_API Camera {
public:
    enum class CameraProjection {
        Perspective,
        Orthographic
    };

    Camera(int screenWidth, int screenHeight, float fov = 45.0f, float nearClip = 0.1f, float farClip = 1000.0f,
        CameraProjection projection = CameraProjection::Perspective);
    ~Camera();

    // Setters
    void SetPosition(const glm::vec3& __position);
    void SetFOV(float fov);
    void SetNearClip(float nearClip);
    void SetFarClip(float farClip);

    void SetAsMainCamera();

    // Getters
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::vec3 GetPosition() const;
    float GetYaw() const;
    float GetPitch() const;

    // Move the camera
    void Translate(const glm::vec3& translation);
    void RotateYaw(float angle);
    void RotatePitch(float angle);
    void LookAt(const glm::vec3& target);

    static Camera * MainCamera;

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
