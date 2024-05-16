#pragma once

#include <common/Scene.h>
#include <common/ShaderPipeline.h>
#include <common/Model.h>
#include <common/Text2D.h>
#include <common/Camera.h>


class MainScene : public Scene
{
public:
	MainScene();
	Venom::ErrorCode _Init() override;
    Venom::ErrorCode Update() override;
protected:
	int displayMode = 1;
	Ptr<ShaderPipeline> m_shader, m_skyboxShader, m_textShader, m_particleShader, m_wireframeShader;
	Ptr<Model> m_sphereModel;
	Ptr<Text2D> m_text2D;
	UPtr<Camera> camera;

	bool cameraLock = false;
	bool drawFaces = true;
	bool drawLines = false;
	bool drawPoints = false;

	float rotationSpeed = 1.0f;
	glm::vec3 verticesColor = { 1.0f, 0.0f, 0.0f };
};