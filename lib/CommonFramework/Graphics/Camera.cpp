#include <common/Camera.h>
#include <common/Error.h>

COMMONFRAMEWORK_API Camera* Camera::MainCamera = nullptr;

Camera::Camera(int screenWidth, int screenHeight, float fov, float nearClip, float farClip, CameraProjection projection)
    : __screenWidth{ screenWidth }
    , __screenHeight{ screenHeight }
    , __fov{ fov }
    , __nearClip{ nearClip }
    , __farClip{ farClip }
    , __projection{ projection }
    , __yaw{ 0.0f }
    , __pitch{ 0.0f }
{
    updateProjectionMatrix();
}

Camera::~Camera()
{
    if (MainCamera == this) MainCamera = nullptr;
}

void Camera::SetPosition(const glm::vec3& position) {
    __position = position;
    updateViewMatrix();
}

void Camera::SetFOV(float fov) {
    __fov = fov;
    updateProjectionMatrix();
}

void Camera::SetNearClip(float nearClip) {
    __nearClip = nearClip;
    updateProjectionMatrix();
}

void Camera::SetFarClip(float farClip) {
    __farClip = farClip;
    updateProjectionMatrix();
}

void Camera::SetAsMainCamera()
{
    MainCamera = this;
}

glm::mat4 Camera::GetViewMatrix() const {
    return __viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return __projectionMatrix;
}

glm::vec3 Camera::GetPosition() const {
    return __position;
}

float Camera::GetYaw() const
{
    return __yaw;
}

float Camera::GetPitch() const
{
    return __pitch;
}

void Camera::Translate(const glm::vec3& translation) {
    glm::vec3 realTranslation = translation.x * __right + translation.y * __up + translation.z * __forward;
    __position += realTranslation;
    updateViewMatrix();
}

void Camera::RotateYaw(float angle)
{
    __yaw += angle;
    updateViewMatrix();
}

void Camera::RotatePitch(float angle)
{
    __pitch += angle;
    if (__pitch > 89.0f)
        __pitch = 89.0f;
    else if (__pitch < -89.0f)
        __pitch = -89.0f;
    updateViewMatrix();
}

void Camera::LookAt(const glm::vec3& target)
{
    __viewMatrix = glm::lookAt(__position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::updateViewMatrix() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(__yaw)) * cos(glm::radians(__pitch));
    front.y = sin(glm::radians(__pitch));
    front.z = sin(glm::radians(__yaw)) * cos(glm::radians(__pitch));
    __forward = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    // World up ?
    __right = glm::normalize(glm::cross(__forward, glm::vec3(0.0f, 1.0f, 0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    __up = glm::normalize(glm::cross(__right, __forward));
    __viewMatrix = glm::lookAt(__position, __position + __forward, __up);
}

void Camera::updateProjectionMatrix() {
    if (__projection == CameraProjection::Perspective)
        __projectionMatrix = glm::perspective(glm::radians(__fov), static_cast<float>(__screenWidth) / static_cast<float>(__screenHeight), __nearClip, __farClip);
    else if (__projection == CameraProjection::Orthographic)
        __projectionMatrix = glm::ortho(0.0f, static_cast<float>(__screenWidth), 0.0f, static_cast<float>(__screenHeight), __nearClip, __farClip);
    else
        throw RuntimeException("Unsupported projection type");
}

