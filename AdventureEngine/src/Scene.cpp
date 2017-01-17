#include "Scene.h"

namespace AdventureEngine
{
	Scene::Scene(const int* windowWidth, const int* windowHeight)
	{
		m_sceneRenderer = nullptr;
		m_waterRenderer = nullptr;
		m_guiRenderer = nullptr;

		fogColor = glm::vec3(0.8f, 0.8f, 0.8f);

		m_water = nullptr;
		m_skybox = nullptr;
		m_mainCamera = nullptr;

		waterReflectionTexture = nullptr;
		waterRefractionTexture = nullptr;

		this->windowWidth = windowWidth;
		this->windowHeight = windowHeight;
	}

	Scene::~Scene()
	{
		for (unsigned int i = 0; i < m_objects.size(); i++)
		{
			delete m_objects[i];
		}

		delete m_sceneRenderer;
		delete m_waterRenderer;
		delete m_skyboxRenderer;
		delete m_guiRenderer;

		AudioManager::clean();
		AssetManager::clean();
	}

	void Scene::addObject(Object* object)
	{
		m_objects.push_back(object);
	}

	void Scene::addRenderable(RenderComponent* render)
	{
		m_renderables.push_back(render);
	}

	void Scene::setWater(WaterComponent* water)
	{
		m_water = water;
	}

	void Scene::setSkybox(SkyboxComponent* skybox)
	{
		m_skybox = skybox;
	}

	void Scene::addLight(LightComponent* light)
	{
		m_lights.push_back(light);
	}

	void Scene::addGUI(GUIComponent* gui)
	{
		m_guiObjects.push_back(gui);
	}

	void Scene::setMainCamera(CameraComponent* mainCamera)
	{
		m_mainCamera = mainCamera;
	}

	bool Scene::load()
	{
		AssetManager::init();
		AudioManager::init();

		m_sceneRenderer = new SceneRenderer();
		if (!m_sceneRenderer->load())
		{
			return false;
		}

		m_waterRenderer = new WaterRenderer(windowWidth, windowHeight);
		if (!m_waterRenderer->load())
		{
			return false;
		}

		waterReflectionTexture = m_waterRenderer->getReflectionTexture();
		waterRefractionTexture = m_waterRenderer->getRefractionTexture();

		m_skyboxRenderer = new SkyboxRenderer();
		if (!m_skyboxRenderer->load())
		{
			return false;
		}

		m_guiRenderer = new GUIRenderer();
		if (!m_guiRenderer->load())
		{
			return false;
		}

		return true;
	}

	void Scene::update(float deltaTime)
	{
		for (unsigned int i = 0; i < m_objects.size(); i++)
		{
			m_objects[i]->update(deltaTime);
		}

		Object* mainCameraObject = m_mainCamera->getObject();
		glm::vec3 forward = mainCameraObject->getForward();
		glm::vec3 up = mainCameraObject->getUp();
		ALfloat orientation[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };

		alListener3f(AL_POSITION, mainCameraObject->position.x, mainCameraObject->position.y, mainCameraObject->position.z);
		alListenerfv(AL_ORIENTATION, orientation);
	}

	void Scene::render() const
	{
		// Sets the clear color to the fog color
		glClearColor(fogColor.r, fogColor.g, fogColor.b, 1);

		renderWaterFBOs();

		// Renders the scene to the screen
		m_sceneRenderer->render(m_renderables, m_lights, m_mainCamera, fogColor);

		// Renders the skybox to the screen
		m_skyboxRenderer->render(m_skybox, m_mainCamera, fogColor);

		// Renders the water
		m_waterRenderer->render(m_water, m_lights, m_mainCamera, fogColor);

		// Renders the GUI
		m_guiRenderer->render(m_guiObjects);
	}

	void Scene::renderWaterFBOs() const
	{
		const Object* waterObject = m_water->getObject();
		Object* mainCameraObject = m_mainCamera->getObject();

		float heightAboveWater = mainCameraObject->position.y - waterObject->position.y;

		// Adjusts the camera for the reflection
		mainCameraObject->position.y -= heightAboveWater * 2;
		mainCameraObject->rotation.x *= -1;
		mainCameraObject->rotation.z *= -1;

		// Binds rendering to the reflection FBO
		m_waterRenderer->bindReflectionFBO();

		// Renders the scene to the reflection FBO
		m_sceneRenderer->render(m_renderables, m_lights, m_mainCamera, fogColor, glm::vec4(0, 1, 0, -waterObject->position.y));

		// Renders the skybox to the reflection FBO
		m_skyboxRenderer->render(m_skybox, m_mainCamera, fogColor);

		// Puts the camera back
		mainCameraObject->position.y += heightAboveWater * 2;
		mainCameraObject->rotation.x *= -1;
		mainCameraObject->rotation.z *= -1;

		// Binds rendering to the refraction FBO
		m_waterRenderer->bindRefractionFBO();

		// Renders the scene to the refraction FBO
		m_sceneRenderer->render(m_renderables, m_lights, m_mainCamera, fogColor, glm::vec4(0, -1, 0, waterObject->position.y));

		// Renders the skybox to the refraction FBO
		m_skyboxRenderer->render(m_skybox, m_mainCamera, fogColor);

		// Unbinds the FBO
		m_waterRenderer->unbindFBOs();
	}
}
