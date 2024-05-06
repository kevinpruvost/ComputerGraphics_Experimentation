#pragma once

#include <common/Scene.h>

class MainScene : public Scene
{
public:
	MainScene(Window* window, BaseFramework* framework, GUI* g);
    void Update() override;
protected:
	int displayMode = 1;
	Ptr<ShaderPipeline> m_shader, m_textShader, m_particleShader, m_wireframeShader;
	Ptr<Model>	 m_sphereModel;
	Ptr<Texture> m_textureParticles, m_backgroundTexture;
	Ptr<ParticleSystem> m_ParticleSystem, m_particlesystem2;
	Ptr<Text2D> m_text2D;
	Ptr<Entity> m_sun;
	UPtr<Skybox> m_skybox;
	std::vector<Entity*> m_objects;
	Camera camera;

	bool cameraLock = false;
	bool drawFaces = true;
	bool drawLines = false;
	bool drawPoints = false;

	float rotationSpeed = 1.0f;
	glm::vec3 verticesColor = { 1.0f, 0.0f, 0.0f };
};