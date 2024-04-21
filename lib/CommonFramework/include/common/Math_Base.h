#pragma once

// For Vulkan & DX11, because OpenGL works with right hand
// #define GLM_FORCE_LEFT_HANDED
// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec2 WorldToScreenSpace(const glm::vec3& worldPos, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, int screenWidth, int screenHeight);
bool isBehindCamera(const glm::vec3& worldPosition, const glm::vec3& cameraPosition, const glm::vec3& cameraForward);
bool IsOnScreen(const glm::vec3& worldPosition, const glm::mat4& view, const glm::mat4& projection, int windowWidth, int windowHeight);