#include <common/Math_Base.h>

glm::vec2 WorldToScreenSpace(const glm::vec3& worldPos, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, int screenWidth, int screenHeight) {
    // Apply view and projection transformations to convert world space to clip space
    glm::vec4 clipPos = projectionMatrix * viewMatrix * glm::vec4(worldPos, 1.0f);

    // Perform perspective division to convert clip space to normalized device coordinates (NDC)
    glm::vec3 ndcPos = glm::vec3(clipPos) / clipPos.w;

    // Convert NDC to screen space coordinates
    glm::vec2 screenPos;
    screenPos.x = (ndcPos.x + 1.0f) * 0.5f * screenWidth;
    screenPos.y = (1.0f + ndcPos.y) * 0.5f * screenHeight;

    return screenPos;
}

bool isBehindCamera(const glm::vec3& worldPosition, const glm::vec3& cameraPosition, const glm::vec3& cameraForward) {
    glm::vec3 toPosition = worldPosition - cameraPosition;
    float dotProduct = glm::dot(toPosition, cameraForward);
    return dotProduct < 0.0f;
}

bool IsOnScreen(const glm::vec3& worldPosition, const glm::mat4& view, const glm::mat4& projection, int windowWidth, int windowHeight) {
    // Transform world position to clip space
    glm::vec4 clipSpacePosition = projection * view * glm::vec4(worldPosition, 1.0);

    // Normalize clip space position
    clipSpacePosition /= clipSpacePosition.w;

    // Check if the position is within the viewport
    bool onScreen = true;
    if (clipSpacePosition.x < -1.0f || clipSpacePosition.x > 1.0f ||
        clipSpacePosition.y < -1.0f || clipSpacePosition.y > 1.0f ||
        clipSpacePosition.z < -1.0f || clipSpacePosition.z > 1.0f) {
        onScreen = false;
    }

    return onScreen;
}
